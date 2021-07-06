#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "ctx.h"
#include "msg.h"
#include "router.h"

namespace framework
{
    namespace libnetwork
    {
        namespace zmq
        {
			class IRouter
			{
			public:
				IRouter(
					void* c = nullptr,
					RouterPollDataHandler handler = nullptr,
					void* param = nullptr);
				~IRouter(void);

			public:
				CommonError start(
					const std::string localIP,
					const unsigned short localPort = 0);
				CommonError stop(void);
				CommonError send(
					const std::string targetID,
					const std::string data);

			private:
				//数据读取线程
				void pollDataThread(void);

			private:
				void* so;
				void* ctx;
				void* tid;
				bool stopped;
				RouterPollDataHandler routerPollDataHandler;
				void* userData;
			};//class IRouter

			IRouter::IRouter(
				void* c /*= nullptr*/,
				RouterPollDataHandler handler /*= nullptr*/,
				void* param /*= nullptr*/) 
				: so{nullptr}, ctx{c}, tid{nullptr}, stopped{false}, 
				routerPollDataHandler{handler}, userData{param}
			{}
			IRouter::~IRouter()
			{
				stop();
			}

			CommonError IRouter::start(
				const std::string localIP,
				const unsigned short localPort /* = 0 */)
			{
				CommonError e{ 
					ctx ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Ctx* c{reinterpret_cast<Ctx*>(ctx)};

					if (c)
					{
						so = c->createNewSocket(ZMQ_ROUTER);
						const std::string address{
							(boost::format("tcp://%s:%d") % (localIP.empty() ? "*" : localIP) % localPort).str()};
						e = (-1 < zmq_bind(so, address.c_str()) ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE);

						if (CommonError::COMMON_ERROR_SUCCESS == e)
						{
							tid = ThreadPool().create(boost::bind(&IRouter::pollDataThread, this));
							e = tid ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_THREAD;
						}
					}
					else
					{
						e = CommonError::COMMON_ERROR_SET_FAILED;
					}
				}

				return e;
			}

			CommonError IRouter::stop()
			{
				stopped = true;
				Ctx* c{reinterpret_cast<Ctx*>(ctx)};

				if (c)
				{
					c->destroySocket(so);
					so = nullptr;
				}
				ThreadPool().destroy(tid);
				
				return CommonError::COMMON_ERROR_SUCCESS;
			}

			CommonError IRouter::send(
				const std::string targetID,
				const std::string data)
			{
				CommonError e{
					so ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Msg msg;
					msg.add(targetID);
					msg.add("");
					msg.add(data);
					e = static_cast<CommonError>(msg.send(so));
				}

				return e;
			}

			void IRouter::pollDataThread()
			{
				zmq_pollitem_t pollitems[]{ { so, 0, ZMQ_POLLIN, 0} };

				while (!stopped)
				{
					Msg msg;
					zmq_poll(pollitems, 1, 1);
					if (pollitems[0].revents & ZMQ_POLLIN)
					{
						if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(msg.receive(so)))
						{
							const std::string sender{msg.remove()};
							msg.remove();
							const std::string data{msg.remove()};
							
							if (routerPollDataHandler)
							{
								routerPollDataHandler(sender, data, userData);
							}
						}
					}
				}
			}

			Router::Router(
				void* c /*= nullptr*/,
				RouterPollDataHandler handler /*= nullptr*/,
				void* param /*= nullptr*/) 
				: router{boost::factory<IRouter*>()(c, handler, param)}
			{}
			Router::~Router()
			{
				boost::checked_delete(router);
			}

			int Router::start(
				const std::string localIP,
				const unsigned short localPort /* = 0 */)
			{
				CommonError e{ 
					!localIP.empty() && gMinPortNumber <= localPort && gMaxPortNumber >= localPort ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = router ? router->start(localIP, localPort) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}

			int Router::stop()
			{
				return static_cast<int>(router ? router->stop() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Router::send(
				const std::string targetID,
				const std::string data)
			{
				CommonError e{
					!targetID.empty() && !data.empty() ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = router ? router->send(targetID, data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}
        }//namespace zmq
    }//namespace libnetwork
}//namespace framework

