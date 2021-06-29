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
#include "dealer.h"

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class IDealer
			{
			public:
				IDealer(
					void* c = nullptr,
					DealerPollDataHandler handler = nullptr,
					void* param = nullptr);
				~IDealer(void);

			public:
				CommonError start(
					const std::string sourceID,
					const std::string ipv4,
					const unsigned short port = 0);
				CommonError stop(void);
				CommonError send(const std::string data);

			private:
				//数据读取线程
				void pollDataThread(void);

			private:
				void* so;
				void* ctx;
				void* tid;
				bool stopped;
				DealerPollDataHandler dealerPollDataHandler;
				void* userData;
			};//class IDealer

			IDealer::IDealer(
				void* c /*= nullptr*/,
				DealerPollDataHandler handler /*= nullptr*/,
				void* param /*= nullptr*/)
				: so{nullptr}, tid{nullptr}, ctx{c}, stopped{false}, 
				dealerPollDataHandler{handler}, userData{param}
			{}
			IDealer::~IDealer()
			{
				stop();
			}

			CommonError IDealer::start(
				const std::string sourceID,
				const std::string ipv4, 
				const unsigned short port /* = 0 */)
			{
				CommonError e{ 
					ctx ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Ctx* c{reinterpret_cast<Ctx*>(ctx)};

					if (c)
					{
						so = c->createNewSocket(ZMQ_DEALER);
						//开启TCP保活机制防止网络连接因长时间无数据而被中断
						int keepalive{ 1 };
						zmq_setsockopt(so, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
						//网络连接空闲30s即发送保活包
						int idle{ 30 };
						zmq_setsockopt(so, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
						//设置ID标识
						zmq_setsockopt(so, ZMQ_IDENTITY, sourceID.c_str(), sourceID.length());
						const std::string address{
							(boost::format("tcp://%s:%d") % ipv4 % port).str()};
						e = (-1 < zmq_connect(so, address.c_str()) ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE);

						if (CommonError::COMMON_ERROR_SUCCESS == e)
						{
							tid = ThreadPool().create(boost::bind(&IDealer::pollDataThread, this));
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

			CommonError IDealer::stop()
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

			CommonError IDealer::send(const std::string data)
			{
				CommonError e{
					so ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					Msg msg;
					msg.add("");
					msg.add(data);
					e = static_cast<CommonError>(msg.send(so));
				}

				return e;
			}

			void IDealer::pollDataThread()
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
							msg.remove();
							const std::string data{msg.remove()};
							
							if (dealerPollDataHandler)
							{
								dealerPollDataHandler(data, userData);
							}
						}
					}
				}
			}

			Dealer::Dealer(
				void* c /*= nullptr*/,
				DealerPollDataHandler handler /*= nullptr*/,
				void* param /*= nullptr*/)
				: dealer{boost::factory<IDealer*>()(c, handler, param)}
			{}
			Dealer::~Dealer()
			{
				boost::checked_delete(dealer);
			}

			int Dealer::start(
				const std::string sourceID,
				const std::string ipv4, 
				const unsigned short port /* = 0 */)
			{
				CommonError e{ 
					!sourceID.empty() && !ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = dealer ? dealer->start(sourceID, ipv4, port) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}

			int Dealer::stop()
			{
				return static_cast<int>(dealer ? dealer->stop() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Dealer::send(const std::string data)
			{
				CommonError e{
					!data.empty() ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = dealer ? dealer->send(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
				}

				return static_cast<int>(e);
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework
