#include "boost/functional/factory.hpp"
#include "boost/checked_delete.hpp"
#include "zmq.h"
#include "libcommon/error.h"
#include "libcommon/hardware/cpu/cpu.h"
using Cpu = framework::libcommon::hardware::Cpu;
#include "ctx.h"

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class ICtx
			{
			public:
				ICtx(void);
				~ICtx(void);

			public:
				CommonError init(void);
				CommonError uninit(void);
				inline const bool valid(void) const
				{
					return ctx ? true : false;
				}
				void* createNewSocket(const int type = -1);
				CommonError destroySocket(void* s = nullptr);

			private:
				void* ctx;
			};//class ICtx

			ICtx::ICtx() : ctx{nullptr}
			{}
			ICtx::~ICtx()
			{
				uninit();
			}

			CommonError ICtx::init()
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
				
				return ctx ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
			}

			CommonError ICtx::uninit()
			{
				if (ctx)
				{
					zmq_term(ctx);
					ctx = nullptr;
				}

				return ctx ? CommonError::COMMON_ERROR_BAD_OPERATE : CommonError::COMMON_ERROR_SUCCESS;
			}

			void* ICtx::createNewSocket(const int type /* = -1 */)
			{
				void* s{ nullptr };

				if (ctx && ZMQ_PAIR <= type && ZMQ_STREAM >= type)
				{
					s = zmq_socket(ctx, type);
				}

				return s;
			}

			CommonError ICtx::destroySocket(void* s /* = nullptr */)
			{
				CommonError e{ 
					ctx && s ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_BAD_OPERATE };

				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					zmq_close(s);
				}

				return e;
			}

			Ctx::Ctx() : ctx{boost::factory<ICtx*>()()}
			{}
			Ctx::~Ctx()
			{
				boost::checked_delete(ctx);
			}

			int Ctx::init()
			{
				return static_cast<int>(ctx ? ctx->init() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			int Ctx::uninit()
			{
				return static_cast<int>(ctx ? ctx->uninit() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}

			const bool Ctx::valid() const
			{
				return ctx ? ctx->valid() : false;
			}

			void* Ctx::createNewSocket(const int type /* = -1 */)
			{
				return ctx ? ctx->createNewSocket(type) : nullptr;
			}

			int Ctx::destroySocket(void* s /* = nullptr */)
			{
				return static_cast<int>(ctx ? ctx->destroySocket(s) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

