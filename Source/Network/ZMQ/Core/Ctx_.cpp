#include "boost/checked_delete.hpp"
#include "ctx.hpp"
#include "socket_base.hpp"
#include "Error.h"
#include "Hardware/Cpu.h"
using CPU = framework::hardware::Cpu;
#include "Network/ZMQ/Core/Ctx_.h"

namespace framework
{
    namespace network
    {
        namespace zeromq
        {
			Ctx::Ctx() : ctx{ nullptr }
			{}
			Ctx::~Ctx()
			{
				uninit();
			}

			int Ctx::init()
			{
				if (!ctx)
				{
					ctx = new(std::nothrow) zmq::ctx_t;

					if (ctx)
					{
						const int cores{ CPU().getCPUCoreNumber() };
						//不关心IO线程设置结果
						//1 == ZMQ_IO_THREADS,不直接使用宏定义避免包含zmq.h头文件
						zmq::ctx_t* c{ reinterpret_cast<zmq::ctx_t*>(ctx) };
						c->set(1, &cores, sizeof(const int));
					}
				}
				
				return ctx ? eSuccess : eBadNewObject;
			}

			int Ctx::uninit()
			{
				if (ctx)
				{
					zmq::ctx_t* c{ reinterpret_cast<zmq::ctx_t*>(ctx) };
					c->terminate();
					boost::checked_delete(c);
					ctx = nullptr;
				}

				return ctx ? eBadOperate : eSuccess;
			}

			void* Ctx::createNewSocket(const int type /* = -1 */)
			{
				void* s{ nullptr };

				if (ctx && -1 < type)
				{
					zmq::ctx_t* c{ reinterpret_cast<zmq::ctx_t*>(ctx) };
					s = c->create_socket(type);
				}

				return s;
			}

			int Ctx::destroySocket(void* s /* = nullptr */)
			{
				int e{ ctx && s ? eSuccess : eBadOperate };

				if (eSuccess == e)
				{
					zmq::socket_base_t* s_{ reinterpret_cast<zmq::socket_base_t*>(s) };
					s_->close();

					zmq::ctx_t* c{ reinterpret_cast<zmq::ctx_t*>(ctx) };
					c->destroy_socket(s_);
				}

				return e;
			}
        }//namespace zeromq
    }//namespace network
}//namespace framework

