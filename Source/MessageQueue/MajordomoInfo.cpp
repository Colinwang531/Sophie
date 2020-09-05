#include "Time/XTime.h"
using Time = base::time::Time;
#include "MessageQueue/MajordomoInfo.h"

namespace mq
{
    namespace module
    {
        MDPWorker::MDPWorker(
            const std::string id, MDPServicePtr service)
            : workerID{ id }, mdpService{ service }, expiryTime{ Time().tickcount() }
        {}

        MDPWorker::~MDPWorker()
        {}

        MDPService::MDPService(const std::string name)
            : serviceName{ name }
        {}

        MDPService::~MDPService()
        {
            workerGroup.clear();
        }

        void MDPService::addWorker(MDPWorkerPtr worker)
        {
            if (worker)
            {
                workerGroup.push_back(worker);
            }
        }
    }//namespace module
}//namespace mq

