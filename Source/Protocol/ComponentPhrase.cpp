#include <vector>
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
using AbstractComponentPtr = boost::shared_ptr<AbstractComponent>;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Component.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Component.pb.h"
#endif//WINDOWS
#include "Protocol/ComponentPhrase.h"

namespace base
{
	namespace protocol
	{
		ComponentParser::ComponentParser() {}
		ComponentParser::~ComponentParser() {}

		DataPacketPtr ComponentParser::parseMessage(void* c /* = nullptr */)
		{
			DataPacketPtr pkt{ 
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_COMPONENT) };

			if (pkt)
			{
				msg::Component* mc{ reinterpret_cast<msg::Component*>(c) };
				const msg::Component_Command command{ mc->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Component_Command::Component_Command_SIGNIN_REQ == command ||
					msg::Component_Command::Component_Command_SIGNOUT_REQ == command)
				{
					const msg::ComponentInfo& info{
							mc->release_componentrequest()->componentinfo() };
					AbstractComponent* ac{
						new(std::nothrow) AbstractComponent(
							static_cast<base::component::ComponentType>(info.type())) };

					if (ac)
					{
						if (info.has_componentid())
						{
							ac->setComponentID(info.componentid());
						}
						if (info.has_commid())
						{
							ac->setCommunicationID(info.commid());
						}
						if (info.has_cname())
						{
							ac->setComponentName(info.cname());
						}

						pkt->setPacketData(ac);
					}
				}
				else if (msg::Component_Command::Component_Command_SIGNIN_REP == command ||
					msg::Component_Command::Component_Command_SIGNOUT_REP == command)
				{
					msg::ComponentResponse* rep{ mc->release_componentresponse() };
					if (rep->has_cid())
					{
						pkt->setPacketData((void*)(rep->cid().c_str()));
					}
					else if (rep->has_result())
					{
						msgpkt->setMessageStatus(rep->result());
					}
				}
				else if (msg::Component_Command::Component_Command_QUERY_REP == command)
				{
					msg::ComponentResponse* rep{ mc->release_componentresponse() };
					if (eSuccess == rep->result())
					{
						const int infosize{ rep->componentinfos_size() };
						for (int i = 0; i != infosize; ++i)
						{
							const msg::ComponentInfo info{ rep->componentinfos(i) };
							AbstractComponent* ac{
								new(std::nothrow) AbstractComponent(
									static_cast<base::component::ComponentType>(info.type())) };

							if (ac)
							{
								ac->setComponentName(info.cname());
								ac->setComponentID(info.componentid());
								ac->setCommunicationID(info.commid());
								pkt->setPacketData(ac);
							}
						}
					}
				}
			}

			return pkt;
		}

		ComponentPacker::ComponentPacker(){}
		ComponentPacker::~ComponentPacker(){}

		const std::string ComponentPacker::packMessage(DataPacketPtr pkt)
		{
			std::string msgstr;
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_COMPONENT);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Component* c{ mm.mutable_component() };

			if (c)
			{
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				const msg::Component_Command command{
					static_cast<msg::Component_Command>(msgpkt->getMessagePacketCommand()) };
				c->set_command(command);

				if (msg::Component_Command::Component_Command_SIGNIN_REQ == command ||
					msg::Component_Command::Component_Command_SIGNOUT_REQ == command)
				{
					msg::ComponentRequest* req{ c->mutable_componentrequest() };
					msg::ComponentInfo* ci{ req->mutable_componentinfo() };
					AbstractComponent* ac{ 
						reinterpret_cast<AbstractComponent*>(pkt->getPacketData()) };

					ci->set_type(static_cast<msg::ComponentInfo_Type>(ac->getComponentType()));
					ci->set_componentid(ac->getComponentID());
					ci->set_commid(ac->getCommunicationID());
					ci->set_cname(ac->getComponentName());
				}
				else if (msg::Component_Command::Component_Command_SIGNIN_REP == command ||
					msg::Component_Command::Component_Command_SIGNOUT_REP == command)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(msgpkt->getMessageStatus());
					rep->set_cid(reinterpret_cast<const char*>(pkt->getPacketData()));
				}
				else if (msg::Component_Command::Component_Command_QUERY_REP == command)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(msgpkt->getMessageStatus());
					std::vector<AbstractComponentPtr>* acps{ 
						reinterpret_cast<std::vector<AbstractComponentPtr>*>(pkt->getPacketData()) };

					for (int i = 0; i != (*acps).size(); ++i)
					{
						AbstractComponentPtr ac{ (*acps)[i] };
						if (ac)
						{
							msg::ComponentInfo* info{ rep->add_componentinfos() };
							info->set_type(static_cast<msg::ComponentInfo_Type>(ac->getComponentType()));
							info->set_componentid(ac->getComponentID());
							info->set_commid(ac->getCommunicationID());
							info->set_cname(ac->getComponentName());
						}
						else
						{
							break;
						}
					}
				}
			}
			
			mm.SerializeToString(&msgstr);
			mm.release_component();
			return msgstr;
		}
	}//namespace protocol
}//namespace base
