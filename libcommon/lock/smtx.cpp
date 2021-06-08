#include "boost/thread/shared_mutex.hpp"
#include "smtx.h"

namespace framework
{
    namespace libcommon
    {
        using Mutex = boost::shared_mutex;
        Mutex mtx;

        SharedMutex::SharedMutex()
        {}
        SharedMutex::~SharedMutex()
        {}

        void SharedMutex::lockRead()
        {
            mtx.lock_shared();
        }

        void SharedMutex::unlockRead()
        {
            mtx.unlock_shared();
        }

        bool SharedMutex::lockWrite()
        {
            return mtx.try_lock_upgrade();
        }

        void SharedMutex::unlockWrite()
        {
            mtx.unlock_upgrade();
        }
    }//namespace libcommon
}//namespace framework
