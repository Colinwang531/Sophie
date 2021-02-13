#include "Error.h"
#include "output/Algorithm.pb.h"
#include "Data/Protocol/ProtocolAlgorithm.h"

namespace framework
{
	namespace data
	{
		ProtocolAlgorithm::ProtocolAlgorithm() : Data()
		{}
		ProtocolAlgorithm::~ProtocolAlgorithm()
		{}

		int ProtocolAlgorithm::unpack(const std::string msg, AlgorithmMsg& am)
		{
			int e{ msg.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				msg::AlgorithmMessage msg_;
				if (msg_.ParseFromArray(msg.c_str(), static_cast<int>(msg.length())))
				{
					//Command
					am.command = msg_.command();
					
					//Status
					if (msg_.has_status())
					{
						am.status = msg_.status();
					}

					//pid
					if (msg_.has_pid())
					{
						am.pid = msg_.pid();
					}

					//Algorithm
					for (int i = 0; i != msg_.algorithms_size(); ++i)
					{
						const msg::Algorithm& algo{ msg_.algorithms(i) };
						AlgorithmData ad;
						ad.type = algo.type();
						ad.gpu = algo.gpu();
						ad.id = algo.id();
						ad.camera = algo.camera();
						ad.detect = algo.dectect();
						ad.track = algo.track();
						if (algo.has_seconddetectforphone())
						{
							ad.detect2forphone = algo.seconddetectforphone();
						}
						if (algo.has_similarforface())
						{
							ad.similarforface = algo.similarforface();
						}
						if (algo.has_inoutforface())
						{
							ad.inoutforface = algo.inoutforface();
						}
						if (algo.has_triggertime())
						{
							ad.triggertimeforsleep = algo.triggertime();
						}
						am.algos.push_back(ad);
					}
				}
			}

			return e;
		}

		const std::string ProtocolAlgorithm::pack(const AlgorithmMsg& am)
		{
			std::string str;
			msg::AlgorithmMessage algomsg;

			//command
			algomsg.set_command(static_cast<msg::AlgorithmMessage_Command>(am.command));
			//status
			algomsg.set_status(am.status);
			//pid
			algomsg.set_pid(am.pid);
			//Algorithms
			for (int i = 0; i != am.algos.size(); ++i)
			{
				msg::Algorithm* ma{ algomsg.add_algorithms() };
				ma->set_type(static_cast<msg::Algorithm_Type>(am.algos[i].type));
				ma->set_gpu(am.algos[i].gpu);
				ma->set_id(am.algos[i].id);
				ma->set_camera(am.algos[i].camera);
				ma->set_dectect(am.algos[i].detect);
				ma->set_track(am.algos[i].track);
				ma->set_seconddetectforphone(am.algos[i].detect2forphone);
				ma->set_similarforface(am.algos[i].similarforface);
				ma->set_inoutforface(am.algos[i].inoutforface);
				ma->set_triggertime(am.algos[i].triggertimeforsleep);
			}

			algomsg.SerializeToString(&str);
			return str;
		}
	}//namespace protocol
}//namespace base
