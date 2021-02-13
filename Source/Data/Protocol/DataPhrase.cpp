#include "boost/pointer_cast.hpp"
#include "boost/make_shared.hpp"
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using MessagePacketPtr = boost::shared_ptr<base::packet::MessagePacket>;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#endif//WINDOWS
#include "Protocol/ComponentPhrase.h"
#include "Protocol/AlgorithmPhrase.h"
#include "Protocol/DevicePhrase.h"
#include "Protocol/StatusPhrase.h"
#include "Protocol/CrewPhrase.h"
#include "Protocol/AlarmPhrase.h"
#include "Protocol/DataPhrase.h"
#include "Protocol/EventPhrase.h"

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

			if (msg_.ParseFromArray((const void*)data.c_str(), (int)data.length()))
			{
				if (msg_.has_alarm())
				{
					pkt = AlarmParser().parseMessage(msg_.release_alarm());
				}
				else if (msg_.has_algorithm())
				{
					pkt = AlgorithmParser().parseMessage(msg_.release_algorithm());
				}
				else if (msg_.has_component())
				{
					pkt = ComponentParser().parseMessage(msg_.release_component());
				}
				else if (msg_.has_crew())
				{
					pkt = CrewParser().parseMessage(msg_.release_crew());
				}
				else if (msg_.has_device())
				{
					pkt = DeviceParser().parseMessage(msg_.release_device());
				}
				else if (msg_.has_status())
				{
					pkt = StatusParser().parseMessage(msg_.release_status());
				}
				else if (msg_.has_user())
				{
				}
				else if (msg_.has_evt())
				{
					pkt = EventParser().parseMessage(msg_.release_evt());
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
						msgstr = AlarmPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_ALGORITHM:
					{
						msgstr = AlgorithmPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_COMPONENT:
					{
						msgstr = ComponentPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_CREW:
					{
						msgstr = CrewPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_DEVICE:
					{
						msgstr = DevicePacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_STATUS:
					{
						msgstr = StatusPacker().packMessage(pkt);
						break;
					}
					case msg::MSG_Type::MSG_Type_USER:
					{
						break;
					}
					case msg::MSG_Type::MSG_Type_EVENT:
					{
						msgstr = EventPacker().packMessage(pkt);
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
