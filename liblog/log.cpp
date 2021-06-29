#include <cstdarg>
#include <stdio.h>
#include "glog/logging.h"
#include "libcommon/error.h"
#include "libcommon/path/path.h"
using ProgramPathParser = framework::libcommon::ProgramPathParser;
#include "log.h"

namespace framework
{
    namespace liblog
    {
        Log::Log()
        {}
        Log::~Log()
        {}

        int Log::init(
            const char* name/* = nullptr*/)
        {
            CommonError e{
                name ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};
            ProgramPathParser parser;
            const std::string dir{parser.parseFileDirectory(name) + "/log"}, file{parser.parseFileName(name)};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                FLAGS_stderrthreshold = google::INFO;
                FLAGS_colorlogtostderr = 1;
                google::SetLogDestination(google::INFO, dir.c_str());
                google::InitGoogleLogging(file.c_str());
            }

            return static_cast<int>(e);
        }

        void Log::uninit()
        {
            google::ShutdownGoogleLogging();
        }

        int Log::write(
            const LogLevel level/* = LogLevel::LOG_LEVEL_INFO*/,
            const char* format/* = nullptr*/,
            ...)
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

            return static_cast<int>(CommonError::COMMON_ERROR_SUCCESS);
        }
    }//namespace liblog
}//namespace framework