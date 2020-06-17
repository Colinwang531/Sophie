#ifndef _WINDOWS
#include <string.h>
#endif//_WINDOWS
#include "zmq.h"
#include "MessageQueue/AsynchronousSender.h"

namespace mq
{
	namespace module
	{
		AsynchronousSender::AsynchronousSender(const unsigned int bytes /* = mtu */) : disassemblyDataBytes{ bytes }
		{}
		AsynchronousSender::~AsynchronousSender() {}

		const unsigned int AsynchronousSender::sendData(
			void* s /* = nullptr */, const char* data /* = nullptr */, const unsigned int bytes /* = 0 */, const bool more /* = false */)
		{
			unsigned int sentBytes{ 0 };

			if (s && data && 0 < bytes)
			{
				//按照mtu限制将数据进行分包发送
				unsigned int totalBytes{ bytes };

				while (0 < totalBytes)
				{
					const bool anymore{ totalBytes > disassemblyDataBytes ? true : false };
					const unsigned int sendingBytes{ 
						totalBytes > disassemblyDataBytes ? disassemblyDataBytes : totalBytes };
					const unsigned int actuallySentBytes{
						sendDisassemblyData(s, data + sentBytes, sendingBytes, more ? true : anymore) };

					if (0 < actuallySentBytes)
					{
						sentBytes += actuallySentBytes;
						totalBytes -= actuallySentBytes;
					}
					else
					{
						break;
					}
				}
			}

			return sentBytes;
		}

		const unsigned int AsynchronousSender::sendDisassemblyData(
			void* s /* = nullptr */, const char* data /* = nullptr */, const unsigned int bytes /* = 0 */, const bool more /* = false */)
		{
			unsigned int sentBytes{ 0 };
			zmq_msg_t msg;

			if (!zmq_msg_init_size(&msg, bytes))
			{
#ifdef _WINDOWS
				memcpy_s(zmq_msg_data(&msg), bytes, data, bytes);
#else
				memcpy(zmq_msg_data(&msg), data, bytes);
#endif//_WINDOWS
				sentBytes = zmq_msg_send(&msg, s, more ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT);
				zmq_msg_close(&msg);
		}

			return sentBytes;
	}
	}//namespace module
}//namespace mq
