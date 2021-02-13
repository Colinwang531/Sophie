#include "Error.h"
#include "output/AIS.pb.h"
#include "Data/Protocol/ProtocolAIS.h"

namespace framework
{
	namespace data
	{
		ProtocolAIS::ProtocolAIS() : Data()
		{}
		ProtocolAIS::~ProtocolAIS()
		{}

		int ProtocolAIS::unpack(const std::string msg, AISMsg& am)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::AISMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Type
					am.type = msg_.type();

					//pid
					am.pid = msg_.pid();

					//PositionA
					if (msg_.has_positiona())
					{
						const msg::PositionA& pa{ msg_.positiona() };
						am.positiona.repeatindicator = pa.repeatindicator();
						am.positiona.mmsi = pa.mmsi();
						am.positiona.navigationalstatus = pa.navigationalstatus();
						am.positiona.rot = pa.rot();
						am.positiona.sog = pa.sog();
						am.positiona.positionaccuracy = pa.positionaccuracy();
						am.positiona.longitude = pa.longitude();
						am.positiona.latitude = pa.latitude();
						am.positiona.cog = pa.cog();
						am.positiona.trueheading = pa.trueheading();
						am.positiona.timestamp = pa.timestamp();
						am.positiona.specialindicator = pa.specialindicator();
						am.positiona.spare = pa.spare();
					}

					//PositionB
					if (msg_.has_positionb())
					{
						const msg::PositionB& pb{ msg_.positionb() };
						am.positionb.repeatindicator = pb.repeatindicator();
						am.positionb.mmsi = pb.mmsi();
						am.positionb.spare1 = pb.spare1();
						am.positionb.sog = pb.sog();
						am.positionb.positionaccuracy = pb.positionaccuracy();
						am.positionb.longitude = pb.longitude();
						am.positionb.latitude = pb.latitude();
						am.positionb.cog = pb.cog();
						am.positionb.trueheading = pb.trueheading();
						am.positionb.timestamp = pb.timestamp();
						am.positionb.spare2 = pb.spare2();
						am.positionb.bunitflag = pb.bunitflag();
						am.positionb.bdisplayflag = pb.bdisplayflag();
						am.positionb.bdscflag = pb.bdscflag();
						am.positionb.bbandflag = pb.bbandflag();
						am.positionb.bmessage22flag = pb.bmessage22flag();
						am.positionb.modeflag = pb.modeflag();
						am.positionb.raimflag = pb.raimflag();
						am.positionb.commstateselectorflag = pb.commstateselectorflag();
						am.positionb.syncstate = pb.syncstate();
						am.positionb.slotincrement = pb.slotincrement();
						am.positionb.numberofslots = pb.numberofslots();
						am.positionb.keepflag = pb.keepflag();
					}

					//ShipStatic
					if (msg_.has_shipstatic())
					{
						const msg::ShipStatic& ss{ msg_.shipstatic() };
						am.shipstatic.repeatindicator = ss.repeatindicator();
						am.shipstatic.mmsi = ss.mmsi();
						am.shipstatic.aisversion = ss.aisversion();
						am.shipstatic.imo = ss.imo();
						am.shipstatic.callsign = ss.callsign();
						am.shipstatic.name = ss.name();
						am.shipstatic.typeofshipcargo = ss.typeofshipcargo();
						am.shipstatic.dimensions = ss.dimensions();
						am.shipstatic.epfd = ss.epfd();
						am.shipstatic.month = ss.month();
						am.shipstatic.day = ss.day();
						am.shipstatic.hour = ss.hour();
						am.shipstatic.minute = ss.minute();
						am.shipstatic.draught = ss.draught();
						am.shipstatic.destination = ss.destination();
						am.shipstatic.dte = ss.dte();
						am.shipstatic.spare = ss.spare();
					}

					//StandardSAR
					if (msg_.has_shipstatic())
					{
						const msg::StandardSAR& ss{ msg_.standardsar() };
						am.standardsar.repeatindicator = ss.repeatindicator();
						am.standardsar.mmsi = ss.mmsi();
						am.standardsar.altitude = ss.altitude();
						am.standardsar.sog = ss.sog();
						am.standardsar.positionaccuracy = ss.positionaccuracy();
						am.standardsar.longitude = ss.longitude();
						am.standardsar.latitude = ss.latitude();
						am.standardsar.cog = ss.cog();
						am.standardsar.timestamp = ss.timestamp();
						am.standardsar.altitudesensor = ss.altitudesensor();
						am.standardsar.spare = ss.spare();
						am.standardsar.assignmodeflag = ss.assignmodeflag();
						am.standardsar.commstateselectorflag = ss.commstateselectorflag();
						am.standardsar.communicationstate = ss.communicationstate();
					}

					//AidsToNavigation
					if (msg_.has_aidstonavigation())
					{
						const msg::AidsToNavigation& aids{ msg_.aidstonavigation() };
						am.aidstonavigation.repeatindicator = aids.repeatindicator();
						am.aidstonavigation.mmsi = aids.mmsi();
						am.aidstonavigation.navigationtype = aids.navigationtype();
						am.aidstonavigation.name = aids.name();
						am.aidstonavigation.positionaccuracy = aids.positionaccuracy();
						am.aidstonavigation.longitude = aids.longitude();
						am.aidstonavigation.latitude = aids.latitude();
						am.aidstonavigation.dimensions = aids.dimensions();
						am.aidstonavigation.epfd = aids.epfd();
						am.aidstonavigation.utctimestamp = aids.utctimestamp();
						am.aidstonavigation.onoffpositionindicator = aids.onoffpositionindicator();
						am.aidstonavigation.atonflag = aids.atonflag();
						am.aidstonavigation.raimflag = aids.raimflag();
						am.aidstonavigation.virtualflag = aids.virtualflag();
						am.aidstonavigation.modeindicator = aids.modeindicator();
						am.aidstonavigation.spare = aids.spare();
						am.aidstonavigation.extendname = aids.extendname();
						am.aidstonavigation.stufbits = aids.stufbits();
					}
				}
			}

			return e;
		}

		const std::string ProtocolAIS::pack(const AISMsg& am)
		{
			std::string str;
			msg::AISMessage aismsg;

			//type
			aismsg.set_type(static_cast<msg::AISMessage_Type>(am.type));
			//pid
			aismsg.set_pid(am.pid);
			//PositionA
			if (1 == am.type)
			{
				msg::PositionA* pa{ aismsg.mutable_positiona() };
				pa->set_repeatindicator(am.positiona.repeatindicator);
				pa->set_mmsi(am.positiona.mmsi);
				pa->set_navigationalstatus(am.positiona.navigationalstatus);
				pa->set_rot(am.positiona.rot);
				pa->set_sog(am.positiona.sog);
				pa->set_positionaccuracy(am.positiona.positionaccuracy);
				pa->set_longitude(am.positiona.longitude);
				pa->set_latitude(am.positiona.latitude);
				pa->set_cog(am.positiona.cog);
				pa->set_trueheading(am.positiona.trueheading);
				pa->set_timestamp(am.positiona.timestamp);
				pa->set_specialindicator(am.positiona.specialindicator);
				pa->set_spare(am.positiona.spare);
			} 
			//PositionB
			else if (2 == am.type)
			{
				msg::PositionB* pb{ aismsg.mutable_positionb() };
				pb->set_repeatindicator(am.positionb.repeatindicator);
			}
			//ShipStatic
			else if (3 == am.type)
			{
				msg::ShipStatic* pa{ aismsg.mutable_shipstatic() };
			}
			//StandardSAR
			else if (4 == am.type)
			{
				msg::StandardSAR* sar{ aismsg.mutable_standardsar() };
			}
			//AidsToNavigation
			else if (5 == am.type)
			{
				msg::AidsToNavigation* sar{ aismsg.mutable_aidstonavigation() };
			}

			aismsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
