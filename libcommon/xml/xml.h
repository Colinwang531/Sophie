//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-23
//		Description :					XML解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-23									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_XML_PARSER_H
#define FRAMEWORK_LIBCOMMON_XML_PARSER_H

#include <string>

namespace framework
{
	namespace libcommon
	{
		class XmlParser
		{
		public:
			XmlParser(void);
			~XmlParser(void);

		public:
			//获取键-值
			//@fileName : 文件名称
			//@keyName : 键名称
			//@Return : 值数据
			const std::string getValue(
				const std::string fileName, 
				const std::string keyName);

			//设置键-值
			//@fileName : 文件名称
			//@keyName : 键名称
			//@value : 值数据
			//@Return : 错误码
			int setValue(
				const std::string fileName,
				const std::string keyName,
				const std::string value);
		};//class XmlParser
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_XML_PARSER_H
