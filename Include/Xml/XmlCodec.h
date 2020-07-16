//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-18
//		Description :					XML��װ/������
//
//		History:						Author									Date										Description
//										������									2020-06-18									����
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
			//��ȡ�ֶ�ֵ
			//@fileName : �ļ�����
			//@fieldName :���ֶ�����
			//@valueStr :���ֶ�ֵ
			//@Return : ������
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
			//�����ֶ�ֵ
			//@fileName : �ļ�����
			//@fieldName :���ֶ�����
			//@valueStr :���ֶ�ֵ
			//@Return : ������
			int setValueWithName(
				const std::string fileName,
				const std::string fieldName,
				const std::string valueStr);
		};//class XMLPacker
	}//namespace xml
}//namespace base

#endif//XML_CODEC_H
