#include "boost/pointer_cast.hpp"
#include "boost/make_shared.hpp"
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using MessagePacketPtr = boost::shared_ptr<base::packet::MessagePacket>;
#include "Protocol/Message.pb.h"
#include "Protocol/Component/ComponentPhrase.h"
#include "Protocol/Algorithm/AlgorithmPhrase.h"
#include "Protocol/Device/DevicePhrase.h"
#include "Protocol/Status/StatusPhrase.h"
#include "Protocol/Crew/CrewPhrase.h"
#include "Protocol/Alarm/AlarmPhrase.h"
#include "Protocol/DataPhrase.h"

namespace base
{
	namespace protocol
	{
		DataParser::DataParser() {}
		DataParser::~DataParser() {}

		DataPacketPtr DataParser::parseData(const std::string data)
		{
			DataPacketPtr pkt;
			msg::MSG msg_;

			if (msg_.ParseFromArray(data.c_str(), data.length()))
			{
				if (msg_.has_alarm())
				{
//					pkt = AlarmParser().parseAlarmMessage(msg_.release_alarm());
				}
				else if (msg_.has_algorithm())
				{
//					pkt = AlgorithmParser().parseAlgorithmMessage(msg_.release_algorithm());
				}
				else if (msg_.has_component())
				{
					pkt = ComponentParser().parseMessage(msg_.release_component());
				}
				else if (msg_.has_crew())
				{
//					pkt = CrewParser().parseCrewMessage(msg_.release_crew());
				}
				else if (msg_.has_device())
				{
					pkt = DeviceParser().parseMessage(msg_.release_device());
				}
				else if (msg_.has_status())
				{
//					pkt = StatusParser().parseStatusMessage(msg_.release_status());
				}
				else if (msg_.has_user())
				{
				}
			}

			if (pkt)
			{
				pkt->setPacketSequence(msg_.sequence());
				pkt->setPacketTimestamp(msg_.timestamp());
			}

			return pkt;
		}

		DataPacker::DataPacker() {}
		DataPacker::~DataPacker() {}

		const std::string DataPacker::packData(DataPacketPtr pkt)
		{
			std::string msgstr;
			const base::packet::DataPacketType packetType{ 
				static_cast<base::packet::DataPacketType>(pkt->getPacketType()) };

			if (base::packet::DataPacketType::DATA_PACKET_TYPE_MESSAGE == packetType)
			{
				MessagePacketPtr msgpkt{ 
					boost::dynamic_pointer_cast<base::packet::MessagePacket>(pkt) };
 				const base::packet::MessagePacketType msgpkttype{ 
					static_cast<base::packet::MessagePacketType>(msgpkt->getMessagePacketType()) };

				switch (msgpkttype)
				{
					case msg::MSG_Type::MSG_Type_ALARM:
					{
						break;
					}
					case msg::MSG_Type::MSG_Type_ALGORITHM:
					{
						break;
					}
					case msg::MSG_Type::MSG_Type_COMPONENT:
					{
						msgstr = ComponentPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_CREW:
					{
						break;
					}
					case msg::MSG_Type::MSG_Type_DEVICE:
					{
// 						msg::Device* md{
// 							reinterpret_cast<msg::Device*>(
// 								DevicePacker().packToDeviceMessage(pkt)) };
// 						mm.set_allocated_device(md);
// 						mm.SerializeToString(&rep);
// 						mm.release_device();

						break;
					}
					case msg::MSG_Type::MSG_Type_STATUS:
					{
// 						msg::Status* ms{ 
// 							reinterpret_cast<msg::Status*>(
// 								StatusPacker().packToStatusMessage(pkt)) };
// 						mm.set_allocated_status(ms);
// 						mm.SerializeToString(&rep);
// 						mm.release_status();

						break;
					}
					case msg::MSG_Type::MSG_Type_USER:
					{
						break;
					}
					default:
						break;
				}
			}
			else if (base::packet::DataPacketType::DATA_PACKET_TYPE_VIDEO == packetType)
			{
			}
			else if (base::packet::DataPacketType::DATA_PACKET_TYPE_AUDIO == packetType)
			{
			}
			else if (base::packet::DataPacketType::DATA_PACKET_TYPE_IMAGE == packetType)
			{
			}

			return msgstr;
		}
	}//namespace protocol
}//namespace base