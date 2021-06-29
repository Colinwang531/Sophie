//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					命令行解析类
//
//		History:						Author									Date										Description
//										王科威									 2020-05-06									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_COMMAND_LINE_PARSER_H
#define FRAMEWORK_LIBCOMMON_COMMAND_LINE_PARSER_H

namespace framework
{
	namespace libcommon
	{
		class ICommandLineParser;
		
		class CommandLineParser
		{
		public:
			CommandLineParser(void);
			virtual ~CommandLineParser(void);

		public:
			//设置命令行选项值
			//启动命令行参数解析前必须调用该方法且选项范围由设置范围决定
			//选项名支持使用全名和简写,例如"name,n"
			//@opt : 选项名
			//@val : 选项值
			//@Return : 错误码
			int setOption(
				const char* opt = nullptr, 
				const char* val = nullptr);

			//获取选项值
			//@option : 选项名
			//@Return : 选项值
			const char* getValue(const char* opt = nullptr);

			//解析命令行
			//@Return : 错误码
			int parse(
				const int argc = 0, 
				const char** argv = nullptr);

		private:
			ICommandLineParser* parser;
		};//class CommandLineParser
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_COMMAND_LINE_PARSER_H
