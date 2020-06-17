#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Time/XTime.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Alarm/AlarmCodec.h"
#include "Protocol/Algorithm/AlgorithmCodec.h"
#include "Protocol/Component/ComponentCodec.h"
#include "Protocol/Crew/CrewCodec.h"
#include "Protocol/Device/DeviceCodec.h"
#include "Protocol/Status/StatusCodec.h"
#include "Protocol/MessageDefs.h"
#include "Protocol/MessageCodec.h"

namespace base
{
	namespace protocol
	{
		MessageParser::MessageParser(){}
		MessageParser::~MessageParser(){}

		void* MessageParser::createNewPacket()
		{
			MessagePacket* mp{ new(std::nothrow) MessagePacket };

			if (mp)
			{
				mp->type = MessageType::MESSAGE_TYPE_NONE;
				mp->sequence = 0;
				mp->timestamp = 0;
				mp->packet = nullptr;
			}

			return mp;
		}

		void MessageParser::destroyPacket(void* pkt /* = nullptr */)
		{
			MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };

			switch (mp->type)
			{
				case MessageType::MESSAGE_TYPE_ALARM:
				{
					break;
				}
				case MessageType::MESSAGE_TYPE_ALGORITHM:
				{
					break;
				}
				case MessageType::MESSAGE_TYPE_COMPONENT:
				{
					ComponentParser().destroyComponent(mp->packet);
					break;
				}
				case MessageType::MESSAGE_TYPE_CREW:
				{
					break;
				}
				case MessageType::MESSAGE_TYPE_DEVICE:
				{
					break;
				}
				case MessageType::MESSAGE_TYPE_STATUS:
				{
					StatusParser().destroyStatus(mp->packet);
					break;
				}
				case MessageType::MESSAGE_TYPE_USER:
				{
					break;
				}
				default:
					break;
			}

			boost::checked_delete(mp);
		}

		int MessageParser::unpackMessage(
			const char* msg /* = nullptr */, 
			void* pkt /* = nullptr */)
		{
			int e{ msg && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				msg::MSG m;
				e = m.ParseFromString(msg) ? eSuccess : eBadParseProtocol;

				if (eSuccess == e)
				{
					MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
					mp->sequence = m.sequence();
					mp->timestamp = m.timestamp();

					switch (m.type())
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
							mp->type = MessageType::MESSAGE_TYPE_COMPONENT;
							parseComponentMessage(m.release_component(), mp);
							break;
						}
						case msg::MSG_Type::MSG_Type_CREW:
						{
							break;
						}
						case msg::MSG_Type::MSG_Type_DEVICE:
						{
							break;
						}
						case msg::MSG_Type::MSG_Type_STATUS:
						{
							mp->type = MessageType::MESSAGE_TYPE_STATUS;
							parseStatusMessage(m.release_status(), mp);
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
			}

			return e;
		}

		int MessageParser::parseComponentMessage(
			void* c /* = nullptr */, 
			void* pkt /* = nullptr */)
		{
			int e{ c && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				ComponentParser cp;
				MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
				mp->packet = cp.createNewComponent();

				if (mp->packet)
				{
					e = cp.unpackComponent(c, mp->packet);
					if (eSuccess != e)
					{
						cp.destroyComponent(mp->packet);
						e = eBadParseProtocol;
					}
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		int MessageParser::parseStatusMessage(
			void* s /* = nullptr */, 
			void* pkt /* = nullptr */)
		{
			int e{ s && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				StatusParser sp;
				MessagePacket* mp{ reinterpret_cast<MessagePacket*>(pkt) };
				mp->packet = sp.createNewStatus();

				if (mp->packet)
				{
					e = sp.unpackStatus(s, mp->packet);
					if (eSuccess != e)
					{
						sp.destroyStatus(mp->packet);
						e = eBadParseProtocol;
					}
				}
				else
				{
					e = eBadNewObject;
				}
			}

			return e;
		}

		MessagePacker::MessagePacker()
		{}

		MessagePacker::~MessagePacker()
		{}

		const std::string MessagePacker::packMessage(
			const int type /* = 0 */, 
			const long long sequence /* = 0 */, 
			void* data /* = nullptr */)
		{
			std::string msgStr;
			msg::MSG m;
			m.set_type(static_cast<msg::MSG_Type>(type));
			m.set_sequence(sequence);
			m.set_timestamp(base::time::Time().tickcount());

			switch (type)
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
					m.set_allocated_component(reinterpret_cast<msg::Component*>(data));
					msgStr = m.SerializeAsString();
					m.release_component();

					break;
				}
				case msg::MSG_Type::MSG_Type_CREW:
				{
					break;
				}
				case msg::MSG_Type::MSG_Type_DEVICE:
				{
					break;
				}
				case msg::MSG_Type::MSG_Type_STATUS:
				{
					break;
				}
				case msg::MSG_Type::MSG_Type_USER:
				{
					break;
				}
				default:
					break;
			}

			return msgStr;
		}
	}//namespace protocol
}//namespace base
