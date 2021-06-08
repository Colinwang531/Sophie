#include <cstdarg>
#include <stdio.h>
#include "glog/logging.h"
#include "log.h"

namespace framework
{
    namespace liblog
    {
        static bool initSuccess{false};

        Log::Log()
        {}
        Log::~Log()
        {}

        int Log::init(
            const char* name/* = nullptr*/,
            const char* dir/* = nullptr*/)
        {
            LogError e{name && dir ? LogError::LOG_ERROR_SUCCESS : LogError::LOG_ERROR_INVALID_PARAMETER};

            if (LogError::LOG_ERROR_SUCCESS == e)
            {
                FLAGS_stderrthreshold = google::INFO;
                FLAGS_colorlogtostderr = 1;
                google::SetLogDestination(google::INFO, dir);
                google::InitGoogleLogging(name);
                initSuccess = true;
            }

            return static_cast<int>(e);
        }

        void Log::uninit()
        {
            google::ShutdownGoogleLogging();
            initSuccess = false;
        }

        int Log::write(
            const LogLevel level/* = LogLevel::LOG_LEVEL_INFO*/,
            const char* format/* = nullptr*/,
            ...)
        {
            LogError e{LogError::LOG_ERROR_INIT_FAILED};

            if (initSuccess)
            {
                char text[4096]{0};

	            va_list list;
	            va_start(list, format);
	            vsnprintf(text, 4096, format, list);
                va_end(list);

                if (LogLevel::LOG_LEVEL_INFO == level)
                {
                    LOG(INFO) << text;
                }
                else if (LogLevel::LOG_LEVEL_WARNING == level)
                {
                    LOG(WARNING) << text;
                }
                else if (LogLevel::LOG_LEVEL_ERROR == level)
                {
                    LOG(ERROR) << text;
                }
                
                e = LogError::LOG_ERROR_SUCCESS;
            }

            return static_cast<int>(e);
        }
    }//namespace liblog
}//namespace framework