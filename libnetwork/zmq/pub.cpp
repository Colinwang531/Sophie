#include "boost/format.hpp"
#include "zmq.h"
#include "const.h"
#include "error.h"
#include "pub.h"

namespace framework
{
	namespace libnetwork
	{
		namespace zmq
		{
			Pub::Pub()
			{}
			Pub::~Pub()
			{}

			int Pub::bind(
				const std::string ipv4,
				const unsigned short port /* = 0 */, 
				void* s /* = nullptr */, 
				const int hwm /* = -1 */)
			{
				CommonError e{
					s && gMinPortNumber <= port && gMaxPortNumber >= port ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					int hwm{ 5 };
					zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(int));
					const std::string address{
						(boost::format("tcp://%s:%d") % (ipv4.empty() ? "*" : ipv4) % port).str()};
					zmq_bind(s, address.c_str());
				}

				return (int)e;
			}
		}//namespace zmq
	}//namespace libnetwork
}//namespace framework
