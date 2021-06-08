//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-01
//		Description :					ZMQ上下文类
//
//		History:						Author									Date										Description
//										王科威									2021-06-01									创建
//

#ifndef FRAMEWORK_LIBNETWORK_ZMQ_CTX_H
#define FRAMEWORK_LIBNETWORK_ZMQ_CTX_H

namespace framework
{
    namespace libnetwork
    {
		namespace zmq
		{
			class Impl;

			class Ctx
			{
			public:
				Ctx(void);
				~Ctx(void);

			public:
				//初始化
				//@Return : 错误码
				int init(void);

				//释放
				//@Return : 错误码
				int uninit(void);

				//有效性验证
				inline const bool valid(void) const
				{
					return ctx ? true : false;
				}

				//创建ZMQ Socket实例
				//@type : Socket类型
				//@Return : Socket实例
				void* createNewSocket(const int type = -1);

				//销毁ZMQ Socket实例
				//@s : Socket实例
				//@Return : 错误码
				int destroySocket(void* s = nullptr);

			private:
				Impl* impl;
			};//class Ctx
		}//namespace zmq
    }//namespace libnetwork
}//namespace framework

#endif//FRAMEWORK_LIBNETWORK_ZMQ_CTX_H
