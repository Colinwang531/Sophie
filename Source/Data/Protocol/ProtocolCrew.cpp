#include "Error.h"
#include "output/Crew.pb.h"
#include "Data/Protocol/ProtocolCrew.h"

namespace framework
{
	namespace data
	{
		ProtocolCrew::ProtocolCrew() : Data()
		{}
		ProtocolCrew::~ProtocolCrew()
		{}

		int ProtocolCrew::unpack(const std::string msg, CrewMsg& cm)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::CrewMessage msg_;
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

					//Crew
					for (int i = 0; i != msg_.crews_size(); ++i)
					{
						const msg::Crew& crew{ msg_.crews(i) };
						CrewData cd;
						cd.id = crew.id();
						if (crew.has_name())
						{
							cd.name = crew.name();
						}
						if (crew.has_job())
						{
							cd.job = crew.job();
						}
						for (int j = 0; j != crew.pictures_size(); ++j)
						{
							cd.pictures.push_back(crew.pictures(j));
						}
					}
				}
			}

			return e;
		}

		const std::string ProtocolCrew::pack(const CrewMsg& cm)
		{
			std::string str;
			msg::CrewMessage crewmsg;
			
			//command
			crewmsg.set_command(static_cast<msg::CrewMessage_Command>(cm.command));
			//status
			crewmsg.set_status(cm.status);
			//pid
			crewmsg.set_pid(cm.pid);
			//Crews
			for (int i = 0; i != cm.crews.size(); ++i)
			{
				msg::Crew* c{ crewmsg.add_crews() };
				c->set_id(cm.crews[i].id);
				c->set_name(cm.crews[i].name);
				c->set_job(cm.crews[i].job);
				for (int j = 0; j != cm.crews[i].pictures.size(); ++j)
				{
					c->add_pictures(cm.crews[i].pictures[j].c_str(), cm.crews[i].pictures[j].length());
				}
			}

			crewmsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
