#ifndef _WINDOWS
#include <string.h>
#endif//_WINDOWS
#include "zmq.h"
#include "Define.h"
#include "MessageQueue/AsynchronousSender.h"

namespace mq
{
	namespace module
	{
		AsynchronousSender::AsynchronousSender() {}
		AsynchronousSender::~AsynchronousSender() {}

		const unsigned long long AsynchronousSender::sendData(
			void* s /* = nullptr */, 
			const void* data /* = nullptr */, 
			const unsigned long long bytes /* = 0 */, 
			const bool forcemore /* = false */)
		{
			unsigned long long sentPartialBytes{ 0 }, totalSendBytes{ bytes };

			if (s && data && 0 < bytes)
			{
				while (0 < totalSendBytes)
				{
					const unsigned long long sentDataBytes{
						sendPartialData(
							s, 
							data + sentPartialBytes, 
							totalSendBytes > gMaxTransmitUnit ? gMaxTransmitUnit : totalSendBytes, 
							forcemore ? true : (totalSendBytes > gMaxTransmitUnit ? true : false)) };

					if (0 < sentDataBytes)
					{
						sentPartialBytes += sentDataBytes;
						totalSendBytes -= sentDataBytes;
					}
					else
					{
						break;
					}
				}
			}

			return sentPartialBytes;
		}

		const unsigned long long AsynchronousSender::sendPartialData(
			void* s /* = nullptr */, 
			const void* data /* = nullptr */, 
			const unsigned long long bytes /* = 0 */, 
			const bool more /* = false */)
		{
			unsigned long long sentBytes{ 0 };
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
