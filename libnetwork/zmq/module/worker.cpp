#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/function.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "libnetwork/zmq/dealer.h"
using Dealer = framework::libnetwork::zmq::Dealer;
#include "libnetwork/zmq/msg.h"
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
				typedef boost::function<void(const std::string)> AfterDealerPollDataHandler;

				class IWorker
				{
				public:
					IWorker(void* c = nullptr);
					~IWorker(void);

				public:
					CommonError connect(
						const std::string sourceID,
						const std::string ipv4,
						const unsigned short port = 0,
						AfterDealerPollDataHandler handler = nullptr);
					CommonError send(const std::string data);

				private:
					static void dealerPollDataHandler(
						const std::string data, 
						void* userData = nullptr);

				private:
					void* ctx;
					Dealer* dealer;
					void* ttid;
					unsigned long long timerTaskInterval;
					bool stopped;
					AfterDealerPollDataHandler afterDealerPollDataHandler;
				};//class IWorker

				IWorker::IWorker(void* c /*= nullptr*/) 
					: ctx{c}, dealer{nullptr}, ttid{nullptr}, timerTaskInterval{30}, stopped{false}
				{}
				IWorker::~IWorker()
				{
					stopped = true;
					boost::checked_delete(dealer);
				}

				CommonError IWorker::connect(
					const std::string sourceID,
					const std::string ipv4, 
					const unsigned short port /* = 0 */,
					AfterDealerPollDataHandler handler /*= nullptr*/)
				{
					CommonError e{ 
						dealer ? CommonError::COMMON_ERROR_EXISTED : CommonError::COMMON_ERROR_SUCCESS };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						dealer = boost::factory<Dealer*>()(ctx, &IWorker::dealerPollDataHandler, this);

						if (dealer)
						{
							afterDealerPollDataHandler = handler;
							e = static_cast<CommonError>(dealer->start(sourceID, ipv4, port));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_OPERATE;
						}
					}

					return e;
				}

				CommonError IWorker::send(const std::string data)
				{
					CommonError e{ 
						dealer ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = static_cast<CommonError>(dealer->send(data));
					}

					return e;
				}

				void IWorker::dealerPollDataHandler(
					const std::string data, 
					void* userData /*= nullptr*/)
				{
					IWorker* w{reinterpret_cast<IWorker*>(userData)};

					if (w)
					{
						if (w->afterDealerPollDataHandler)
						{
							w->afterDealerPollDataHandler(data);
						}
					}
				}

				Worker::Worker(void* c /*= nullptr*/) : worker{ boost::factory<IWorker*>()(c) }
				{}
				Worker::~Worker()
				{
					boost::checked_delete(worker);
				}

				int Worker::connect(
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
						e = (worker ? 
							worker->connect(sourceID, ipv4, port, boost::bind(&Worker::afterWorkerPollDataProcess, this, _1)) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}

					return static_cast<int>(e);
				}

				int Worker::send(const std::string data)
				{
					CommonError e{ 
						!data.empty() ? 
						CommonError::COMMON_ERROR_SUCCESS : 
						CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = (worker ? 
							worker->send(data) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}

					return static_cast<int>(e);
				}
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework
