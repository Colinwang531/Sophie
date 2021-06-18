#include <new>
#include "boost/format.hpp"
#include "zmq.h"
#include "const.h"
#include "error.h"
#include "zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "zmq/pub.h"
using Pub = framework::libnetwork::zmq::Pub;
#include "zmq/msg.h"
using Msg = framework::libnetwork::zmq::Msg;
#include "publisher.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				class IPublisher
				{
				public:
					IPublisher(void);
					~IPublisher(void);

				public:
					CommonError bind(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					CommonError send(void* msg = nullptr);

				protected:
					void* ps;
				};//class IPublisher

				IPublisher::IPublisher() : ps{nullptr}
				{}
				IPublisher::~IPublisher()
				{}

				CommonError IPublisher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{ 
						ctx_ && ctx_->valid() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						ps = ctx_->createNewSocket(ZMQ_PUB);

						if (ps)
						{
							e = static_cast<CommonError>(Pub().bind(ipv4, port, ps, 10));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
						}
					}

					return e;
				}

				CommonError IPublisher::send(void* msg /*= nullptr*/)
				{
					CommonError e{
						msg ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						Msg* msg_{reinterpret_cast<Msg*>(msg)};
						e = static_cast<CommonError>(msg_->send(ps));
					}
					
					return e;
				}

				Publisher::Publisher() : publisher{ new(std::nothrow) IPublisher }
				{}
				Publisher::~Publisher()
				{
					boost::checked_delete(publisher);
				}

				int Publisher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return static_cast<int>(publisher ? publisher->bind(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Publisher::send(void* msg /* = nullptr */)
				{
					return static_cast<int>(publisher ? publisher->send(msg) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}
			}//namespace module
		}//namespace zmq
	}//namespace network
}//namespace framework
