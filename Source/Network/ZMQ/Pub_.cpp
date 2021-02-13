#include "boost/format.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Define.h"
#include "Error.h"
#include "Network/ZMQ/Core/Ctx_.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/Pub_.h"

namespace framework
{
	namespace network
	{
		namespace zeromq
		{
			Pub::Pub() : so{ nullptr }
			{}
			Pub::~Pub()
			{}

			int Pub::startPub(const unsigned short port /* = 0 */)
			{
				int e{ 
					gMinPortNumber < port && gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					e = Ctx().get_mutable_instance().init();

					if (eSuccess == e)
					{
						so = Ctx().get_mutable_instance().createNewSocket(ZMQ_PUB);

						if (so)
						{
							zmq::socket_base_t* s{ reinterpret_cast<zmq::socket_base_t*>(so) };
							const int hwm{ 10 };
							s->setsockopt(ZMQ_SNDHWM, &hwm, sizeof(hwm));
							e = s->bind((boost::format("tcp://*:%d") % port).str().c_str()) ? eBadBind : eSuccess;
						} 
						else
						{
							stopPub();
						}
					}
				}

				return e;
			}

			int Pub::stopPub()
			{
				Ctx().get_mutable_instance().destroySocket(so);
				return Ctx().get_mutable_instance().uninit();
			}

			int Pub::sendMsg(Msg* msg /* = nullptr */)
			{
				return msg ? msg->sendData(so) : eInvalidParameter;
			}
		}//namespace zeromq
	}//namespace network
}//namespace framework
