//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-26
//		Description :					共享互斥量类
//
//		History:						Author									Date										Description
//										王科威									2020-04-26									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_SHARED_MUTEX_H
#define FRAMEWORK_LIBCOMMON_SHARED_MUTEX_H

namespace framework
{
    namespace libcommon
    {
        class SharedMutex
        {
        public:
            SharedMutex(void);
            virtual ~SharedMutex(void);

        public:
            //打开共享读锁
            void lockRead(void);

            //关闭共享读锁
            void unlockRead(void);
            
            //打开共享写锁
            //@Return : 打开状态
            bool lockWrite(void);

            //关闭共享写锁
            void unlockWrite(void);
        };//class SharedMutex
    }//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_SHARED_MUTEX_H

