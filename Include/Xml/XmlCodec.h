//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-18
//		Description :					XML封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-06-18									创建
//

#ifndef XML_CODEC_H
#define XML_CODEC_H

namespace base
{
	namespace xml
	{
		class XMLParser
		{
		public:
			XMLParser(void);
			~XMLParser(void);

		public:
			//获取字段值
			//@fileName : 文件名称
			//@fieldName :　字段名称
			//@valueStr :　字段值
			//@Return : 错误码
			int getValueByName(
				const std::string fileName, 
				const std::string fieldName,
				std::string& valueStr);
		};//class XMLParser

		class XMLPacker
		{
		public:
			XMLPacker(void);
			~XMLPacker(void);

		public:
			//设置字段值
			//@fileName : 文件名称
			//@fieldName :　字段名称
			//@valueStr :　字段值
			//@Return : 错误码
			int setValueWithName(
				const std::string fileName,
				const std::string fieldName,
				const std::string valueStr);
		};//class XMLPacker
	}//namespace xml
}//namespace base

#endif//XML_CODEC_H
