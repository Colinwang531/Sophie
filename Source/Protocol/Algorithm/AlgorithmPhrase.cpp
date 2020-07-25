#include "boost/checked_delete.hpp"
#include "boost/polymorphic_cast.hpp"
#include "AI/HelmetDetect/HelmetDetect.h"
using HelmetDetect = base::ai::HelmetDetect;
#include "AI/SleepDetect/SleepDetect.h"
using SleepDetect = base::ai::SleepDetect;
#include "AI/PhoneDetect/PhoneDetect.h"
using PhoneDetect = base::ai::PhoneDetect;
#include "AI/FightDetect/FightDetect.h"
using FightDetect = base::ai::FightDetect;
#include "AI/FaceRecognition/FaceRecognition.h"
using FaceRecognition = base::ai::FaceRecognition;
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Message.pb.h"
#include "Protocol/Algorithm/Algorithm.pb.h"
#include "Protocol/Algorithm/AlgorithmPhrase.h"

namespace base
{
	namespace protocol
	{
		AlgorithmParser::AlgorithmParser(){}
		AlgorithmParser::~AlgorithmParser(){}

		void* AlgorithmParser::parseAlgorithmMessage(void* msg /* = nullptr */)
		{
			msg::MSG* mm{ reinterpret_cast<msg::MSG*>(msg) };
			AbstractPacket* ap{ nullptr };
			msg::Algorithm* ma{ mm->release_algorithm() };

			if (ma)
			{
				const AlgorithmCommand command{ 
					static_cast<AlgorithmCommand>(ma->command()) };

				if (AlgorithmCommand::ALGORITHM_COMMAND_SET_REQ == command ||
					AlgorithmCommand::ALGORITHM_COMMAND_QUERY_REQ == command)
				{
					ap = new(std::nothrow) MessagePacket(
						base::packet::PacketType::PACKET_TYPE_ALGORITHM, static_cast<int>(command));

					if (ap)
					{
						AbstractAlgorithm* aa{ nullptr };
						const msg::AlgorithmInfo& info{ ma->release_algorithmrequest()->algorithminfo() };
						const msg::AlgorithmInfo_Type type{ info.type() };

						if (msg::AlgorithmInfo_Type::AlgorithmInfo_Type_HELMET == type)
						{
							aa = new(std::nothrow) HelmetDetect(info.cid(), info.gpu());
						}
						else if (msg::AlgorithmInfo_Type::AlgorithmInfo_Type_SLEEP == type)
						{
							aa = new(std::nothrow) SleepDetect(info.cid(), info.gpu());
						}
						else if (msg::AlgorithmInfo_Type::AlgorithmInfo_Type_PHONE == type)
						{
							aa = new(std::nothrow) PhoneDetect(info.cid(), info.gpu());
							if (aa)
							{
								aa->setExtendDetectThreshold(info.dectectsecond());
							}
						}
						else if (msg::AlgorithmInfo_Type::AlgorithmInfo_Type_FIGHT == type)
						{
							aa = new(std::nothrow) FightDetect(info.cid(), info.gpu());
						}
						else if (msg::AlgorithmInfo_Type::AlgorithmInfo_Type_ATTENDANCE_IN == type ||
							msg::AlgorithmInfo_Type::AlgorithmInfo_Type_ATTENDANCE_OUT == type)
						{
							aa = new(std::nothrow) FaceRecognition(
								info.cid(), static_cast<base::ai::AlgorithmType>(info.type()), info.gpu());
							if (aa)
							{
								aa->setFaceCompareSimilar(info.similar());
							}
						}

						if (aa)
						{
							aa->setDetectThreshold(info.dectectfirst());
							aa->setTrackThreshold(info.track());
							ap->setPacketData(aa);
						}
						else
						{
							boost::checked_delete(boost::polymorphic_downcast<MessagePacket*>(ap));
							ap = nullptr;
						}
					}
				}
				else if (AlgorithmCommand::ALGORITHM_COMMAND_SET_REP == command ||
					AlgorithmCommand::ALGORITHM_COMMAND_QUERY_REP == command)
				{
					const int result{ ma->release_algorithmresponse()->result() };
					ap = new(std::nothrow) MessagePacket(
						base::packet::PacketType::PACKET_TYPE_ALGORITHM, static_cast<int>(command), result);
				}
			}

			return ap;
		}

		AlgorithmPacker::AlgorithmPacker() {}
		AlgorithmPacker::~AlgorithmPacker() {}

		void* AlgorithmPacker::packAlgorithm(
			const int command /* = 0 */,
			const int result /* = 0 */,
			void* data /* = nullptr */)
		{
			const msg::Algorithm_Command cmd{ static_cast<msg::Algorithm_Command>(command) };
			msg::Algorithm* s{ msg::Algorithm().New() };

			if (s)
			{
				s->set_command(cmd);

				if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REP == cmd)
				{
					msg::AlgorithmResponse* rep{ s->mutable_algorithmresponse() };
					rep->set_result(result);
				}
			}

			return s;
		}
	}//namespace protocol
}//namespace base
