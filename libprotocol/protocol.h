//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-23
//		Description :					URL解析类
//                                      1.该类支持的完整URL格式如下：
//                                      --------------------------------------------------------------------------------
//                                      | command://sender=A&receiver=B&data=C?via=X&via=Y&...&key1=value1&key2=value2 |
//                                      --------------------------------------------------------------------------------
//                                      2.分段说明
//                                      2.1命令段
//                                          命令段为command，command则表示register、query、configure、notification和syn等具体命令名称。
//                                      2.2命令参数段
//                                          命令参数段为sender=A&receiver=B&data=C，可由用户自定义命令参数，但必须遵守key-value规则且key不可重复出现。各参数间用“&”标识符分隔。
//                                      2.3自定义参数段
//                                          自定义参数段为via=X&via=Y&...&key1=value1&key2=value2，可由用户自定义命令参数，但必须遵守key-value规则且key可重复出现。各参数间用“&”标识符分隔。
//
//		History:						Author									Date										Description
//										王科威									 2021-06-23									 创建
//

#ifndef FRAMEWORK_LIBURL_URL_PARSER_H
#define FRAMEWORK_LIBURL_URL_PARSER_H

#include <string>
#include <vector>

namespace framework
{
    namespace liburl
    {
        typedef struct tagParamItem_t
        {
            std::string key;
            std::string value;
        }ParamItem;
        
        class IUrlParser;

        class UrlParser
        {
        public:
            UrlParser(void);
            ~UrlParser(void);

        public:
            //解析URL
			//@url : 数据
            //@Return : 错误码
            int parse(const std::string url);

            //组合URL
			//@url : 数据
            //@Return : 错误码
            const std::string compose(void);

            //获取命令段
            //@Return ：命令段
            const std::string getCommand(void);

            //设置命令段
            //@command : 命名名称
            //@Return ：错误码
            int setCommand(const std::string command);

            //获取命令参数段
            //@Return : 命令参数段
            const std::vector<ParamItem> getCommandParameters(void);

            //设置命令段
            //@key : 键
            //@value : 值
            //@Return ：错误码
            int setCommandParameter(const std::string key, const std::string value);

            //获取自定义参数段
            //@Return : 自定义参数段
            const std::vector<ParamItem> getUserParameters(void);

            //设置自定义参数段
            //@key : 键
            //@value : 值
            //@Return ：错误码
            int setUserParameter(const std::string key, const std::string value);

        private:
            IUrlParser* parser;
        };//class UrlParser
    }//namespace liburl
}//namespace framework

#endif//FRAMEWORK_LIBURL_URL_PARSER_H