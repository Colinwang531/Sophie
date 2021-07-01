//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					配置解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_CONFIG_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_CONFIG_PARSER_H

#include "defs.h"

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagConfigType_t : int
        {
            CONFIG_TYPE_NONE = 0,
            CONFIG_TYPE_ALGO_NEW_REQ,
            CONFIG_TYPE_ALGO_NEW_REP,
            CONFIG_TYPE_DEVICE_NEW_REQ,
            CONFIG_TYPE_DEVICE_NEW_REP,
            CONFIG_TYPE_DEVICE_DELETE_REQ,
            CONFIG_TYPE_DEVICE_DELETE_REP,
            CONFIG_TYPE_SAIL_STATUS_SET_REQ,
            CONFIG_TYPE_SAIL_STATUS_SET_REP,
            CONFIG_TYPE_CREW_NEW_REQ,
            CONFIG_TYPE_CREW_NEW_REP
        }ConfigType;

        class IConfigParser;

        class ConfigParser
        {
        public:
            ConfigParser(void);
            ~ConfigParser(void);

        public:
            //解析
			//@data : 序列化数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 序列化数据
            const std::string compose(void);

            //获取配置类型
            //@Return ：配置类型
            const ConfigType getConfigType(void) const;

            //设置配置类型
            //@type : 配置类型
            //@Return ：错误码
            int setConfigType(
                const ConfigType type = ConfigType::CONFIG_TYPE_NONE);

            //获取算法信息
            //@Return ：算法信息
            const AlgorithmInfo getAlgorithmInfo(void) const;

            //设置算法信息
            //@info : 算法信息
            //@Return ：错误码
            int setAlgorithmInfo(const AlgorithmInfo info);

        private:
            IConfigParser* parser;
        };//class ConfigParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_CONFIG_PARSER_H