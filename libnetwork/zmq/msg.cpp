#include <vector>
#ifdef WINDOWS
#else
#include <cstring>
#endif//WINDOWS
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "zmq.h"
#include "libcommon/error.h"
#include "msg.h"

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class IMsg
			{
			public:
				IMsg(void);
				~IMsg(void);

			public:
				CommonError add(const std::string msg);
				const std::string remove(void);
				CommonError receive(void* s = nullptr);
				CommonError send(void* s = nullptr);

			private:
				std::vector<std::string> msgs;
			};//class IMsg

			IMsg::IMsg()
			{}
			IMsg::~IMsg()
			{}

			CommonError IMsg::add(const std::string msg)
			{
				msgs.push_back(msg);
				return CommonError::COMMON_ERROR_SUCCESS;
			}

			const std::string IMsg::remove()
			{
				std::string msg;

				if (!msgs.empty())
				{
					msg = msgs[0];
					msgs.erase(msgs.begin());
				}

				return msg;
			}

			CommonError IMsg::receive(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					zmq_msg_t msg;
					zmq_msg_init(&msg);

					while (-1 < zmq_msg_recv(&msg, s, ZMQ_DONTWAIT))
					{
						const std::string data{ 
							static_cast<const char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg) };
						msgs.push_back(data);

						if (0 == zmq_msg_more(&msg))
						{
							break;
						}
					}
				}

				return e;
			}

			CommonError IMsg::send(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					const int dataSize{ static_cast<const int>(msgs.size()) };

					for (int i = 0; i != dataSize; ++i)
					{
						const std::string data{ msgs[i] };
						const size_t datalen{ data.length() };
						zmq_msg_t msg;

						if (-1 < zmq_msg_init_size(&msg, datalen))
						{
#ifdef WINDOWS
							memcpy_s(zmq_msg_data(&msg), datalen, data.c_str(), datalen);
#else
							memcpy(zmq_msg_data(&msg), data.c_str(), datalen);
#endif//WINDOWS
							zmq_msg_send(&msg, s, i < dataSize - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT);
						}
					}
				}

				return e;
			}

			Msg::Msg() : msg{boost::factory<IMsg*>()()}
			{}
			Msg::~Msg() 
			{
				boost::checked_delete(msg);	
			}

			int Msg::add(const std::string data)
			{
				return static_cast<int>(msg ? msg->add(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			const std::string Msg::remove()
			{
				return msg ? msg->remove() : "";
			}

			int Msg::receive(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = (msg ? msg->receive(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				return static_cast<int>(e);
			}

			int Msg::send(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = (msg ? msg->send(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}

				return static_cast<int>(e);
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework
