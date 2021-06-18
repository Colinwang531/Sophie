#include <new>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "zmq.h"
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "time/time.h"
using Time = framework::libcommon::Time;
#include "thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "zmq/dealer.h"
using Dealer = framework::libnetwork::zmq::Dealer;
#include "zmq/msg.h"
using Msg = framework::libnetwork::zmq::Msg;
#include "worker.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				typedef boost::function<void(const std::string)> DealerPollDataHandler;
				typedef boost::function<void(void)> ProcessTimerTaskHandler;

				class IWorker
				{
				public:
					IWorker(void);
					~IWorker(void);

				public:
					CommonError connect(
						const std::string ipv4,
						const unsigned short port = 0,
						void* ctx = nullptr);
					//启动
					//@dpoll : Dealer端数据接收回调
					//@task : 模型定时器任务回调
					//@interval : 定时器任务执行时间间隔，以秒为单位
					//@Return : 错误码
					CommonError start(
						DealerPollDataHandler dpoll = nullptr,
						ProcessTimerTaskHandler task = nullptr,
						const unsigned long long interval = 30);
					CommonError stop(void* ctx = nullptr);

				private:
					//网络数据读取线程
					void pollDataThread(void);
					//定时任务执行线程
					void processTimerTaskThread(void);

				private:
					void* ds;
					void* poll_tid;
					void* task_tid;
					bool stopped;
					unsigned long long timerTaskInterval;
					DealerPollDataHandler dealerPollDataHandler;
					ProcessTimerTaskHandler processTimerTaskHandler;
				};//class IWorker

				IWorker::IWorker() 
					: ds{nullptr}, poll_tid{nullptr}, task_tid{nullptr},stopped{ false }, timerTaskInterval{0}
				{}
				IWorker::~IWorker()
				{
					stop();
				}

				CommonError IWorker::connect(
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

				CommonError IWorker::start(
					DealerPollDataHandler dpoll /*= nullptr*/,
					ProcessTimerTaskHandler task /*= nullptr*/,
					const unsigned long long interval /*= 30*/)
				{
					CommonError e{
						 ds ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						dealerPollDataHandler = dpoll;
						processTimerTaskHandler = task;
						timerTaskInterval = interval;

						poll_tid = ThreadPool().create(boost::bind(&IWorker::pollDataThread, this));
						if (0 < timerTaskInterval)
						{
							task_tid = ThreadPool().create(boost::bind(&IWorker::processTimerTaskThread, this));
						}
					}

					return e;
				}

				CommonError IWorker::stop(void* ctx/* = nullptr*/)
				{
					Ctx* ctx_{reinterpret_cast<Ctx*>(ctx)};
					CommonError e{
						ctx_ && ctx_->valid() && false == stopped ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE};

					if(CommonError::COMMON_ERROR_SUCCESS == e)
					{
						stopped = true;
						ctx_->destroySocket(ds);
						ThreadPool().destroy(poll_tid);
						ThreadPool().destroy(task_tid);
					}

					return e;
				}

				void IWorker::pollDataThread()
				{
					zmq_pollitem_t pollitems[1]{ { ds, 0, ZMQ_POLLIN, 0} };

					while (!stopped)
					{
						Msg msg;
						zmq_poll(pollitems, 1, 1);
						if (pollitems[0].revents & ZMQ_POLLIN)
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

				void IWorker::processTimerTaskThread()
				{
					const unsigned long long interval{timerTaskInterval * 1000};
					unsigned long long last{0};

					while (!stopped)
					{
						unsigned long long now{Time().tickcount()};

						if (0 == last || now - last >= interval)
						{
							if(processTimerTaskHandler)
							{
								processTimerTaskHandler();
							}

							last = now;
						}
						
					}
				}

				Worker::Worker() : worker{ new(std::nothrow) IWorker }
				{}
				Worker::~Worker()
				{}

				int Worker::connect(
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					void* ctx /* = nullptr */)
				{
					return static_cast<int>(worker ? worker->connect(ipv4, port, ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Worker::start(const unsigned long long interval/* = 30*/)
				{
					return static_cast<int>(
						worker ? 
						worker->start(
							boost::bind(&Worker::afterDealerPollDataProcess, this, _1), 
							boost::bind(&Worker::afterTimerExpiredProcess, this), 
							interval) : 
						CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				int Worker::stop(void* ctx/* = nullptr*/)
				{
					return static_cast<int>(worker ? worker->stop(ctx) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework
