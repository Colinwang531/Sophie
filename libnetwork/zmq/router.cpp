#include "boost/format.hpp"
#include "zmq.h"
#include "const.h"
#include "error.h"
#include "router.h"

namespace framework
{
    namespace libnetwork
    {
        namespace zmq
        {
			Router::Router()
			{}
			Router::~Router()
			{}

			int Router::bind(
				const std::string ipv4,
				const unsigned short port /* = 0 */,
				void* s /* = nullptr */)
			{
				CommonError e{ 
					s && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					const std::string address{
						(boost::format("tcp://%s:%d") % (ipv4.empty() ? "*" : ipv4) % port).str()};
					zmq_bind(s, address.c_str());
				}

				return (int)e;
			}

			int Router::send(
				const std::string targetID,
				const std::string data,
				void* s /* = nullptr */)
			{
				CommonError e{
					!targetID.empty() && !data.empty() && s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{

				}

				return (int)e;
			}
        }//namespace zeromq
    }//namespace network
}//namespace framework

