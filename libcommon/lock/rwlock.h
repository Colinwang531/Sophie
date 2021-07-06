//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-07-05
//		Description :					读写类
//
//		History:						Author									Date										Description
//										王科威									 2021-07-05									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_RW_LOCK_H
#define FRAMEWORK_LIBCOMMON_RW_LOCK_H

namespace framework
{
    namespace libcommon
    {
        class IWRLock;
        
        class WRLock
        {
        public:
            WRLock(void);
            ~WRLock(void);

        public:
            //打开共享读锁
            void rlock(void);

            //关闭共享读锁
            void unrlock(void);
            
            //打开共享写锁
            //@Return : 打开状态
            bool wlock(void);

            //关闭共享写锁
            void unwlock(void);

        private:
            IWRLock* wrlock;
        };//class WRLock
    }//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_RW_LOCK_H

