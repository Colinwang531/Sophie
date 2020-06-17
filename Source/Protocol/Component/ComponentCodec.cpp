#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Time/XTime.h"
#include "Number/Number.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Component/Component.pb.h"
#include "Protocol/MessageDefs.h"
#include "Protocol/Component/ComponentCodec.h"

namespace base
{
	namespace protocol
	{
		ComponentParser::ComponentParser(){}
		ComponentParser::~ComponentParser(){}

		void* ComponentParser::createNewComponent()
		{
			ComponentPacket* cp{ new(std::nothrow) ComponentPacket };

			if (cp)
			{
				cp->command = ComponentCommand::COMPONENT_COMMAND_NONE;
				cp->data = nullptr;
			}

			return cp;
		}

		void ComponentParser::destroyComponent(void* pkt /* = nullptr */)
		{
			ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(pkt) };

			if (cp)
			{
				if (ComponentCommand::COMPONENT_COMMAND_QUERY_REQ == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == cp->command)
				{
					boost::checked_delete(reinterpret_cast<ComponentRequest*>(cp->data));
				} 
				else if (ComponentCommand::COMPONENT_COMMAND_QUERY_REP == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REP == cp->command)
				{
					boost::checked_delete(reinterpret_cast<ComponentResponse*>(cp->data));
				}
			}

			boost::checked_delete(cp);
		}

		int ComponentParser::unpackComponent(
			void* c /* = nullptr */, 
			void* pkt /* = nullptr */)
		{
			int e{ c && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				msg::Component* mc{ reinterpret_cast<msg::Component*>(c) };
				ComponentPacket* cp{ reinterpret_cast<ComponentPacket*>(pkt) };
				cp->command = static_cast<ComponentCommand>(mc->command());

				if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REQ == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REQ == cp->command)
				{
					ComponentRequest* req{ new(std::nothrow) ComponentRequest };

					if (req)
					{
						const msg::ComponentInfo cinfo{ 
							mc->release_componentrequest()->componentinfo() };
						req->cinfo.type = static_cast<ComponentType>(cinfo.type());
						if (cinfo.has_cid())
						{
							req->cinfo.cid = cinfo.cid();
						}
						if (cinfo.has_cname())
						{
							req->cinfo.cname = cinfo.cname();
						}
						cp->data = req;
					} 
					else
					{
						e = eBadNewObject;
					}
				}
				else if (ComponentCommand::COMPONENT_COMMAND_SIGNIN_REP == cp->command ||
					ComponentCommand::COMPONENT_COMMAND_SIGNOUT_REP == cp->command)
				{
					ComponentResponse* rep{ new(std::nothrow) ComponentResponse };

					if (rep)
					{
						msg::ComponentResponse* response{ mc->release_componentresponse() };
						rep->result = response->result();
						if (response->has_cid())
						{
							rep->cid = response->cid();
						}
						const int infosize{ response->componentinfos_size() };
						for (int i = 0; i != infosize; ++i)
						{
							const msg::ComponentInfo cinfo{ response->componentinfos(i) };
							ComponentInfo info;
							info.type = static_cast<ComponentType>(cinfo.type());
							info.cname = cinfo.cname();
							info.cid = cinfo.cid();
							rep->cinfos.push_back(info);
						}
						cp->data = rep;
					} 
					else
					{
						e = eBadNewObject;
					}
				}
				else
				{
					e = eBadOperate;
				}
			}

			return e;
		}

		ComponentPacker::ComponentPacker(){}
		ComponentPacker::~ComponentPacker(){}

		void* ComponentPacker::packComponent(
			const int command /* = 0 */, 
			const int result /* = 0 */, 
			void* data /* = nullptr */)
		{
			const msg::Component_Command cmd{ static_cast<msg::Component_Command>(command) };
			msg::Component* c{ msg::Component().New() };

			if (c)
			{
				c->set_command(cmd);

				if (msg::Component_Command::Component_Command_SIGNIN_REQ == cmd ||
					msg::Component_Command::Component_Command_SIGNOUT_REQ == cmd)
				{
					msg::ComponentRequest* req{ c->mutable_componentrequest() };
					msg::ComponentInfo* ci{ req->mutable_componentinfo() };
					ComponentInfo* info{ reinterpret_cast<ComponentInfo*>(data) };
					ci->set_type(static_cast<msg::ComponentInfo_Type>(info->type));
					ci->set_cid(info->cid);
					ci->set_cname(info->cname);
				}
				else if (msg::Component_Command::Component_Command_SIGNIN_REP == cmd ||
					msg::Component_Command::Component_Command_SIGNOUT_REP == cmd)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(result);
					rep->set_cid(reinterpret_cast<const char*>(data));
				}
				else if (msg::Component_Command::Component_Command_QUERY_REP == cmd)
				{
					msg::ComponentResponse* rep{ c->mutable_componentresponse() };
					rep->set_result(result);

					std::vector<void*>* rcis{ 
						reinterpret_cast<std::vector<void*>*>(data) };
					for (int i = 0; i != rcis->size(); ++i)
					{
						msg::ComponentInfo* info{ rep->add_componentinfos() };
						ComponentInfo* ci{ reinterpret_cast<ComponentInfo*>((*rcis)[i]) };
						info->set_type(static_cast<msg::ComponentInfo_Type>(ci->type));
						info->set_cid(ci->cid);
						info->set_cname(ci->cname);
					}
				}
			}
			
			return c;
		}
	}//namespace protocol
}//namespace base
