#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "AI/AbstractAlgorithm.h"
using AbstractAlgorithm = base::ai::AbstractAlgorithm;
#include "Packet/Message/MessagePacket.h"
using MessagePacket = base::packet::MessagePacket;
using MessagePacketPtr = boost::shared_ptr<MessagePacket>;
#ifdef WINDOWS
#include "Protocol/win/Message.pb.h"
#include "Protocol/win/Algorithm.pb.h"
#else
#include "Protocol/linux/Message.pb.h"
#include "Protocol/linux/Algorithm.pb.h"
#endif//WINDOWS
#include "Protocol/AlgorithmPhrase.h"

namespace base
{
	namespace protocol
	{
		AlgorithmParser::AlgorithmParser(){}
		AlgorithmParser::~AlgorithmParser(){}

		DataPacketPtr AlgorithmParser::parseMessage(void* a /* = nullptr */)
		{
			DataPacketPtr pkt{
				boost::make_shared<MessagePacket>(
					base::packet::MessagePacketType::MESSAGE_PACKET_TYPE_ALGORITHM) };

			if (pkt)
			{
				msg::Algorithm* ma{ reinterpret_cast<msg::Algorithm*>(a) };
				const msg::Algorithm_Command command{ ma->command() };
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				msgpkt->setMessagePacketCommand(static_cast<int>(command));

				if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REQ == command ||
					msg::Algorithm_Command::Algorithm_Command_QUERY_REQ == command)
				{
					const msg::AlgorithmInfo& info{
							ma->release_algorithmrequest()->algorithminfo() };
					AbstractAlgorithm* aa{
						new(std::nothrow) AbstractAlgorithm(
							static_cast<base::ai::AlgorithmType>(info.type())) };

					if (aa)
					{
						aa->setGpuID(info.gpu());
						aa->setDetectThreshold(info.dectectfirst(), info.dectectsecond());
						aa->setTrackThreshold(info.track());
						pkt->setPacketData(aa);
					}
				}
				else if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REP == command ||
					msg::Algorithm_Command::Algorithm_Command_QUERY_REP == command)
				{
				}
			}

			return pkt;
		}

		AlgorithmPacker::AlgorithmPacker() {}
		AlgorithmPacker::~AlgorithmPacker() {}

		const std::string AlgorithmPacker::packMessage(DataPacketPtr pkt)
		{
			std::string msgstr;
			msg::MSG mm;
			mm.set_type(msg::MSG_Type::MSG_Type_ALGORITHM);
			mm.set_sequence(pkt->getPacketSequence());
			mm.set_timestamp(pkt->getPacketTimestamp());
			msg::Algorithm* a{ mm.mutable_algorithm() };

			if (a)
			{
				MessagePacketPtr msgpkt{ boost::dynamic_pointer_cast<MessagePacket>(pkt) };
				const msg::Algorithm_Command command{
					static_cast<msg::Algorithm_Command>(msgpkt->getMessagePacketCommand()) };
				a->set_command(command);

				if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REP == command)
				{
					msg::AlgorithmResponse* rep{ a->mutable_algorithmresponse() };
					rep->set_result(msgpkt->getMessageStatus());
				}
			}

			mm.SerializeToString(&msgstr);
			mm.release_component();
			return msgstr;
		}
	}//namespace protocol
}//namespace base
