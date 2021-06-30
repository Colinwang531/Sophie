//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					注册解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_REGISTER_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_REGISTER_PARSER_H

#include <string>

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagCommandType_t : int
        {
            COMMAND_TYPE_NONE = 0,
            COMMAND_TYPE_REGISTER_REQ,
            COMMAND_TYPE_REGISTER_REP
        }CommandType;

        class IRegisterParser;

        class RegisterParser
        {
        public:
            RegisterParser(void);
            ~RegisterParser(void);

        public:
            //解析
			//@data : 数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 数据序列化字符串
            const std::string compose(void);

            //获取命令类型
            //@Return ：命令段
            const CommandType getCommandType(void);

            //设置命令类型
            //@type : 命令类型
            //@Return ：错误码
            int setCommandType(
                const CommandType type = CommandType::COMMAND_TYPE_NONE);

        private:
            IRegisterParser* parser;
        };//class RegisterParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_REGISTER_PARSER_H