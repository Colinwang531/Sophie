//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					报警解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_ALARM_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_ALARM_PARSER_H

#include "defs.h"

namespace framework
{
    namespace libprotocol
    {
        class IAlarmParser;

        class AlarmParser
        {
        public:
            AlarmParser(void);
            ~AlarmParser(void);

        public:
            //解析
			//@data : 数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 数据序列化字符串
            const std::string compose(void);

            //获取报警信息
            //@Return ：报警信息
            const AlarmInfo getAlarmInfo(void) const;

            //设置报警信息
            //@info : 报警信息
            //@Return ：错误码
            int setAlarmInfo(const AlarmInfo info);

        private:
            IAlarmParser* parser;
        };//class AlarmParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_ALARM_PARSER_H