//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-23
//		Description :					AIS解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-23									 创建
//

#ifndef FRAMEWORK_LIBAIS_AIS_PARSER_H
#define FRAMEWORK_LIBAIS_AIS_PARSER_H

#include "libcommon/defs.h"

namespace framework
{
	namespace libais
	{
		class IAISParser;

		class AISParser
		{
		public:
			AISParser(void);
			~AISParser(void);

		public:
			//解析
			//@data : 数据
            //@Return : 错误码
			int parse(const std::string data);

			//获取数据类型
            //@Return ：数据类型
            const AISDataType getDataType(void) const;

			//获取位置A类数据
            //@Return ：位置A类数据
            const PositionA getPositionAInfo(void) const;

			//获取位置B类数据
            //@Return ：位置B类数据
            const PositionB getPositionBInfo(void) const;

			//获取船状态数据
            //@Return ：船状态数据
            const ShipStatic getShipStaticInfo(void) const;

			//获取标准SAR数据
            //@Return ：标准SAR数据
            const StandardSAR getStandardSARInfo(void) const;

			//获取辅助航行数据
            //@Return ：辅助航行数据
            const AidsToNavigation getAidsToNavigationInfo(void) const;

		private:
			IAISParser* parser;
		};//class AISParser
	}//namespace libais
}//namespace framework

#endif//FRAMEWORK_LIBAIS_AIS_PARSER_H
