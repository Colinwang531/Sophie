#include <vector>
#include <cstring>
#include "boost/checked_delete.hpp"
#include "zmq.h"
#include "error.h"
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
				void add(const std::string msg);
				const std::string remove(void);
				CommonError receive(void* s = nullptr);
				CommonError send(void* s = nullptr);

			private:
				std::vector<std::string> datas;
			};//class IMsg

			IMsg::IMsg()
			{}
			IMsg::~IMsg()
			{}

			void IMsg::add(const std::string msg)
			{
				datas.push_back(msg);
			}

			const std::string IMsg::remove()
			{
				std::string msg;

				if (!datas.empty())
				{
					msg = datas[0];
					datas.erase(datas.begin());
				}

				return msg;
			}

			CommonError IMsg::receive(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					zmq_msg_t msg_;
					zmq_msg_init(&msg_);

					while (-1 < zmq_msg_recv(&msg_, s, ZMQ_DONTWAIT))
					{
						const std::string data{ 
							static_cast<const char*>(zmq_msg_data(&msg_)), zmq_msg_size(&msg_) };
						datas.push_back(data);

						if (0 == zmq_msg_more(&msg_))
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
					const int dataSize{ (const int)datas.size() };

					for (int i = 0; i != dataSize; ++i)
					{
						const std::string data{ datas[i] };
						const size_t datalen{ data.length() };
						zmq_msg_t msg_;

						if (-1 < zmq_msg_init_size(&msg_, datalen))
						{
#ifdef WINDOWS
							memcpy_s(zmq_msg_data(&msg_), datalen, data.c_str(), datalen);
#else
							memcpy(zmq_msg_data(&msg_), data.c_str(), datalen);
#endif//WINDOWS
							zmq_msg_send(&msg_, s, i < dataSize - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT);
						}
					}
				}

				return e;
			}

			Msg::Msg() : msg{new(std::nothrow) IMsg}
			{}
			Msg::~Msg() 
			{
				boost::checked_delete(msg);	
			}

			void Msg::add(const std::string data)
			{
				if (msg)
				{
					msg->add(data);
				}
			}

			const std::string Msg::remove()
			{
				return msg ? msg->remove() : "";
			}

			int Msg::receive(void* s /* = nullptr */)
			{
				return static_cast<int>(msg ? msg->receive(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Msg::send(void* s /* = nullptr */)
			{
				return static_cast<int>(msg ? msg->send(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework
