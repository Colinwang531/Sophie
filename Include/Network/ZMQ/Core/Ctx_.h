//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-25
//		Description :					Context类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									创建
//

#ifndef FRAMEWORK_NETWORK_ZMQ_CONTEXT_H
#define FRAMEWORK_NETWORK_ZMQ_CONTEXT_H

#include "boost/serialization/singleton.hpp"

namespace framework
{
    namespace network
    {
		namespace zeromq
		{
			class Ctx :
				public boost::serialization::singleton<Ctx>
			{
			public:
				Ctx(void);
				virtual ~Ctx(void);

			public:
				//初始化/释放Context实例
				//@type : Socket类型
				//@Return : Socket实例
				int init(void);
				int uninit(void);

				//创建Socket
				//@type : Socket类型
				//@Return : Socket实例
				void* createNewSocket(const int type = -1);

				//销毁Socket
				//@Return : 错误码
				int destroySocket(void* s = nullptr);

			private:
				void* ctx;
			};//class Ctx
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_CONTEXT_H
