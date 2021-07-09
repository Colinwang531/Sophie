#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "ctx.h"
#include "msg.h"
#include "pub.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			class IPub
			{
			public:
				IPub(void* c = nullptr);
				~IPub(void);

			public:
				CommonError start(
					const std::string localIP,
					const unsigned short localPort = 0,
					const int hwm = -1);
				CommonError stop(void);
				CommonError send(const std::string data);

			private:
				void* so;
				void* ctx;
			};//class IPub

			IPub::IPub(void* c /*= nullptr*/) : so{nullptr}, ctx{c}
			{}
			IPub::~IPub()
			{
				stop();
			}

			CommonError IPub::start(
				const std::string localIP,
				const unsigned short localPort /*= 0*/,
				const int hwm /*= -1*/)
			{
				CommonError e{
					ctx ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Ctx* c{reinterpret_cast<Ctx*>(ctx)};

					if (c)
					{
						so = c->createNewSocket(ZMQ_PUB);
						zmq_setsockopt(so, ZMQ_SNDHWM, &hwm, sizeof(int));
						const std::string address{
							(boost::format("tcp://%s:%d") % (localIP.empty() ? "*" : localIP) % localPort).str()};
						zmq_bind(so, address.c_str());
					}
					else
					{
						e = CommonError::COMMON_ERROR_SET_FAILED;
					}
				}

				return e;
			}

			CommonError IPub::stop()
			{
				Ctx* c{reinterpret_cast<Ctx*>(ctx)};

				if (c)
				{
					c->destroySocket(so);
					so = nullptr;
				}

				return CommonError::COMMON_ERROR_SUCCESS;
			}

			CommonError IPub::send(const std::string data)
			{
				CommonError e{
					so ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Msg msg;
					msg.add(data);
					e = static_cast<CommonError>(msg.send(so));
				}

				return e;
			}

			Pub::Pub(void* c /*= nullptr*/) : pub{boost::factory<IPub*>()(c)}
			{}
			Pub::~Pub()
			{
				boost::checked_delete(pub);
			}

			int Pub::start(
				const std::string localIP,
				const unsigned short localPort /* = 0 */, 
				const int hwm /* = -1 */)
			{
				CommonError e{ 
					!localIP.empty() && gMinPortNumber <= localPort && gMaxPortNumber >= localPort ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = pub ? pub->start(localIP, localPort, hwm) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}

			int Pub::stop()
			{
				return static_cast<int>(pub ? pub->stop() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Pub::send(const std::string data)
			{
				CommonError e{
					!data.empty() ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = pub ? pub->send(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework
