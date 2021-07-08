#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/function.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "libnetwork/zmq/router.h"
using Router = framework::libnetwork::zmq::Router;
#include "libnetwork/zmq/msg.h"
using Msg = framework::libnetwork::zmq::Msg;
#include "switcher.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			namespace module
			{
				typedef boost::function<void(const std::string, const std::string)> AfterRouterPollDataHandler;

				class ISwitcher
				{
				public:
					ISwitcher(void* c = nullptr);
					~ISwitcher(void);

				public:
					CommonError bind(
						const std::string localIP,
						const unsigned short localPort = 0,
						AfterRouterPollDataHandler handler = nullptr);
					CommonError send(
						const std::string targetID, 
						const std::string data);

				private:
					static void routerPollDataHandler(
						const std::string sourceID, 
						const std::string data, 
						void* userData = nullptr);

				private:
					void* ctx;
					Router* router;
					AfterRouterPollDataHandler afterRouterPollDataHandler;
				};//class ISwitcher

				ISwitcher::ISwitcher(void* c /*= nullptr*/) : ctx{c}, router{nullptr}
				{}

				ISwitcher::~ISwitcher()
				{
					boost::checked_delete(router);
				}

				CommonError ISwitcher::bind(
					const std::string localIP, 
					const unsigned short localPort /* = 0 */,
					AfterRouterPollDataHandler handler /*= nullptr*/)
				{
					CommonError e{ 
						router ? CommonError::COMMON_ERROR_EXISTED : CommonError::COMMON_ERROR_SUCCESS };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						router = boost::factory<Router*>()(ctx, &ISwitcher::routerPollDataHandler, this);
							
						if (router)
						{
							afterRouterPollDataHandler = handler;
							e = static_cast<CommonError>(router->start(localIP, localPort));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_OPERATE;
						}
					}

					return e;
				}

				CommonError ISwitcher::send(
					const std::string targetID, 
					const std::string data)
				{
					CommonError e{ 
						router ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = static_cast<CommonError>(router->send(targetID, data));
					}

					return e;
				}

				void ISwitcher::routerPollDataHandler(
					const std::string sourceID, 
					const std::string data, 
					void* userData /*= nullptr*/)
				{
					ISwitcher* s{reinterpret_cast<ISwitcher*>(userData)};

					if (s)
					{
						if (s->afterRouterPollDataHandler)
						{
							s->afterRouterPollDataHandler(sourceID, data);
						}
					}
				}

				Switcher::Switcher(void* c /*= nullptr*/) : switcher{boost::factory<ISwitcher*>()(c)}
				{}
				Switcher::~Switcher()
				{
					boost::checked_delete(switcher);					
				}

				int Switcher::bind(
					const std::string localIP, 
					const unsigned short localPort /* = 0 */)
				{
					CommonError e{ 
						!localIP.empty() && gMinPortNumber <= localPort && gMaxPortNumber >= localPort ? 
						CommonError::COMMON_ERROR_SUCCESS : 
						CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = (switcher ? 
							switcher->bind(localIP, localPort, boost::bind(&Switcher::afterSwitcherPollDataProcess, this, _1, _2)) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}
					
					return static_cast<int>(e);
				}

				int Switcher::send(
					const std::string targetID, 
					const std::string data)
				{
					CommonError e{ 
						!targetID.empty() && !data.empty() ? 
						CommonError::COMMON_ERROR_SUCCESS : 
						CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = (switcher ? 
							switcher->send(targetID, data) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}
					
					return static_cast<int>(e);
				}
			}//namespace module
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework
