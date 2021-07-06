//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					注册-查询解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_REGISTER_QUERY_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_REGISTER_QUERY_PARSER_H

#include "defs.h"

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagRegisterQueryType_t : int
        {
            REGISTER_QUERY_TYPE_NONE = 0,
            REGISTER_QUERY_TYPE_REGISTER_REQ,
            REGISTER_QUERY_TYPE_REGISTER_REP,
            REGISTER_QUERY_TYPE_QUERY_REQ,
            REGISTER_QUERY_TYPE_QUERY_REP
        }RegisterQueryType;

        class IRegisterQueryParser;

        class RegisterQueryParser
        {
        public:
            RegisterQueryParser(void);
            ~RegisterQueryParser(void);

        public:
            //解析
			//@data : 序列化数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 序列化数据
            const std::string compose(void);

            //获取类型
            //@Return ：类型
            const RegisterQueryType getCommandType(void) const;

            //设置类型
            //@type : 类型
            //@Return ：错误码
            int setCommandType(
                const RegisterQueryType type = RegisterQueryType::REGISTER_QUERY_TYPE_NONE);

            //获取注册应用程序信息
            //@Return ：应用程序信息
            const ApplicationInfo getRegisterApplicationInfo(void) const;

            //设置注册应用程序信息
            //@info : 应用程序信息
            //@Return ：错误码
            int setRegisterApplicationInfo(const ApplicationInfo info);

            //获取查询应用程序信息
            //@Return ：应用程序信息
            const std::vector<ApplicationInfo> getQueryApplicationInfos(void) const;

            //设置查询应用程序信息
            //@infos : 应用程序信息
            //@Return ：错误码
            int setQueryApplicationInfos(const std::vector<ApplicationInfo> infos);

            //获取注册/查询状态码
            //@Return ：注册/查询状态码
            const int getStatusCode(void) const;

            //设置注册/查询状态码
            //@status : 状态码
            //@Return ：错误码
            int setStatusCode(const int status = -1);

        private:
            IRegisterQueryParser* parser;
        };//class RegisterQueryParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_REGISTER_QUERY_PARSER_H