#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "detail/register_query.pb.h"
#include "libcommon/error.h"
#include "register_query_parser.h"

namespace framework
{
    namespace libprotocol
    {
        class IRegisterQueryParser
        {
        public:
            IRegisterQueryParser(void);
            ~IRegisterQueryParser(void);

        public:
            CommonError parse(const std::string data);
            const std::string compose(void);
            const RegisterQueryType getCommandType(void) const;
            CommonError setCommandType(
                const RegisterQueryType type = RegisterQueryType::REGISTER_QUERY_TYPE_NONE);
            const ApplicationInfo getRegisterApplicationInfo(void) const;
            CommonError setRegisterApplicationInfo(const ApplicationInfo info);
            const std::vector<ApplicationInfo> getQueryApplicationInfos(void) const;
            CommonError setQueryApplicationInfos(const std::vector<ApplicationInfo> infos);
            const int getStatusCode(void) const;
            CommonError setStatusCode(const int status = -1);

        private:
            RegisterQueryType registerQueryType;
            std::vector<ApplicationInfo> applicationInfos;
            int statusCode;
        };//class IRegisterQueryParser

        IRegisterQueryParser::IRegisterQueryParser() 
            : registerQueryType{RegisterQueryType::REGISTER_QUERY_TYPE_NONE}, statusCode{-1}
        {}
        IRegisterQueryParser::~IRegisterQueryParser()
        {}

        CommonError IRegisterQueryParser::parse(const std::string data)
        {
            CommonError e{CommonError::COMMON_ERROR_BAD_OPERATE};
            msg::ApplicationMessage msg;

            if (msg.ParseFromArray(data.c_str(), static_cast<int>(data.length())))
            {
                registerQueryType = static_cast<RegisterQueryType>(msg.type());
                for (int i = 0; i != msg.infos_size(); ++i)
                {
                    msg::ApplicationInfo info{msg.infos(i)};
                    ApplicationInfo newApplicationInfo;
                    newApplicationInfo.type = static_cast<ApplicationType>(info.type());
                    newApplicationInfo.name = info.name();
                    newApplicationInfo.IPv4 = info.ipv4();
                    newApplicationInfo.ID = info.id();
                    newApplicationInfo.nickName = info.nickname();
                    applicationInfos.push_back(newApplicationInfo);
                }
                if(msg.has_status())
                {
                    statusCode = msg.status();
                }

                e = CommonError::COMMON_ERROR_SUCCESS;
            }
            
            return e;
        }

        const std::string IRegisterQueryParser::compose()
        {
            std::string data;
            msg::ApplicationMessage msg;

            msg.set_type(
                static_cast<msg::ApplicationMessage_CommandType>(registerQueryType));
            for (int i = 0; i != applicationInfos.size(); ++i)
            {
                msg::ApplicationInfo* info{msg.add_infos()};
                info->set_type(
                    static_cast<msg::ApplicationInfo_ApplicationType>(applicationInfos[i].type));
                info->set_name(applicationInfos[i].name);
                info->set_ipv4(applicationInfos[i].IPv4);
                info->set_id(applicationInfos[i].ID);
                info->set_nickname(applicationInfos[i].nickName);
            }
            if (-1 < statusCode)
            {
                msg.set_status(statusCode);
            }
            msg.SerializeToString(&data);

            return data;
        }

        const RegisterQueryType IRegisterQueryParser::getCommandType() const
        {
            return registerQueryType;
        }

        CommonError IRegisterQueryParser::setCommandType(
            const RegisterQueryType type /*= RegisterQueryType::REGISTER_QUERY_TYPE_NONE*/)
        {
            registerQueryType = type;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const ApplicationInfo IRegisterQueryParser::getRegisterApplicationInfo() const
        {
            ApplicationInfo info{
                ApplicationType::APPLICATION_TYPE_NONE};

            if ((RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REQ == registerQueryType ||
                RegisterQueryType::REGISTER_QUERY_TYPE_REGISTER_REP == registerQueryType) &&
                0 < applicationInfos.size())
            {
                info = applicationInfos[0];
            }
            
            return info;
        }

        CommonError IRegisterQueryParser::setRegisterApplicationInfo(const ApplicationInfo info)
        {
            if (0 < applicationInfos.size())
            {
                applicationInfos[0] = info;
            }
            else
            {
                applicationInfos.push_back(info);
            }
            
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const std::vector<ApplicationInfo> IRegisterQueryParser::getQueryApplicationInfos() const
        {
            return applicationInfos;
        }

        CommonError IRegisterQueryParser::setQueryApplicationInfos(const std::vector<ApplicationInfo> infos)
        {
            applicationInfos = infos;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const int IRegisterQueryParser::getStatusCode() const
        {
            return statusCode;
        }

        CommonError IRegisterQueryParser::setStatusCode(const int status /*= -1*/)
        {
            statusCode = status;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        RegisterQueryParser::RegisterQueryParser() 
            : parser{boost::factory<IRegisterQueryParser*>()()}
        {}
        RegisterQueryParser::~RegisterQueryParser()
        {
            boost::checked_delete(parser);
        }

        int RegisterQueryParser::parse(const std::string data)
        {
            CommonError e{
                !data.empty() ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->parse(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::string RegisterQueryParser::compose()
        {
            return parser ? parser->compose() : "";
        }

        const RegisterQueryType RegisterQueryParser::getCommandType() const
        {
            return parser ? parser->getCommandType() : RegisterQueryType::REGISTER_QUERY_TYPE_NONE;
        }

        int RegisterQueryParser::setCommandType(
            const RegisterQueryType type /*= RegisterQueryType::REGISTER_QUERY_TYPE_NONE*/)
        {
            CommonError e{
                RegisterQueryType::REGISTER_QUERY_TYPE_NONE < type ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setCommandType(type) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const ApplicationInfo RegisterQueryParser::getRegisterApplicationInfo() const
        {
            ApplicationInfo nullApplicationInfo{
                ApplicationType::APPLICATION_TYPE_NONE};
            return parser ? parser->getRegisterApplicationInfo() : nullApplicationInfo;
        }

        int RegisterQueryParser::setRegisterApplicationInfo(const ApplicationInfo info)
        {
            CommonError e{
                ApplicationType::APPLICATION_TYPE_NONE < info.type ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setRegisterApplicationInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::vector<ApplicationInfo> RegisterQueryParser::getQueryApplicationInfos(void) const
        {
            std::vector<ApplicationInfo> nullApplicationInfos;
            return parser ? parser->getQueryApplicationInfos() : nullApplicationInfos;
        }

        int RegisterQueryParser::setQueryApplicationInfos(const std::vector<ApplicationInfo> infos)
        {
            CommonError e{
                0 < infos.size() ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setQueryApplicationInfos(infos) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const int RegisterQueryParser::getStatusCode() const
        {
            return parser ? parser->getStatusCode() : -1;
        }

        int RegisterQueryParser::setStatusCode(const int status /*= -1*/)
        {
            return static_cast<int>(parser ? parser->setStatusCode(status) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }
    }//namespace libprotocol
}//namespace framework