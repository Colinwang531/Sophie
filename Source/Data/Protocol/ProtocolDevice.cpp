#include "Error.h"
#include "output/Device.pb.h"
#include "Data/Protocol/ProtocolDevice.h"

namespace framework
{
	namespace data
	{
		ProtocolDevice::ProtocolDevice() : Data()
		{}
		ProtocolDevice::~ProtocolDevice()
		{}

		int ProtocolDevice::unpack(const std::string msg, DeviceMsg& dm)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::DeviceMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Command
					dm.command = msg_.command();

					//Status
					if (msg_.has_status())
					{
						dm.status = msg_.status();
					}

					//pid
					if (msg_.has_pid())
					{
						dm.pid = msg_.pid();
					}

					//Devices
					for (int i = 0; i != msg_.devices_size(); ++i)
					{
						const msg::Device& md{ msg_.devices(i) };
						DeviceData dd;
						dd.did = md.did();
						if (md.has_enable())
						{
							dd.enable = md.enable();
						}
						if (md.has_factory())
						{
							dd.factory = md.factory();
						}
						if (md.has_type())
						{
							dd.type = md.type();
						}
						if (md.has_username())
						{
							dd.username = md.username();
						}
						if (md.has_userpwd())
						{
							dd.userpwd = md.userpwd();
						}
						if (md.has_ip())
						{
							dd.ipv4 = md.ip();
						}
						if (md.has_port())
						{
							dd.port = md.port();
						}
						if (md.has_nickname())
						{
							dd.nickname = md.nickname();
						}

						for (int j = 0; j != md.cameras_size(); ++j)
						{
							const msg::Camera& mc{ md.cameras(i) };
							CameraData cd;
							cd.type = mc.type();
							cd.did = mc.id();
							cd.index = mc.index();
							cd.enable = mc.enable();
							cd.nickname = mc.nickname();
							cd.ipv4 = mc.ip();
							dd.cds.push_back(cd);
						}

						dm.dds.push_back(dd);
					}
				}
				else
				{
					e = eBadParseProtocol;
				}
			}

			return e;
		}

		const std::string ProtocolDevice::pack(const DeviceMsg& dm)
		{
 			std::string str;
			msg::DeviceMessage devmsg;

			//command
			devmsg.set_command(static_cast<msg::DeviceMessage_Command>(dm.command));
			//status
			devmsg.set_status(dm.status);
			//pid
			devmsg.set_pid(dm.pid);
			//devices
			for (int i = 0; i != dm.dds.size(); ++i)
			{
				msg::Device* md{ devmsg.add_devices() };
				md->set_did(dm.dds[i].did);
				md->set_enable(dm.dds[i].enable);
				md->set_factory(static_cast<msg::Device_Factory>(dm.dds[i].factory));
				md->set_type(static_cast<msg::Device_Type>(dm.dds[i].type));
				md->set_username(dm.dds[i].username);
				md->set_userpwd(dm.dds[i].userpwd);
				md->set_ip(dm.dds[i].ipv4);
				md->set_port(dm.dds[i].port);
				md->set_nickname(dm.dds[i].nickname);

				for (int j = 0; j != dm.dds[i].cds.size(); ++j)
				{
					msg::Camera* mc{ md->add_cameras() };
					mc->set_type(static_cast<msg::Camera_Type>(2/*dm.dds[i].cds[j].type*/));
					mc->set_id(dm.dds[i].cds[j].did);
					mc->set_index(dm.dds[i].cds[j].index);
					mc->set_enable(dm.dds[i].cds[j].enable);
					mc->set_nickname(dm.dds[i].cds[j].nickname);
					mc->set_ip(dm.dds[i].cds[j].ipv4);
				}
			}

			devmsg.SerializeToString(&str);
 			return str;
		}
	}//namespace protocol
}//namespace framework
