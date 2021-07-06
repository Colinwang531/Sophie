#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "rwlock.h"

namespace framework
{
    namespace libcommon
    {
        class IWRLock
        {
        public:
            IWRLock(void);
            ~IWRLock(void);

        public:
            void rlock(void);
            void unrlock(void);
            bool wlock(void);
            void unwlock(void);

        private:
            boost::shared_mutex mtx;
        };//IWRLock

        IWRLock::IWRLock()
        {}
        IWRLock::~IWRLock()
        {}

        void IWRLock::rlock()
        {
            mtx.lock_shared();
        }

        void IWRLock::unrlock()
        {
            mtx.unlock_shared();
        }

        bool IWRLock::wlock()
        {
            return mtx.try_lock_upgrade();
        }

        void IWRLock::unwlock()
        {
            mtx.unlock_upgrade();
        }

        WRLock::WRLock() : wrlock{boost::factory<IWRLock*>()()}
        {}
        WRLock::~WRLock()
        {
            boost::checked_delete(wrlock);
        }

        void WRLock::rlock()
        {
            if (wrlock)
            {
                wrlock->rlock();
            }
        }

        void WRLock::unrlock()
        {
            if (wrlock)
            {
                wrlock->unrlock();
            }
        }

        bool WRLock::wlock()
        {
            return wrlock ? wrlock->wlock() : false;
        }

        void WRLock::unwlock()
        {
            if (wrlock)
            {
                wrlock->unwlock();
            }
        }
    }//namespace libcommon
}//namespace framework
