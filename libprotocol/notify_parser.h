//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					通知解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_NOTIFY_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_NOTIFY_PARSER_H

#include "defs.h"

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagNotifyType_t : int
        {
            NOTIFY_TYPE_NONE = 0,
            NOTIFY_TYPE_AIS_POSITION_A,
            NOTIFY_TYPE_AIS_POSITION_B,
            NOTIFY_TYPE_AIS_SHIP_STATIC,
		    NOTIFY_TYPE_AIS_STANDARD_SAR,
		    NOTIFY_TYPE_AIS_AIDS_TO_NAVIGATION,
		    NOTIFY_TYPE_CLOCK_SYC
        }NotifyType;

        class INotifyParser;

        class NotifyParser
        {
        public:
            NotifyParser(void);
            ~NotifyParser(void);

        public:
            //解析
			//@data : 数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 数据序列化字符串
            const std::string compose(void);

            //获取通知类型
            //@Return ：通知类型
            const NotifyType getNotifyType(void) const;

            //设置通知类型
            //@type : 通知类型
            //@Return ：错误码
            int setNotifyType(
                const NotifyType type = NotifyType::NOTIFY_TYPE_NONE);

            //获取母钟时间信息
            //@Return ：母钟时间信息
            const std::string getClockInfo(void) const;

            //设置母钟时间信息
            //@info : 母钟时间信息
            //@Return ：错误码
            int setClockInfo(const std::string info);

            //获取AIS位置A类信息
            //@Return ：AIS位置A类信息
            const PositionA getAISPositionAInfo(void) const;

            //设置AIS位置A类信息
            //@info : AIS位置A类信息
            //@Return ：错误码
            int setAISPositionAInfo(const PositionA info);

            //获取AIS位置B类信息
            //@Return ：AIS位置B类信息
            const PositionB getAISPositionBInfo(void) const;

            //设置AIS位置B类信息
            //@info : AIS位置B类信息
            //@Return ：错误码
            int setAISPositionBInfo(const PositionB info);

            //获取船状态信息
            //@Return ：船状态信息
            const ShipStatic getAISShipStaticInfo(void) const;

            //设置船状态信息
            //@info : 船状态信息
            //@Return ：错误码
            int setAISShipStaticInfo(const ShipStatic info);

            //获取标准SAR信息
            //@Return ：标准SAR信息
            const StandardSAR getAISStandardSARInfo(void) const;

            //设置标准SAR信息
            //@info : 标准SAR信息
            //@Return ：错误码
            int setAISStandardSARInfo(const StandardSAR info);

            //获取辅助航行信息
            //@Return ：辅助航行信息
            const AidsToNavigation getAISAidsToNavigationInfo(void) const;

            //设置辅助航行信息
            //@info : 辅助航行信息
            //@Return ：错误码
            int setAISAidsToNavigationInfo(const AidsToNavigation info);

        private:
            INotifyParser* parser;
        };//class NotifyParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_NOTIFY_PARSER_H