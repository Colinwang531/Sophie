#include "Error.h"
#include "output/Status.pb.h"
#include "Data/Protocol/ProtocolStatus.h"

namespace framework
{
	namespace data
	{
		ProtocolStatus::ProtocolStatus() : Data()
		{}
		ProtocolStatus::~ProtocolStatus()
		{}

		int ProtocolStatus::unpack(const std::string msg, StatusMsg& sm)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::StatusMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Command
					sm.command = msg_.command();

					//State
					if (msg_.has_state())
					{
						sm.state = msg_.state().state();
					}

					//Status
					if (msg_.has_status())
					{
						sm.state = msg_.status();
					}

					//Manual controller
					if (msg_.has_controller())
					{
						const msg::Controller& c{ msg_.controller() };
						sm.mc.flag = c.operation();

						if (c.has_state())
						{
							sm.mc.state = c.state();
						}
					}

					//pid
					if (msg_.has_pid())
					{
						sm.pid = msg_.pid();
					}
				}
			}

			return e;
		}

		const std::string ProtocolStatus::pack(const StatusMsg& sm)
		{
			std::string str;
			msg::StatusMessage statusmsg;

			//command
			statusmsg.set_command(static_cast<msg::StatusMessage_Command>(sm.command));
			//State
			msg::State* ms{ statusmsg.mutable_state() };
			ms->set_state(sm.state);
			//Manual controller
			msg::Controller* mc{ statusmsg.mutable_controller() };
			mc->set_operation(sm.mc.flag);
			mc->set_state(sm.mc.state);
			//Status
			statusmsg.set_status(sm.status);
			//pid
			statusmsg.set_pid(sm.pid);

			statusmsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
