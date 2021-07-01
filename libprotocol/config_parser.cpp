#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "libcommon/error.h"
#include "url.h"

namespace framework
{
    namespace liburl
    {
        class IUrlParser
        {
        public:
            IUrlParser(void);
            ~IUrlParser(void);

        public:
            CommonError parse(const std::string url);
            const std::string compose(void);
            const std::string getCommand(void);
            CommonError setCommand(const std::string command);
            const std::vector<ParamItem> getCommandParameters(void);
            CommonError setCommandParameter(const std::string key, const std::string value);
            const std::vector<ParamItem> getUserParameters(void);
            CommonError setUserParameter(const std::string key, const std::string value);

        private:
            void parseCommandParameters(const std::string str);
            void parseUserParameters(const std::string str);

        private:
            std::string command;
            std::vector<ParamItem> commandParameters;
            std::vector<ParamItem> userParameters;
        };//class IUrlParser

        IUrlParser::IUrlParser()
        {}
        IUrlParser::~IUrlParser()
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

        const std::string IUrlParser::getCommand()
        {
            return command;
        }

        CommonError IUrlParser::setCommand(const std::string command)
        {
            this->command = command;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const std::vector<ParamItem> IUrlParser::getCommandParameters()
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

        UrlParser::UrlParser() : parser{boost::factory<IUrlParser*>()()}
        {}
        UrlParser::~UrlParser()
        {
            boost::checked_delete(parser);
        }

        int UrlParser::parse(const std::string url)
        {
            CommonError e{
                !url.empty() ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->parse(url) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::string UrlParser::compose()
        {
            return parser ? parser->compose() : "";
        }

        const std::string UrlParser::getCommand()
        {
            return parser ? parser->getCommand() : "";
        }

        int UrlParser::setCommand(const std::string command)
        {
            CommonError e{
                !command.empty() ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setCommand(command) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::vector<ParamItem> UrlParser::getCommandParameters()
        {
            return parser ? parser->getCommandParameters() : std::vector<ParamItem>();
        }

        int UrlParser::setCommandParameter(const std::string key, const std::string value)
        {
            CommonError e{
                !key.empty() && !value.empty() ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setCommandParameter(key, value) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::vector<ParamItem> UrlParser::getUserParameters()
        {
            return parser ? parser->getUserParameters() : std::vector<ParamItem>();
        }

        int UrlParser::setUserParameter(const std::string key, const std::string value)
        {
            CommonError e{
                !key.empty() && !value.empty() ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setUserParameter(key, value) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }
    }//namespace liburl
}//namespace framework