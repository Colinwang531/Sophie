#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "detail/notify.pb.h"
#include "libcommon/error.h"
#include "notify_parser.h"

namespace framework
{
    namespace libprotocol
    {
        class INotifyParser
        {
        public:
            INotifyParser(void);
            ~INotifyParser(void);

        public:
            CommonError parse(const std::string data);
            const std::string compose(void);
            const NotifyType getNotifyType(void) const;
            CommonError setNotifyType(
                const NotifyType type = NotifyType::NOTIFY_TYPE_NONE);
            const std::string getClockInfo(void) const;
            CommonError setClockInfo(const std::string info);
            const PositionA getAISPositionAInfo(void) const;
            CommonError setAISPositionAInfo(const PositionA info);
            const PositionB getAISPositionBInfo(void) const;
            CommonError setAISPositionBInfo(const PositionB info);
            const ShipStatic getAISShipStaticInfo(void) const;
            CommonError setAISShipStaticInfo(const ShipStatic info);
            const StandardSAR getAISStandardSARInfo(void) const;
            CommonError setAISStandardSARInfo(const StandardSAR info);
            const AidsToNavigation getAISAidsToNavigationInfo(void) const;
            CommonError setAISAidsToNavigationInfo(const AidsToNavigation info);

        private:
            NotifyType notifyType;
            PositionA positionA;
            PositionB positionB;
            ShipStatic shipStatic;
            StandardSAR standardSAR;
            AidsToNavigation aidsToNav;
            std::string clockInfo;
        };//class INotifyParser

        INotifyParser::INotifyParser() : notifyType(NotifyType::NOTIFY_TYPE_NONE)
        {}
        INotifyParser::~INotifyParser()
        {}

        CommonError INotifyParser::parse(const std::string data)
        {
            CommonError e{CommonError::COMMON_ERROR_BAD_OPERATE};
            msg::NotificationMessage msg;

            if (msg.ParseFromArray(data.c_str(), static_cast<int>(data.length())))
            {
                notifyType = static_cast<NotifyType>(msg.type());
                if (msg.has_clocksyc())
                {
                    clockInfo = msg.clocksyc().clock();
                }
                if (msg.has_positiona())
                {
                    const msg::PositionA& pa{ msg.positiona() };
                    positionA.repeatindicator = pa.repeatindicator();
                    positionA.mmsi = pa.mmsi();
                    positionA.navigationalstatus = pa.navigationalstatus();
                    positionA.rot = pa.rot();
                    positionA.sog = pa.sog();
                    positionA.positionaccuracy = pa.positionaccuracy();
                    positionA.longitude = pa.longitude();
                    positionA.latitude = pa.latitude();
                    positionA.cog = pa.cog();
                    positionA.trueheading = pa.trueheading();
                    positionA.timestamp = pa.timestamp();
                    positionA.specialindicator = pa.specialindicator();
                    positionA.spare = pa.spare();
                }
                if (msg.has_positionb())
                {
                    const msg::PositionB& pb{ msg.positionb() };
                    positionB.repeatindicator = pb.repeatindicator();
                    positionB.mmsi = pb.mmsi();
                    positionB.spare1 = pb.spare1();
                    positionB.sog = pb.sog();
                    positionB.positionaccuracy = pb.positionaccuracy();
                    positionB.longitude = pb.longitude();
                    positionB.latitude = pb.latitude();
                    positionB.cog = pb.cog();
                    positionB.trueheading = pb.trueheading();
                    positionB.timestamp = pb.timestamp();
                    positionB.spare2 = pb.spare2();
                    positionB.bunitflag = pb.bunitflag();
                    positionB.bdisplayflag = pb.bdisplayflag();
                    positionB.bdscflag = pb.bdscflag();
                    positionB.bbandflag = pb.bbandflag();
                    positionB.bmessage22flag = pb.bmessage22flag();
                    positionB.modeflag = pb.modeflag();
                    positionB.raimflag = pb.raimflag();
                    positionB.commstateselectorflag = pb.commstateselectorflag();
                    positionB.syncstate = pb.syncstate();
                    positionB.slotincrement = pb.slotincrement();
                    positionB.numberofslots = pb.numberofslots();
                    positionB.keepflag = pb.keepflag();
                }
                if (msg.has_shipstatic())
                {
                    const msg::ShipStatic& ss{ msg.shipstatic() };
                    shipStatic.repeatindicator = ss.repeatindicator();
                    shipStatic.mmsi = ss.mmsi();
                    shipStatic.aisversion = ss.aisversion();
                    shipStatic.imo = ss.imo();
                    shipStatic.callsign = ss.callsign();
                    shipStatic.name = ss.name();
                    shipStatic.typeofshipcargo = ss.typeofshipcargo();
                    shipStatic.dimensions = ss.dimensions();
                    shipStatic.epfd = ss.epfd();
                    shipStatic.month = ss.month();
                    shipStatic.day = ss.day();
                    shipStatic.hour = ss.hour();
                    shipStatic.minute = ss.minute();
                    shipStatic.draught = ss.draught();
                    shipStatic.destination = ss.destination();
                    shipStatic.dte = ss.dte();
                    shipStatic.spare = ss.spare();
                }
                if (msg.has_standardsar())
                {
                    const msg::StandardSAR& ss{ msg.standardsar() };
                    standardSAR.repeatindicator = ss.repeatindicator();
                    standardSAR.mmsi = ss.mmsi();
                    standardSAR.altitude = ss.altitude();
                    standardSAR.sog = ss.sog();
                    standardSAR.positionaccuracy = ss.positionaccuracy();
                    standardSAR.longitude = ss.longitude();
                    standardSAR.latitude = ss.latitude();
                    standardSAR.cog = ss.cog();
                    standardSAR.timestamp = ss.timestamp();
                    standardSAR.altitudesensor = ss.altitudesensor();
                    standardSAR.spare = ss.spare();
                    standardSAR.assignmodeflag = ss.assignmodeflag();
                    standardSAR.commstateselectorflag = ss.commstateselectorflag();
                    standardSAR.communicationstate = ss.communicationstate();
                }
                if (msg.has_aidstonavigation())
                {
                    const msg::AidsToNavigation& aids{ msg.aidstonavigation() };
                    aidsToNav.repeatindicator = aids.repeatindicator();
                    aidsToNav.mmsi = aids.mmsi();
                    aidsToNav.navigationtype = aids.navigationtype();
                    aidsToNav.name = aids.name();
                    aidsToNav.positionaccuracy = aids.positionaccuracy();
                    aidsToNav.longitude = aids.longitude();
                    aidsToNav.latitude = aids.latitude();
                    aidsToNav.dimensions = aids.dimensions();
                    aidsToNav.epfd = aids.epfd();
                    aidsToNav.utctimestamp = aids.utctimestamp();
                    aidsToNav.onoffpositionindicator = aids.onoffpositionindicator();
                    aidsToNav.atonflag = aids.atonflag();
                    aidsToNav.raimflag = aids.raimflag();
                    aidsToNav.virtualflag = aids.virtualflag();
                    aidsToNav.modeindicator = aids.modeindicator();
                    aidsToNav.spare = aids.spare();
                    aidsToNav.extendname = aids.extendname();
                    aidsToNav.stufbits = aids.stufbits();
                }

                e = CommonError::COMMON_ERROR_SUCCESS;
            }

            return e;
        }

        const std::string INotifyParser::compose()
        {
            std::string data;
            msg::NotificationMessage msg;
            
			msg.set_type(static_cast<msg::NotificationMessage_CommandType>(notifyType));
            if (NotifyType::NOTIFY_TYPE_CLOCK_SYC == notifyType)
            {
                msg::ClockSyc* clock{msg.mutable_clocksyc()};
                clock->set_clock(clockInfo);
            }
			else if (NotifyType::NOTIFY_TYPE_AIS_POSITION_A == notifyType)
			{
				msg::PositionA* pa{msg.mutable_positiona()};
				pa->set_repeatindicator(positionA.repeatindicator);
				pa->set_mmsi(positionA.mmsi);
				pa->set_navigationalstatus(positionA.navigationalstatus);
				pa->set_rot(positionA.rot);
				pa->set_sog(positionA.sog);
				pa->set_positionaccuracy(positionA.positionaccuracy);
				pa->set_longitude(positionA.longitude);
				pa->set_latitude(positionA.latitude);
				pa->set_cog(positionA.cog);
				pa->set_trueheading(positionA.trueheading);
				pa->set_timestamp(positionA.timestamp);
				pa->set_specialindicator(positionA.specialindicator);
				pa->set_spare(positionA.spare);
			}
			else if (NotifyType::NOTIFY_TYPE_AIS_POSITION_B == notifyType)
			{
				msg::PositionB* pb{msg.mutable_positionb()};
				pb->set_repeatindicator(positionB.repeatindicator);
                pb->set_mmsi(positionB.mmsi);
                pb->set_spare1(positionB.spare1);
                pb->set_sog(positionB.sog);
                pb->set_positionaccuracy(positionB.positionaccuracy);
                pb->set_longitude(positionB.longitude);
                pb->set_latitude(positionB.latitude);
                pb->set_cog(positionB.cog);
                pb->set_trueheading(positionB.trueheading);
                pb->set_timestamp(positionB.timestamp);
                pb->set_spare2(positionB.spare2);
                pb->set_bunitflag(positionB.bunitflag);
                pb->set_bdisplayflag(positionB.bdisplayflag);
                pb->set_bdscflag(positionB.bdscflag);
                pb->set_bbandflag(positionB.bbandflag);
                pb->set_bmessage22flag(positionB.bmessage22flag);
                pb->set_modeflag(positionB.modeflag);
                pb->set_raimflag(positionB.raimflag);
                pb->set_commstateselectorflag(positionB.commstateselectorflag);
                pb->set_syncstate(positionB.syncstate);
                pb->set_slotincrement(positionB.slotincrement);
                pb->set_numberofslots(positionB.numberofslots);
                pb->set_keepflag(positionB.keepflag);
			}
			else if (NotifyType::NOTIFY_TYPE_AIS_SHIP_STATIC == notifyType)
			{
				msg::ShipStatic* ss{msg.mutable_shipstatic()};
                ss->set_repeatindicator(shipStatic.repeatindicator);
                ss->set_mmsi(shipStatic.mmsi);
                ss->set_aisversion(shipStatic.aisversion);
                ss->set_imo(shipStatic.imo);
                ss->set_callsign(shipStatic.callsign);
                ss->set_name(shipStatic.name);
                ss->set_typeofshipcargo(shipStatic.typeofshipcargo);
                ss->set_dimensions(shipStatic.dimensions);
                ss->set_epfd(shipStatic.epfd);
                ss->set_month(shipStatic.month);
                ss->set_day(shipStatic.day);
                ss->set_hour(shipStatic.hour);
                ss->set_minute(shipStatic.minute);
                ss->set_draught(shipStatic.draught);
                ss->set_destination(shipStatic.destination);
                ss->set_dte(shipStatic.dte);
                ss->set_spare(shipStatic.spare);
			}
			else if (NotifyType::NOTIFY_TYPE_AIS_STANDARD_SAR == notifyType)
			{
				msg::StandardSAR* sar{msg.mutable_standardsar()};
                sar->set_repeatindicator(standardSAR.repeatindicator);
                sar->set_mmsi(standardSAR.mmsi);
                sar->set_altitude(standardSAR.altitude);
                sar->set_sog(standardSAR.sog);
                sar->set_positionaccuracy(standardSAR.positionaccuracy);
                sar->set_longitude(standardSAR.longitude);
                sar->set_latitude(standardSAR.latitude);
                sar->set_cog(standardSAR.cog);
                sar->set_timestamp(standardSAR.timestamp);
                sar->set_altitudesensor(standardSAR.altitudesensor);
                sar->set_spare(standardSAR.spare);
                sar->set_assignmodeflag(standardSAR.assignmodeflag);
                sar->set_commstateselectorflag(standardSAR.commstateselectorflag);
                sar->set_communicationstate(standardSAR.communicationstate);
			}
			else if (NotifyType::NOTIFY_TYPE_AIS_AIDS_TO_NAVIGATION == notifyType)
			{
				msg::AidsToNavigation* nav{msg.mutable_aidstonavigation()};
                nav->set_repeatindicator(aidsToNav.repeatindicator);
                nav->set_mmsi(aidsToNav.mmsi);
                nav->set_navigationtype(aidsToNav.navigationtype);
                nav->set_name(aidsToNav.name);
                nav->set_positionaccuracy(aidsToNav.positionaccuracy);
                nav->set_longitude(aidsToNav.longitude);
                nav->set_latitude(aidsToNav.latitude);
                nav->set_dimensions(aidsToNav.dimensions);
                nav->set_epfd(aidsToNav.epfd);
                nav->set_utctimestamp(aidsToNav.utctimestamp);
                nav->set_onoffpositionindicator(aidsToNav.onoffpositionindicator);
                nav->set_atonflag(aidsToNav.atonflag);
                nav->set_raimflag(aidsToNav.raimflag);
                nav->set_virtualflag(aidsToNav.virtualflag);
                nav->set_modeindicator(aidsToNav.modeindicator);
                nav->set_spare(aidsToNav.spare);
                nav->set_extendname(aidsToNav.extendname);
                nav->set_stufbits(aidsToNav.stufbits);
			}

			msg.SerializeToString(&data);
            return data;
        }

        const NotifyType INotifyParser::getNotifyType() const
        {
            return notifyType;
        }

        CommonError INotifyParser::setNotifyType(
            const NotifyType type /*= NotifyType::NOTIFY_TYPE_NONE*/)
        {
            notifyType = type;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const std::string INotifyParser::getClockInfo() const
        {
            return clockInfo;
        }

        CommonError INotifyParser::setClockInfo(const std::string info)
        {
            clockInfo = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const PositionA INotifyParser::getAISPositionAInfo() const
        {
            return positionA;
        }

        CommonError INotifyParser::setAISPositionAInfo(const PositionA info)
        {
            positionA = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const PositionB INotifyParser::getAISPositionBInfo() const
        {
            return positionB;
        }

        CommonError INotifyParser::setAISPositionBInfo(const PositionB info)
        {
            positionB = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const ShipStatic INotifyParser::getAISShipStaticInfo() const
        {
            return shipStatic;
        }

        CommonError INotifyParser::setAISShipStaticInfo(const ShipStatic info)
        {
            shipStatic = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const StandardSAR INotifyParser::getAISStandardSARInfo() const
        {
            return standardSAR;
        }

        CommonError INotifyParser::setAISStandardSARInfo(const StandardSAR info)
        {
            standardSAR = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        const AidsToNavigation INotifyParser::getAISAidsToNavigationInfo() const
        {
            return aidsToNav;
        }

        CommonError INotifyParser::setAISAidsToNavigationInfo(const AidsToNavigation info)
        {
            aidsToNav = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        NotifyParser::NotifyParser() : parser{boost::factory<INotifyParser*>()()}
        {}
        NotifyParser::~NotifyParser()
        {
            boost::checked_delete(parser);
        }

        int NotifyParser::parse(const std::string data)
        {
            CommonError e{
                !data.empty() ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->parse(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::string NotifyParser::compose()
        {
            return parser ? parser->compose() : "";
        }

        const NotifyType NotifyParser::getNotifyType() const
        {
            return parser ? parser->getNotifyType() : NotifyType::NOTIFY_TYPE_NONE;
        }

        int NotifyParser::setNotifyType(const NotifyType type /*= NotifyType::NOTIFY_TYPE_NONE*/)
        {
            CommonError e{
                NotifyType::NOTIFY_TYPE_NONE < type ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setNotifyType(type) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const std::string NotifyParser::getClockInfo() const
        {
            return parser ? parser->getClockInfo() : "";
        }

        int NotifyParser::setClockInfo(const std::string info)
        {
            CommonError e{
                !info.empty() ? 
                CommonError::COMMON_ERROR_SUCCESS : 
                CommonError::COMMON_ERROR_INVALID_PARAMETER};

            if (CommonError::COMMON_ERROR_SUCCESS == e)
            {
                e = (parser ? parser->setClockInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
            }

            return static_cast<int>(e);
        }

        const PositionA NotifyParser::getAISPositionAInfo() const
        {
            PositionA positionA;
            return parser ? parser->getAISPositionAInfo() : positionA;
        }

        int NotifyParser::setAISPositionAInfo(const PositionA info)
        {
            return static_cast<int>(parser ? parser->setAISPositionAInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }

        const PositionB NotifyParser::getAISPositionBInfo() const
        {
            PositionB positionB;
            return parser ? parser->getAISPositionBInfo() : positionB;
        }

        int NotifyParser::setAISPositionBInfo(const PositionB info)
        {
            return static_cast<int>(parser ? parser->setAISPositionBInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }

        const ShipStatic NotifyParser::getAISShipStaticInfo() const
        {
            ShipStatic shipStatic;
            return parser ? parser->getAISShipStaticInfo() : shipStatic;
        }

        int NotifyParser::setAISShipStaticInfo(const ShipStatic info)
        {
            return static_cast<int>(parser ? parser->setAISShipStaticInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }

        const StandardSAR NotifyParser::getAISStandardSARInfo() const
        {
            StandardSAR standardSAR;
            return parser ? parser->getAISStandardSARInfo() : standardSAR;
        }

        int NotifyParser::setAISStandardSARInfo(const StandardSAR info)
        {
            return static_cast<int>(parser ? parser->setAISStandardSARInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }

        const AidsToNavigation NotifyParser::getAISAidsToNavigationInfo() const
        {
            AidsToNavigation nav;
            return parser ? parser->getAISAidsToNavigationInfo() : nav;
        }

        int NotifyParser::setAISAidsToNavigationInfo(const AidsToNavigation info)
        {
            return static_cast<int>(parser ? parser->setAISAidsToNavigationInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }
    }//namespace libprotocol
}//namespace framework