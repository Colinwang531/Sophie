#include "boost/bind.hpp"
#include "zmq.h"
#include "const.h"
#include "error.h"
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
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
				typedef boost::function<void<const std::string, const std::string>> RouterPollDataHandler;
				typedef boost::function<void<const std::string>> DealerPollDataHandler;

				class Impl
				{
				public:
					Impl(void);
					~Impl(void);

				public:
					int bind(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					int connect(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					//启动
					//@r : Router端数据接收回调
					//@d : Dealer端数据接收回调
					//@Return : 错误码
					int start(
						RouterPollDataHandler r = nullptr, 
						DealerPollDataHandler d = nullptr);
					int stop(void* ctx = nullptr);

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
				};//class Impl

				Impl::Impl() 
					: rs{nullptr}, ds{nullptr}, tid{nullptr}, stopped{ false }, 
					routerPollDataHandler{nullptr}, dealerPollDataHandler{nullptr}
				{}
				Impl::~Impl()
				{
					stop();
				}

				int Impl::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{ 
						ctx_ && ctx_.invalid() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						rs = ctx_.createNewSocket(ZMQ_ROUTER);

						if (rs)
						{
							e = Router().bind(ipv4, port, rs);
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
						}
					}

					return (int)e;
				}

				int Impl::connect(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{ 
						ctx_ && ctx_.valid() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						ds = ctx_.createNewSocket(ZMQ_DEALER);

						if (ds)
						{
							e = Dealer().bind(ipv4, port, ds);
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
						}
					}

					return (int)e;
				}

				int Impl::start(
					RouterPollDataHandler r /*= nullptr*/, 
					DealerPollDataHandler d /*= nullptr*/)
				{
					CommonError e{
						rs || ds ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						routerPollDataHandler = r;
						dealerPollDataHandler = d;
						tid = ThreadPool().create(boost::bind(&Impl::pollDataThread, this));
						e = tid ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_THREAD;
					}

					return (int)e;
				}

				int Impl::stop(void* ctx = nullptr)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{
						ctx_ && ctx_.valid() && fasle == stopped ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						stopped = true;
						ctx.destroySocket(rs);
						ctx.destroySocket(ds);
						ThreadPool().destroy(tid);
						tid = nullptr;
					}

					return (int)e;
				}

				void Impl::pollDataThread()
				{
					zmq_pollitem_t pollitems[2]{ 
						{ rs, 0, ZMQ_POLLIN, 0}, { ds, 0, ZMQ_POLLIN, 0} };

					while (!stopped)
					{
						Msg msg;
						zmq_poll(pollitems, 2, 1);
						if (pollitems[0].revents & ZMQ_POLLIN)
						{
							if (CommonError::COMMON_ERROR_SUCCESS == msg.receive(rs))
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
							if (CommonError::COMMON_ERROR_SUCCESS == msg.receive(ds))
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

				Dispatcher::Dispatcher() : impl{new(std::nothrow) Impl}
				{}
				Dispatcher::~Dispatcher()
				{
					boost::checked_delete(impl);					
				}

				int Dispatcher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return impl ? impl->bind(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				int Dispatcher::connect(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return impl ? impl->connect(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				int Dispatcher::start()
				{
					return impl ? impl->start(
						boost::bind(&Dispatcher::afterRouterPollDataProcess, this, _1, _2),
						boost::bind(&Dispatcher::afterDealerPollDataProcess, this, _1)) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				int Dispatcher::stop(void* ctx = nullptr)
				{
					return impl ? impl->stop(ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}
			}//namespace module
		}//namespace zeromq
	}//namespace network
}//namespace framework
