//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					应用程序命令行参数处理类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef BASE_COMMAND_LINE_H
#define BASE_COMMAND_LINE_H

#include "boost/program_options.hpp"

namespace base
{
	namespace commandline
	{
		class CommandLine
		{
		public:
			CommandLine(void);
			virtual ~CommandLine(void);

		public:
			//设置命令行选项和对应值,该方法必须在parseCommandLine方法前调用且选项范围由设置范围决定
			//选项名称支持同时使用全名和简写,格式如"name,n"
			//@option : 命令行项
			//@param : 参数值
			//@Return : 错误码值
			int setCommandOptions(const char* option = nullptr, const char* param = nullptr);

			//获取命令行选项的值
			//选项名称支持同时使用全名和简写,格式如"name,n"
			//@option : 命令行项
			//@Return : 参数值,为空表示命令行项无效
			const char* getParameterByOption(const char* option = nullptr);

			//解析命令行选项
			int parseCommandLine(int argc, char** argv);

		private:
			boost::program_options::options_description desc;
			boost::program_options::variables_map variables;
		};//class CommandLine
	}//namespace commandline
}//namespace base

#endif//BASE_COMMAND_LINE_H
