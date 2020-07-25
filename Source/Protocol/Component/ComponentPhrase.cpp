#include "boost/cast.hpp"
#include "boost/checked_delete.hpp"
#include "boost/shared_ptr.hpp"
#include "Component/AbstractComponent.h"
using AbstractComponent = base::component::AbstractComponent;
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Message.pb.h"
#include "Protocol/Component/Component.pb.h"
#include "Protocol/Component/ComponentPhrase.h"

namespace base
{
	namespace protocol
	{
		ComponentParser::ComponentParser() : CommandParser() {}
		ComponentParser::~ComponentParser(){}

		void* ComponentParser::parseComponentMessage(void* msg /* = nullptr */)
		{
			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
			AbstractPacket* ap{ nullptr };
			msg::Component* mc{ mm->release_component() };

			if (mc)
			{
				const ComponentCommand command{ 
					static_cast<ComponentCommand>(mc->command()) };

				if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == command)
				{
					ap = new(std::nothrow) MessagePacket(
						base::packet::PacketType::PACKET_TYPE_COMPONENT, static_cast<int>(command));

					if (ap)
					{
						const msg::ComponentInfo componentInfo{ 
							mc->release_componentrequest()->componentinfo() };
						AbstractComponent* ac{ 
							new(std::nothrow) AbstractComponent(static_cast<base::component::ComponentType>(componentInfo.type())) };

						if (ac)
						{
							if (componentInfo.has_cid())
							{
								ac->setComponentID(componentInfo.cid());
							}
							if (componentInfo.has_cname())
							{
								ac->setComponentName(componentInfo.cname());
							}
							ap->setPacketData(ac);
							ap->setPacketSequence(mm->sequence());
						}
						else
						{
							boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(ap));
							ap = nullptr;
						}
					}
				}
				else if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REP == command)
				{
// 					ComponentResponse* rep{ new(std::nothrow) ComponentResponse };
// 
// 					if (rep)
// 					{
// 						msg::ComponentResponse* response{ mc->release_componentresponse() };
// 						rep->result = response->result();
// 						if (response->has_cid())
// 						{
// 							rep->cid = response->cid();
// 						}
// 						const int infosize{ response->componentinfos_size() };
// 						for (int i = 0; i != infosize; ++i)
// 						{
// 							const msg::ComponentInfo cinfo{ response->componentinfos(i) };
// 							ComponentInfo info;
// 							info.type = static_cast<ComponentType>(cinfo.type());
// 							info.cname = cinfo.cname();
// 							info.cid = cinfo.cid();
// 							rep->cinfos.push_back(info);
// 						}
// 						cp->data = rep;
// 					} 
// 					else
// 					{
// 						e = eBadNewObject;
// 					}
				}
			}

			return ap;
		}

		ComponentPacker::ComponentPacker(){}
		ComponentPacker::~ComponentPacker(){}

		void* ComponentPacker::packToComponentMessage(
			const int command /* = 0 */, 
			const int result /* = 0 */, 
			const void* data /* = nullptr */)
		{
			msg::Component* c{ msg::Component().New() };

			if (c)
			{
				c->set_command(static_cast<msg::Component_Command>(command));
				const ComponentCommand commandType{ static_cast<ComponentCommand>(command) };

				if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == commandType ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == commandType)
				{
// 					msg::ComponentRequest* req{ c->mutable_componentrequest() };
// 					msg::ComponentInfo* ci{ req->mutable_componentinfo() };
// 					ComponentInfo* info{ reinterpret_cast<ComponentInfo*>(data) };
// 					ci->set_type(static_cast<msg::ComponentInfo_Type>(info->type));
// 					ci->set_cid(info->cid);
// 					ci->set_cname(info->cname);
				}
				else if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == commandType ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REP == commandType)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(result);
					rep->set_cid(reinterpret_cast<const char*>(data));
				}
				else if (ComponentCommand::COMPONENT_COMMAND_QUERY_REP == commandType)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(result);

					std::vector<boost::shared_ptr<AbstractComponent>>* componentPtrs{
						reinterpret_cast<std::vector<boost::shared_ptr<AbstractComponent>>*>(const_cast<void*>(data)) };
					for (std::vector<boost::shared_ptr<AbstractComponent>>::iterator it = componentPtrs->begin();
						it != componentPtrs->end(); ++it)
					{
						msg::ComponentInfo* info{ rep->add_componentinfos() };
						info->set_type(static_cast<msg::ComponentInfo_Type>((*it)->getComponentType()));
						info->set_cid((*it)->getComponentID());
						info->set_cname((*it)->getComponentName());
					}
				}
			}
			
			return c;
		}
	}//namespace protocol
}//namespace base
