#include "Protocol/Message.pb.h"
#include "Packet/AbstractPacket.h"
using AbstractPacket = base::packet::AbstractPacket;
//#include "Protocol/Algorithm/AlgorithmPhrase.h"
//#include "Protocol/Component/ComponentPhrase.h"
// #include "Protocol/Device/DevicePhrase.h"
// #include "Protocol/Status/StatusPhrase.h"
// #include "Protocol/Crew/CrewPhrase.h"
// #include "Protocol/Alarm/AlarmPhrase.h"
#include "Protocol/CommandPhrase.h"

namespace base
{
	namespace protocol
	{
		CommandParser::CommandParser(){}
		CommandParser::~CommandParser(){}

		void* CommandParser::parseFromArray(
			const void* data /* = nullptr */, 
			const unsigned int databytes /* = 0 */)
		{
			void* obj{ nullptr };
			msg::MSG mm;

			if (data && 0 < databytes && mm.ParseFromArray(data, databytes))
			{
				switch (mm.type())
				{
					case msg::MSG_Type::MSG_Type_ALARM:
					{
						parseAlarmMessage(&mm);
						break;
					}
					case msg::MSG_Type::MSG_Type_ALGORITHM:
					{
						parseAlgorithmMessage(&mm);
						break;
					}
					case msg::MSG_Type::MSG_Type_COMPONENT:
					{
						obj = parseComponentMessage(&mm);
						break;
					}
					case msg::MSG_Type::MSG_Type_CREW:
					{
						obj = parseCrewMessage(&mm);
						break;
					}
					case msg::MSG_Type::MSG_Type_DEVICE:
					{
						obj = parseDeviceMessage(&mm);
						break;
					}
					case msg::MSG_Type::MSG_Type_STATUS:
					{
						parseStatusMessage(&mm);
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

			return obj;
		}

		void* CommandParser::parseAlarmMessage(void* msg /* = nullptr */)
		{
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					AlarmParser().unpackFromAlarmMessage(mm->release_alarm())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		void* CommandParser::parseAlgorithmMessage(void* msg /* = nullptr */)
		{
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					AlgorithmParser().unpackFromAlgorithmMessage(mm->release_algorithm())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		void* CommandParser::parseComponentMessage(void* msg /* = nullptr */)
		{
			return nullptr;
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					ComponentParser().unpackFromComponentMessage(mm->release_component())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		void* CommandParser::parseDeviceMessage(void* msg /* = nullptr */)
		{
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					DeviceParser().unpackFromDeviceMessage(mm->release_device())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		void* CommandParser::parseStatusMessage(void* msg /* = nullptr */)
		{
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					StatusParser().unpackFromStatusMessage(mm->release_status())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		void* CommandParser::parseCrewMessage(void* msg /* = nullptr */)
		{
// 			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
// 			AbstractPacket* ap{
// 				reinterpret_cast<AbstractPacket*>(
// 					CrewParser().unpackFromCrewMessage(mm->release_crew())) };
// 
// 			if (ap)
// 			{
// 				ap->setPacketSequence(mm->sequence());
// 			}
// 
// 			return ap;
			return nullptr;
		}

		CommandPacker::CommandPacker()
		{}

		CommandPacker::~CommandPacker()
		{}

		const std::string CommandPacker::packPacketToArray(void* pkt /* = nullptr */)
		{
			std::string rep;
			AbstractPacket* ap{ 
				reinterpret_cast<AbstractPacket*>(pkt) };

			if (ap)
			{
				const base::packet::PacketType pkttype{ ap->getPacketType() };
				msg::MSG mm;
				mm.set_type(static_cast<msg::MSG_Type>(pkttype));
				mm.set_sequence(ap->getPacketSequence());
				mm.set_timestamp(ap->getPacketTimestamp());

				switch (pkttype)
				{
					case base::packet::PacketType::PACKET_TYPE_ALARM:
					{
						break;
					}
					case base::packet::PacketType::PACKET_TYPE_ALGORITHM:
					{
						break;
					}
					case base::packet::PacketType::PACKET_TYPE_COMPONENT:
					{
// 						msg::Component* mc{ 
// 							reinterpret_cast<msg::Component*>(
// 								ComponentPacker().packToComponentMessage(
// 									ap->getPacketDataCommandType(), 
// 									ap->getPacketDataReplyResult(), 
// 									ap->getPacketData())) };
// 						;
// 						mm.set_allocated_component(mc);
// 						mm.SerializeToString(&rep);
// 						mm.release_component();

						break;
					}
					case base::packet::PacketType::PACKET_TYPE_CREW:
					{
						break;
					}
					case base::packet::PacketType::PACKET_TYPE_DEVICE:
					{
						break;
					}
					case base::packet::PacketType::PACKET_TYPE_STATUS:
					{
						break;
					}
					case base::packet::PacketType::PACKET_TYPE_USER:
					{
						break;
					}
					default:
						break;
				}
			}

			return rep;
		}
	}//namespace protocol
}//namespace base
