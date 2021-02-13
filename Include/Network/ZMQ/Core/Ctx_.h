//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-25
//		Description :					Context��
//
//		History:						Author									Date										Description
//										������									2020-04-26									����
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
				//��ʼ��/�ͷ�Contextʵ��
				//@type : Socket����
				//@Return : Socketʵ��
				int init(void);
				int uninit(void);

				//����Socket
				//@type : Socket����
				//@Return : Socketʵ��
				void* createNewSocket(const int type = -1);

				//����Socket
				//@Return : ������
				int destroySocket(void* s = nullptr);

			private:
				void* ctx;
			};//class Ctx
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ZMQ_CONTEXT_H
