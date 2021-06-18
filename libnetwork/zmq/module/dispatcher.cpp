#include <new>
#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/function.hpp"
#include "zmq.h"
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "zmq/router.h"
using Router = framework::libnetwork::zmq::Router;
#include "zmq/dealer.h"
using Dealer = framework::libnetwork::zmq::Dealer;
#include "zmq/msg.h"
using Msg = framework::libnetwork::zmq::Msg;
#include "dispatcher.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				typedef boost::function<void(const std::string, const std::string)> RouterPollDataHandler;
				typedef boost::function<void(const std::string)> DealerPollDataHandler;

				class IDispatcher
				{
				public:
					IDispatcher(void);
					~IDispatcher(void);

				public:
					CommonError bind(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					CommonError connect(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					//启动
					//@rpoll : Router端数据接收回调
					//@dpoll : Dealer端数据接收回调
					//@Return : 错误码
					CommonError start(
						RouterPollDataHandler rpoll = nullptr, 
						DealerPollDataHandler dpoll = nullptr);
					CommonError stop(void* ctx = nullptr);

				private:
					//网络数据读取线程
					void pollDataThread(void);

				private:
					void* rs;
					void* ds;
					void* tid;
					bool stopped;
					RouterPollDataHandler routerPollDataHandler;
					DealerPollDataHandler dealerPollDataHandler;
				};//class IDispatcher

				IDispatcher::IDispatcher() 
					: rs{nullptr}, ds{nullptr}, tid{nullptr}, stopped{ false }
				{}
				IDispatcher::~IDispatcher()
				{
					stop();
				}

				CommonError IDispatcher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{ 
						ctx_ && ctx_->valid() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						rs = ctx_->createNewSocket(ZMQ_ROUTER);

						if (rs)
						{
							e = static_cast<CommonError>(Router().bind(ipv4, port, rs));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
						}
					}

					return e;
				}

				CommonError IDispatcher::connect(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{ 
						ctx_ && ctx_->valid() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						ds = ctx_->createNewSocket(ZMQ_DEALER);

						if (ds)
						{
							e = static_cast<CommonError>(Dealer().connect(ipv4, port, ds));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
						}
					}

					return e;
				}

				CommonError IDispatcher::start(
					RouterPollDataHandler rpoll /*= nullptr*/, 
					DealerPollDataHandler dpoll /*= nullptr*/)
				{
					CommonError e{
						rs || ds ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						routerPollDataHandler = rpoll;
						dealerPollDataHandler = dpoll;
						tid = ThreadPool().create(boost::bind(&IDispatcher::pollDataThread, this));
						e = tid ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_THREAD;
					}

					return e;
				}

				CommonError IDispatcher::stop(void* ctx /*= nullptr*/)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{
						ctx_ && ctx_->valid() && false == stopped ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						stopped = true;
						ctx_->destroySocket(rs);
						ctx_->destroySocket(ds);
						ThreadPool().destroy(tid);
						tid = nullptr;
					}

					return e;
				}

				void IDispatcher::pollDataThread()
				{
					zmq_pollitem_t pollitems[2]{ 
						{ rs, 0, ZMQ_POLLIN, 0}, { ds, 0, ZMQ_POLLIN, 0} };

					while (!stopped)
					{
						Msg msg;
						zmq_poll(pollitems, 2, 1);
						if (pollitems[0].revents & ZMQ_POLLIN)
						{
							if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(msg.receive(rs)))
							{
								const std::string sender{msg.remove()};
								msg.remove();
								const std::string data{msg.remove()};
								
								if (routerPollDataHandler)
								{
									routerPollDataHandler(sender, data);
								}
								
							}
						}
						else if (pollitems[1].revents & ZMQ_POLLIN)
						{
							if (CommonError::COMMON_ERROR_SUCCESS == static_cast<CommonError>(msg.receive(ds)))
							{
								const std::string data{msg.remove()};

								if(dealerPollDataHandler)
								{
									dealerPollDataHandler(data);
								}
							}
						}
					}
				}

				Dispatcher::Dispatcher() : dispatcher{new(std::nothrow) IDispatcher}
				{}
				Dispatcher::~Dispatcher()
				{
					boost::checked_delete(dispatcher);					
				}

				int Dispatcher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return static_cast<int>(dispatcher ? dispatcher->bind(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Dispatcher::connect(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return static_cast<int>(dispatcher ? dispatcher->connect(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Dispatcher::start()
				{
					return static_cast<int>(dispatcher ? dispatcher->start(
						boost::bind(&Dispatcher::afterRouterPollDataProcess, this, _1, _2),
						boost::bind(&Dispatcher::afterDealerPollDataProcess, this, _1)) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Dispatcher::stop(void* ctx/* = nullptr*/)
				{
					return static_cast<int>(dispatcher ? dispatcher->stop(ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}
			}//namespace module
		}//namespace zeromq
	}//namespace network
}//namespace framework
