#include "boost/format.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "boost/format.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "Define.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/Core/Publisher_.h"

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			Publisher::Publisher()
			{}
			Publisher::~Publisher()
			{}

			int Publisher::bind(
				void* s /* = nullptr */, 
				const unsigned short port /* = 0 */, 
				const int hwm /* = -1 */)
			{
				int e{
					s && gMinPortNumber < port && gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					zmq::socket_base_t* so{ reinterpret_cast<zmq::socket_base_t*>(s) };
					int hwm{ 5 };
					so->setsockopt(ZMQ_SNDHWM, &hwm, sizeof(int));
					e = so->bind((boost::format("tcp://*:%d") % port).str().c_str()) ? eBadBind : eSuccess;
				}

				return e;
			}

			int Publisher::send(
				void* s /* = nullptr */, 
				Msg* msg /* = nullptr */)
			{
				return s && msg ? msg->sendData(s) : eInvalidParameter;
			}
		}//namespace zeromq
	}//namespace network
}//namespace framework
