#include "boost/checked_delete.hpp"
#include "zmq.h"
#include "error.h"
#include "hardware/cpu/cpu.h"
using Cpu = framework::libcommon::hardware::Cpu;
#include "ctx.h"

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
				int init(void);
				int uninit(void);
				inline const bool valid(void) const
				{
					return ctx ? true : false;
				}
				void* createNewSocket(const int type = -1);
				int destroySocket(void* s = nullptr);

			private:
				void* ctx;
			};//class Impl

			Impl::Impl() : ctx{nullptr}
			{}
			Impl::~Impl()
			{
				uninit();
			}

			int Impl::init()
			{
				if (!ctx)
				{
					ctx = zmq_ctx_new();

					if (ctx)
					{
						int cores{ Cpu().getCoreNumber() };
						zmq_ctx_set(ctx, ZMQ_IO_THREADS, cores);
					}
				}
				
				return (int)(ctx ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Impl::uninit()
			{
				if (ctx)
				{
					zmq_term(ctx);
					ctx = nullptr;
				}

				return (int)(ctx ? CommonError::COMMON_ERROR_BAD_OPERATE : CommonError::COMMON_ERROR_SUCCESS);
			}

			void* Impl::createNewSocket(const int type /* = -1 */)
			{
				void* s{ nullptr };

				if (ctx && ZMQ_PAIR <= type && ZMQ_STREAM >= type)
				{
					s = zmq_socket(ctx, type);
				}

				return s;
			}

			int Impl::destroySocket(void* s /* = nullptr */)
			{
				CommonError e{ 
					ctx && s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					zmq_close(s);
				}

				return (int)e;
			}

			Ctx::Ctx() : impl{ new(std::nothrow) Impl }
			{}
			Ctx::~Ctx()
			{
				boost::checked_delete(impl);
			}

			int Ctx::init()
			{
				return impl ? impl->init() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}

			int Ctx::uninit()
			{
				return impl ? impl->uninit() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}

			void* Ctx::createNewSocket(const int type /* = -1 */)
			{
				return impl ? impl->createNewSocket(type) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}

			int Ctx::destroySocket(void* s /* = nullptr */)
			{
				return impl ? impl->destroySocket(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

