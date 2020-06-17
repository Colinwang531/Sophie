#ifndef _WINDOWS
#include <string.h>
#endif//_WINDOWS
#include "zmq.h"
#include "Error.h"
#include "MessageQueue/AsynchronousReceiver.h"

namespace mq
{
	namespace module
	{
		AsynchronousReceiver::AsynchronousReceiver(){}
		AsynchronousReceiver::~AsynchronousReceiver(){}

		const int AsynchronousReceiver::receiveData(
			void* s /* = nullptr */, char* data /* = nullptr */, const unsigned int bytes /* = 0 */)
		{
			int e{s && data && 0 < bytes ? eSuccess : eInvalidParameter};

			if (eSuccess == e)
			{
				zmq_msg_t msg;
				zmq_msg_init(&msg);
				int more{ ZMQ_MORE };

				while (more)
				{
					zmq_msg_recv(&msg, s, ZMQ_DONTWAIT);
					more = zmq_msg_more(&msg);
				}

				const unsigned int totalBytes{ zmq_msg_size(&msg) };
				if (totalBytes > bytes)
				{
					e = eOutOfRange;
				}
				else
				{
#ifdef _WINDOWS
					memcpy_s(data, totalBytes, zmq_msg_data(&msg), totalBytes);
#else
					memcpy(data, zmq_msg_data(&msg), totalBytes);
#endif//_WINDOWS
					e = static_cast<const int>(totalBytes);
				}

				zmq_msg_close(&msg);
			}

			return e;
		}
	}//namespace module
}//namespace mq
