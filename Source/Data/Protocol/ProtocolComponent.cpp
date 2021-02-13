#include "Error.h"
#include "output/Component.pb.h"
#include "Data/Protocol/ProtocolComponent.h"

namespace framework
{
	namespace data
	{
		ProtocolComponent::ProtocolComponent() : Data()
		{}
		ProtocolComponent::~ProtocolComponent() {}

		int ProtocolComponent::unpack(const std::string msg, ComponentMsg& cm)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::ComponentMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Command
					cm.command = msg_.command();

					//Status
					if (msg_.has_status())
					{
						cm.status = msg_.status();
					}

					//pid
					if (msg_.has_pid())
					{
						cm.pid = msg_.pid();
					}

					//Components
					for (int i = 0; i != msg_.components_size(); ++i)
					{
						const msg::Component& mc{ msg_.components(i) };
						ComponentData cd;
						cd.type = mc.type();
						cd.name = mc.name();
						cd.ipv4 = mc.ipv4();
						if (mc.has_id())
						{
							cd.id = mc.id();
						}
						if (mc.has_nickname())
						{
							cd.nickname = mc.nickname();
						}
						cm.cds.push_back(cd);
					}
				}
				else
				{
					e = eBadParseProtocol;
				}
			}

			return e;
		}

		const std::string ProtocolComponent::pack(const ComponentMsg& cm)
		{
			std::string str;
			msg::ComponentMessage compmsg;

			//command
			compmsg.set_command(static_cast<msg::ComponentMessage_Command>(cm.command));
			//status
			compmsg.set_status(cm.status);
			//pid
			compmsg.set_pid(cm.pid);
			//components
			for (int i = 0; i != cm.cds.size(); ++i)
			{
				msg::Component* mc{ compmsg.add_components() };
				mc->set_type(static_cast<msg::Component_Type>(cm.cds[i].type));
				mc->set_name(cm.cds[i].name);
				mc->set_ipv4(cm.cds[i].ipv4);
				mc->set_id(cm.cds[i].id);
				mc->set_nickname(cm.cds[i].nickname);
			}

			compmsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
