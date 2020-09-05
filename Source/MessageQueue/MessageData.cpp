#include <exception>
#include "zmq.h"
#include "Error.h"
#include "MessageQueue/MessageData.h"

namespace mq
{
    namespace message
    {
		MessageData::MessageData() {}
		MessageData::~MessageData() {}

		void MessageData::clearup()
		{
			dataGroup.clear();
		}

		int MessageData::recvData(zmq::socket_base_t* s /* = nullptr */)
		{
			int e{ eBadRecv };
			clearup();

			while (s)
			{
				zmq::msg_t msg;
				try
				{
					if (0 < s->recv(&msg, ZMQ_DONTWAIT))
					{
						const std::string msgstr{ static_cast<const char*>(msg.data()), msg.size() };
						dataGroup.push_back(msgstr);

						int more{ 0 };
						size_t bytes{ sizeof(more) };
						s->getsockopt(ZMQ_RCVMORE, &more, &bytes);
						//没有可接收的数据就可以结束了
						if (!more)
						{
							e = eSuccess;
							break;
						}
					}
					else
					{
						break;
					}
				}
				catch (const std::exception&)
				{
					break;
				}
			}

			return e;
		}

		int MessageData::sendData(zmq::socket_base_t* s /* = nullptr */)
		{
			int e{ s ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const int totalCount{ dataGroup.size() };

				for (int i = 0; i != totalCount; ++i)
				{
					const std::string msgstr{ dataGroup[i] };

					try
					{
						zmq::msg_t msg;
						memcpy(msg.data(), msgstr.c_str(), msgstr.size());
						s->send(&msg, i < totalCount - 1 ? ZMQ_SNDMORE : 0);
					}
					catch (std::exception&)
					{
						e = eBadSend;
						break;
					}
				}
				
				clearup();
			}

			return e;
		}

		int MessageData::pushFront(const std::string data)
		{
			int e{ !data.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				dataGroup.insert(dataGroup.begin(), data);
			}

			return e;
		}

		int MessageData::pushEnd(const std::string data)
		{
			int e{ !data.empty() ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				dataGroup.push_back(data);
			}

			return e;
		}

		const std::string MessageData::popFront()
		{
			std::string msgstr;

			if (0 < dataGroup.size())
			{
				msgstr = dataGroup.front();
				dataGroup.erase(dataGroup.begin());
			}

			return msgstr;
		}
    }//namespace message
}//namespace mq
