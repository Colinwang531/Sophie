//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-02
//		Description :					程序路径解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-06									创建
//

#ifndef FRAMEWORK_LIBCOMMON_PROGRAM_PATH_PARSER_H
#define FRAMEWORK_LIBCOMMON_PROGRAM_PATH_PARSER_H

namespace framework
{
	namespace libcommon
	{
		class ProgramPathParser
		{
		public:
			ProgramPathParser(void);
			~ProgramPathParser(void);

		public:
			//解析程序执行的目录路径
			//@fullPath : 程序完整路径，该路径可以为空
			//@Return : 程序执行的目录路径
			//@Comment ： 如果参数为空则自动获取程序完整路径
			const std::string parseFileDirectory(const std::string fullPath);

			//解析程序名称
			//@fullPath : 程序完整路径，该路径可以为空
			//@Return : 程序名称
			//@Comment ： 如果参数为空则自动获取程序完整路径
			const std::string parseFileName(const std::string fullPath);
		};//class ProgramPathParser
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_PROGRAM_PATH_PARSER_H
