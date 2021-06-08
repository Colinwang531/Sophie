#include <vector>
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
			class Impl
			{
			public:
				Impl(void);
				~Impl(void);

			public:
				void add(const std::string msg);
				const std::string remove(void);
				int receive(void* s = nullptr);
				int send(void* s = nullptr);

			private:
				std::vector<std::string> datas;
			};//class Impl

			Impl::Impl()
			{}
			Impl::~Impl()
			{}

			void Impl::add(const std::string msg)
			{
				datas.push_back(msg);
			}

			const std::string Impl::remove()
			{
				std::string msg;

				if (!datas.empty())
				{
					msg = datas[0];
					datas.erase(datas.begin());
				}

				return msg;
			}

			int Impl::receive(void* s /* = nullptr */)
			{
				CommonError e{ 
					s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					zmq_msg_t msg_;
					zmq_msg_init(&msg_);

					while (-1 < zmq_msg_recv(&msg_, ZMQ_DONTWAIT))
					{
						const std::string data{ 
							static_cast<const char*>(msg_.data()), msg_.size() };
						datas.push_back(data);

						if (0 == zmq_msg_more(&msg_))
						{
							break;
						}
					}
				}

				return (int)e;
			}

			int Impl::send(void* s /* = nullptr */)
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
						zmq::msg_t msg_;

						if (-1 < zmq_msg_init_size(&msg_, datalen))
						{
#ifdef WINDOWS
							memcpy_s(msg_.data(), datalen, data.c_str(), datalen);
#else
							memcpy(msg_.data(), data.c_str(), datalen);
#endif//WINDOWS
							zmq_msg_send(&msg_, s, i < dataSize - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT);
						}
					}
				}

				return (int)e;
			}

			Msg::Msg() : impl{new(std::nothrow) Impl}
			{}
			Msg::~Msg() 
			{
				boost::checked_delete(impl);	
			}

			void Msg::add(const std::string msg)
			{
				if (impl)
				{
					impl->add(msg);
				}
			}

			const std::string Msg::remove()
			{
				return impl ? impl->remove() : "";
			}

			int Msg::receive(void* s /* = nullptr */)
			{
				return impl ? impl->receive(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}

			int Msg::send(void* s /* = nullptr */)
			{
				return impl ? impl->send(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework
