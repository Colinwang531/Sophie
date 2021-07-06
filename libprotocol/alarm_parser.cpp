#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "detail/alarm.pb.h"
#include "libcommon/error.h"
#include "alarm_parser.h"

namespace framework
{
    namespace libprotocol
    {
        class IAlarmParser
        {
        public:
            IAlarmParser(void);
            ~IAlarmParser(void);

        public:
            CommonError parse(const std::string data);
            const std::string compose(void);
            const AlarmInfo getAlarmInfo(void) const;
            CommonError setAlarmInfo(const AlarmInfo info);

        private:
            AlarmInfo alarmInfo;
        };//class IAlarmParser

        IAlarmParser::IAlarmParser()
        {}
        IAlarmParser::~IAlarmParser()
        {}

        CommonError IAlarmParser::parse(const std::string data)
        {
            CommonError e{CommonError::COMMON_ERROR_BAD_OPERATE};
            msg::AlarmMessage msg;

            if (msg.ParseFromArray(data.c_str(), static_cast<int>(data.length())))
            {
                msg::AlarmInfo info{msg.info()};
                alarmInfo.type = static_cast<AlarmType>(info.type());
                alarmInfo.cameraID = info.camera();
                alarmInfo.timestamp = info.timestamp();
                alarmInfo.picture = info.picture();
                for (int i = 0; i != info.positioninfos_size(); ++i)
                {
                    msg::PositionInfo pos{info.positioninfos(i)}; 
                    alarmInfo.positions.push_back(
                        PositionInfo{pos.x(), pos.y(), pos.w(), pos.h()});
                }
                if (info.has_idforface())
                {
                    alarmInfo.faceID = info.idforface();
                }
                if (info.has_inoutforface())
                {
                    alarmInfo.checkin = info.inoutforface();
                }
                if (info.has_bodycount())
                {
                    alarmInfo.personNum = info.bodycount();
                }

                e = CommonError::COMMON_ERROR_SUCCESS;
            }

            return e;
        }

        const std::string IAlarmParser::compose()
        {
            std::string data;
            msg::AlarmMessage msg;
            
            msg::AlarmInfo* info{msg.mutable_info()};
			info->set_type(static_cast<msg::AlarmInfo_AlarmType>(alarmInfo.type));
            info->set_camera(alarmInfo.cameraID);
            info->set_timestamp(alarmInfo.timestamp);
            info->set_picture(alarmInfo.picture);
            for (int i = 0; i != alarmInfo.positions.size(); ++i)
            {
                msg::PositionInfo* pos{info->add_positioninfos()};
                pos->set_x(alarmInfo.positions[i].x);
                pos->set_y(alarmInfo.positions[i].y);
                pos->set_w(alarmInfo.positions[i].w);
                pos->set_h(alarmInfo.positions[i].h);
            }
            info->set_idforface(alarmInfo.faceID);
            info->set_inoutforface(alarmInfo.checkin);
            info->set_bodycount(alarmInfo.personNum);
			msg.SerializeToString(&data);

            return data;
        }

        const AlarmInfo IAlarmParser::getAlarmInfo() const
        {
            return alarmInfo;
        }

        CommonError IAlarmParser::setAlarmInfo(const AlarmInfo info)
        {
            alarmInfo = info;
            return CommonError::COMMON_ERROR_SUCCESS;
        }

        AlarmParser::AlarmParser() : parser{boost::factory<IAlarmParser*>()()}
        {}
        AlarmParser::~AlarmParser()
        {
            boost::checked_delete(parser);
        }

        int AlarmParser::parse(const std::string data)
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

        const std::string AlarmParser::compose()
        {
            return parser ? parser->compose() : "";
        }

        const AlarmInfo AlarmParser::getAlarmInfo() const
        {
            AlarmInfo info;
            return parser ? parser->getAlarmInfo() : info;
        }

        int AlarmParser::setAlarmInfo(const AlarmInfo info)
        {
            return static_cast<int>(parser ? parser->setAlarmInfo(info) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
        }
    }//namespace libprotocol
}//namespace framework