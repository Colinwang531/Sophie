#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
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
            int setCommandType(
                const RegisterQueryType type = RegisterQueryType::REGISTER_QUERY_TYPE_NONE);
            const ApplicationInfo getRegisterApplicationInfo(void) const;
            int setRegisterApplicationInfo(const ApplicationInfo info);
            const int getQueryStatusCode(void) const;
            int setQueryStatusCode(const int status = -1);
            const std::vector<ApplicationInfo> getQueryApplicationInfos(void) const;
            int setQueryApplicationInfos(const std::vector<ApplicationInfo> infos);

        private:
            RegisterQueryType registerQueryType;
        };//class IRegisterQueryParser

        IRegisterQueryParser::IRegisterQueryParser() 
            : registerQueryType{RegisterQueryType::REGISTER_QUERY_TYPE_NONE}
        {}
        IRegisterQueryParser::~IRegisterQueryParser()
        {}

        CommonError IUrlParser::parse(const std::string url)
        {
            const int colonPos{
                static_cast<const int>(url.find_first_of(':'))};
            const int questionPos{
                static_cast<const int>(url.find_first_of('?'))};
            const int totalLength{
                static_cast<const int>(url.length())};

            //命令名称
            command = url.substr(0, colonPos);
            //命令参数段
	        const std::string commandParam{ 
                url.substr(colonPos + 3, -1 < questionPos ? questionPos - colonPos - 3 : totalLength - colonPos - 2) };
            if (!commandParam.empty())
            {
                parseCommandParameters(commandParam);
            }
            //自定义参数段
	        const std::string userParam{ 
                -1 < questionPos ? url.substr(questionPos + 1, totalLength - questionPos) : "" };
            if (!userParam.empty())
            {
                parseUserParameters(userParam);
            }
            
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const std::string IUrlParser::compose()
        {
            std::string url;
            const int cmdParaCount{
                static_cast<const int>(commandParameters.size())};
            const int userParaCount{
                static_cast<const int>(userParameters.size())};

            url.append(command);
            url.append("://");
            for (int i = 0; i != cmdParaCount; ++i)
            {
                url.append(commandParameters[i].key);
                url.append("=");
                url.append(commandParameters[i].value);

                if (i < cmdParaCount - 1)
                {
                    url.append("&");
                } 
            }
            for (int i = 0; i != userParaCount; ++i)
            {
                if (0 == i)
                {
                    url.append("?");
                }
                
                url.append(userParameters[i].key);
                url.append("=");
                url.append(userParameters[i].value);

                if (i < userParaCount - 1)
                {
                    url.append("&");
                } 
            }

            return url;
        }

        const RegisterQueryType IRegisterQueryParser::getCommandType() const
        {
            return registerQueryType;
        }

        CommonError RegisterQueryType::setCommandType(
            const RegisterQueryType type /*= RegisterQueryType::REGISTER_QUERY_TYPE_NONE*/)
        {
            registerQueryType = type;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const ApplicationInfo RegisterQueryType::getRegisterApplicationInfo()
        {
            return commandParameters;
        }

        CommonError IUrlParser::setCommandParameter(const std::string key, const std::string value)
        {
            ParamItem pi{key, value};
            commandParameters.push_back(pi);
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const std::vector<ParamItem> IUrlParser::getUserParameters()
        {
            return userParameters;
        }

        CommonError IUrlParser::setUserParameter(const std::string key, const std::string value)
        {
            ParamItem pi{key, value};
            userParameters.push_back(pi);
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        void IUrlParser::parseCommandParameters(const std::string str)
        {
            if (!str.empty())
            {
                std::vector<std::string> items;
                boost::split(items, str, boost::is_any_of("&"));

                for(int i = 0; i !=items.size(); ++i)
                {
                    std::vector<std::string> subitems;
                    boost::split(subitems, items[i], boost::is_any_of("="));

                    ParamItem pi{subitems[0], subitems[1]};
                    commandParameters.push_back(pi);
                }
            }
        }

        void IUrlParser::parseUserParameters(const std::string str)
        {
            if (!str.empty())
            {
                std::vector<std::string> items;
                boost::split(items, str, boost::is_any_of("&"));

                for(int i = 0; i !=items.size(); ++i)
                {
                    std::vector<std::string> subitems;
                    boost::split(subitems, items[i], boost::is_any_of("="));

                    ParamItem pi{subitems[0], subitems[1]};
                    userParameters.push_back(pi);
                }
            }
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
            return parser ? parser->getCommandType() : "";
        }

        int RegisterQueryParser::setCommandType(
            const RegisterQueryType type /*= RegisterQueryType::REGISTER_QUERY_TYPE_NONE*/)
        {
            CommonError e{
                REGISTER_QUERY_TYPE_NONE < type ? 
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

        const int RegisterQueryParser::getQueryStatusCode()
        {
            return parser ? parser->getQueryStatusCode() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
        }

        int RegisterQueryParser::setQueryStatusCode(const int status /*= -1*/)
        {
            return static_cast<int>(parser ? parser->setQueryStatusCode(status) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
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
    }//namespace libprotocol
}//namespace framework