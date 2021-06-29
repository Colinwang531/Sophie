#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/const.h"
#include "libcommon/error.h"
#include "libnetwork/zmq/ctx.h"
using Ctx = framework::libnetwork::zmq::Ctx;
#include "libnetwork/zmq/pub.h"
using Pub = framework::libnetwork::zmq::Pub;
#include "libnetwork/zmq/msg.h"
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
					IPublisher(void* c = nullptr);
					~IPublisher(void);

				public:
					CommonError bind(
						const std::string ipv4,
						const unsigned short port = 0);
					CommonError send(const std::string data);

				protected:
					void* ctx;
					Pub* pub;
				};//class IPublisher

				IPublisher::IPublisher(void* c /*= nullptr*/) : ctx{c}, pub{nullptr}
				{}
				IPublisher::~IPublisher()
				{
					boost::checked_delete(pub);
				}

				CommonError IPublisher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */)
				{
					CommonError e{ 
						pub ? CommonError::COMMON_ERROR_EXISTED : CommonError::COMMON_ERROR_SUCCESS };
						
					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						pub = boost::factory<Pub*>()(ctx);

						if (pub)
						{
							e = static_cast<CommonError>(pub->start(ipv4, port, 10));
						}
						else
						{
							e = CommonError::COMMON_ERROR_BAD_OPERATE;
						}
					}

					return e;
				}

				CommonError IPublisher::send(const std::string data)
				{
					CommonError e{ 
						pub ? CommonError::COMMON_ERROR_EXISTED : CommonError::COMMON_ERROR_SUCCESS };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = static_cast<CommonError>(pub->send(data));
					}

					return e;
				}

				Publisher::Publisher(void* c /*= nullptr*/) : publisher{ boost::factory<IPublisher*>()(c) }
				{}
				Publisher::~Publisher()
				{
					boost::checked_delete(publisher);
				}

				int Publisher::bind(
					const std::string ipv4, 
					const unsigned short port /* = 0 */)
				{
					CommonError e{ 
						!ipv4.empty() && gMinPortNumber <= port && gMaxPortNumber >= port ? 
						CommonError::COMMON_ERROR_SUCCESS : 
						CommonError::COMMON_ERROR_INVALID_PARAMETER };

					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = (publisher ? 
							publisher->bind(ipv4, port) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}
					
					return static_cast<int>(e);
				}

				int Publisher::send(const std::string data)
				{
					CommonError e{ 
						!data.empty() ? 
						CommonError::COMMON_ERROR_SUCCESS : 
						CommonError::COMMON_ERROR_INVALID_PARAMETER };
						
					if (CommonError::COMMON_ERROR_SUCCESS == e)
					{
						e = (publisher ? 
							publisher->send(data) : 
							CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
					}
					
					return static_cast<int>(e);
				}
			}//namespace module
		}//namespace zmq
	}//namespace network
}//namespace framework
