#include <unordered_map>
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Data/AIS/AISData.h"
using AISData = framework::data::AISData;
#include "Decoder/AIS/AISDataDecoder.h"

namespace framework
{
	namespace multimedia
	{
		static std::unordered_map<char, char> numberCodingTable{
			{'0', 0b000000}, {'1', 0b000001}, {'2', 0b000010}, {'3', 0b000011}, {'4', 0b000100}, {'5', 0b000101}, {'6', 0b000110}, {'7', 0b000111}, {'8', 0b001000}, {'9', 0b001001},
			{':', 0b001010}, {';', 0b001011}, {'<', 0b001100}, {'=', 0b001101}, {'>', 0b001110}, {'?', 0b001111}, {'@', 0b010000},
			{'A', 0b010001}, {'B', 0b010010}, {'C', 0b010011}, {'D', 0b010100}, {'E', 0b010101}, {'F', 0b010110}, {'G', 0b010111}, {'H', 0b011000}, {'I', 0b011001},
			{'J', 0b011010}, {'K', 0b011011}, {'L', 0b011100}, {'M', 0b011101}, {'N', 0b011110}, {'O', 0b011111}, {'P', 0b100000}, {'Q', 0b100001}, {'R', 0b100010},
			{'S', 0b100011}, {'T', 0b100100}, {'U', 0b100101}, {'V', 0b100110}, {'W', 0b100111},
			{'`', 0b101000},
			{'a', 0b101001}, {'b', 0b101010}, {'c', 0b101011}, {'d', 0b101100}, {'e', 0b101101}, {'f', 0b101110}, {'g', 0b101111}, {'h', 0b110000}, {'i', 0b110001},
			{'j', 0b110010}, {'k', 0b110011}, {'l', 0b110100}, {'m', 0b110101}, {'n', 0b110110}, {'o', 0b110111}, {'p', 0b111000}, {'q', 0b111001}, {'r', 0b111010},
			{'s', 0b111011}, {'t', 0b111100}, {'u', 0b111101}, {'v', 0b111110}, {'w', 0b111111} };

		static std::unordered_map<char, char> charactorCodingTable{
			{0b000000, '@'},
			{0b000001, 'A'}, {0b000010, 'B'}, {0b000011, 'C'}, {0b000100, 'D'}, {0b000101, 'E'}, {0b000110, 'F'}, {0b000111, 'G'}, {0b001000, 'H'}, {0b001001, 'I'},
			{0b001010, 'J'}, {0b001011, 'K'}, {0b001100, 'L'}, {0b001101, 'M'}, {0b001110, 'N'}, {0b001111, 'O'}, {0b010000, 'P'}, {0b010001, 'Q'},
			{0b010010, 'R'}, {0b010011, 'S'}, {0b010100, 'T'}, {0b010101, 'U'}, {0b010110, 'V'}, {0b010111, 'W'}, {0b011000, 'X'}, {0b011001, 'Y'},
			{0b011010, 'Z'},
			{0b011011, '['}, {0b011100, '\\'}, {0b011101, ']'}, {0b011110, '^'}, {0b011111, '-'}, {0b100000, ' '}, {0b100001, '!'}, {0b100010, '"'},
			{0b100011, '#'}, {0b100100, '$'}, {0b100101, '%'}, {0b100110, '&'}, {0b100111, '\''}, {0b101000, '('}, {0b101001, ')'}, {0b101010, '*'},
			{0b101011, '+'}, {0b101100, ','}, {0b101101, '-'}, {0b101110, ' '}, {0b101111, '/'},
			{0b110000, '0'}, {0b110001, '1'}, {0b110010, '2'}, {0b110011, '3'}, {0b110100, '4'}, {0b110101, '5'}, {0b110110, '6'}, {0b110111, '7'},
			{0b111000, '8'}, {0b111001, '9'},
			{0b111010, ' '}, {0b111011, ';'}, {0b111100, '<'}, {0b111101, '='}, {0b111110, '>'}, {0b111111, '?'} };

		AISDataDecoder::AISDataDecoder(
			DataNotificationCallback callback /* = nullptr */) : dataNotificationCallback{ callback }
		{}
		AISDataDecoder::~AISDataDecoder(void)
		{}

		int AISDataDecoder::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (framework::data::DataFactory::DATA_FACTORY_AIS == data->getDataFactory())
				{
					const char* msg{ reinterpret_cast<const char*>(data->getData()) };
					const char msgID{ numberCodingTable[msg[0]] };

					if (1 == msgID || 2 == msgID || 3 == msgID)
					{
						parseMessage_1_2_3(msg);
					} 
					else if (5 == msgID)
					{
						parseMessage5(msg);
					}
					else if (9 == msgID)
					{
						parseMessage9(msg);
					}
					else if (18 == msgID)
					{ 
						parseMessage18(msg);
					}
					else if (19 == msgID)
					{
						parseMessage19(msg);
					}
					else if (21 == msgID)
					{
						parseMessage21(msg);
					}
					else
					{
						e = eBadOperate;
					}
				} 
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		void AISDataDecoder::parseMessage_1_2_3(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->pa.repeatindicator = (numberCodingTable[msg[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->pa.mmsi.append((boost::format("%ld") % userID).str());

			//Navigation Status
			//38~41
			dp->pa.navigationalstatus = (numberCodingTable[msg[6]] & 0xf);

			//Rate of turn
			//42~49
			short rot{ numberCodingTable[msg[7]] };
			rot <<= 6;
			rot |= numberCodingTable[msg[8]];
			//	rot >>= 4;
			dp->pa.rot = rot;

				//SOG
			short temp{ numberCodingTable[msg[8]] };
			temp <<= 6;
			temp |= numberCodingTable[msg[9]];
			temp &= 0x3ff;
			dp->pa.sog = temp / 10.0f;

			//Position accuracy
			dp->pa.positionaccuracy = (numberCodingTable[msg[10]] >> 5);

			//Longitude
			int longitude{ numberCodingTable[msg[10]] & 0x1f };
			longitude <<= 6;
			longitude |= numberCodingTable[msg[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[13]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[14]];
			longitude >>= 1;
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			dp->pa.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			int latitude{ numberCodingTable[msg[14]] & 0x01 };
			latitude <<= 6;
			latitude |= numberCodingTable[msg[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[17]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[18]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[19]];
			latitude >>= 4;
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			dp->pa.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			int cog{ numberCodingTable[msg[19]] & 0x0f };
			cog <<= 6;
			cog |= numberCodingTable[msg[20]];
			cog <<= 6;
			cog |= numberCodingTable[msg[21]];
			cog >>= 4;
			dp->pa.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[msg[21]] & 0xf };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[msg[22]];
			trueHeading >>= 1;
			dp->pa.trueheading = trueHeading;

			char timestamp{ numberCodingTable[msg[22]] & 0x1 };
			timestamp <<= 6;
			timestamp |= numberCodingTable[msg[23]];
			timestamp >>= 1;
			dp->pa.timestamp = timestamp;

			char regionalApplication{ numberCodingTable[msg[23]] & 0x1 };
			regionalApplication <<= 6;
			regionalApplication |= numberCodingTable[msg[24]];
			regionalApplication >>= 3;

			char spare{ numberCodingTable[msg[24]] & 0x4 };
			spare >>= 2;
			dp->pa.spare = spare;

			char raimFlag{ numberCodingTable[msg[24]] & 0x2 };
			spare >>= 1;

			int communicationState{ numberCodingTable[msg[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[27]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[28]];
			communicationState <<= 1;

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}

		void AISDataDecoder::parseMessage5(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->ss.repeatindicator = (numberCodingTable[msg[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->ss.mmsi.append((boost::format("%ld") % userID).str());

			//AIS Version Indicator
			char aisVersionIndicator{ numberCodingTable[msg[6]] & 0xc };
			aisVersionIndicator >>= 2;
			dp->ss.aisversion = aisVersionIndicator;

			//IMO Number
			long long imoNumber{ numberCodingTable[msg[6]] & 0x3 };
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[msg[7]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[msg[8]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[msg[9]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[msg[10]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[msg[11]];
			imoNumber <<= 4;
			imoNumber >>= 6;
			dp->ss.imo = imoNumber;

			//Call Sign
			long long callSign{ numberCodingTable[msg[11]] & 0x3 };
			callSign <<= 6;
			callSign |= numberCodingTable[msg[12]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[13]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[14]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[15]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[16]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[17]];
			callSign <<= 6;
			callSign |= numberCodingTable[msg[18]];
			callSign <<= 4;
			callSign >>= 6;
			//Total 7 charactors
			std::string callSign_;
			for (int i = 0; i != 7; ++i)
			{
				char c{ callSign & 0x3f };
				callSign_ += charactorCodingTable[c];
				callSign >>= 6;
			}
			std::reverse(callSign_.begin(), callSign_.end());
			dp->ss.callsign.append(callSign_);

			//Name
			std::string name;
			for (int i = 18; i != 38; ++i)
			{
				long long temp{ numberCodingTable[msg[i]] & 0x3 };
				temp <<= 4;
				temp |= (numberCodingTable[msg[i + 1]] >> 2);
				name += charactorCodingTable[temp];
			}
			dp->ss.name.append(name);

			//Type of ship & cargo
			char shipAndCargoType{ numberCodingTable[msg[38]] & 0x3 };
			shipAndCargoType <<= 6;
			shipAndCargoType |= numberCodingTable[msg[39]];
			dp->ss.typeofshipcargo = shipAndCargoType;

			//Ship dimension
			int shipDimension{ numberCodingTable[msg[40]] };
			for (int i = 41; i != 45; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[msg[i]];
			}
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };
			dp->ss.dimensions.append((boost::format("A:%d B:%d C:%d D:%d") % aval % bval % cval % dval).str());

			//Type of electronic position
			dp->ss.epfd = (numberCodingTable[msg[45]] >> 2);

			//ETA
			int eta{ numberCodingTable[msg[45]] & 0x3 };
			for (int i = 46; i != 49; ++i)
			{
				eta <<= 6;
				eta |= numberCodingTable[msg[i]];
			}
			dp->ss.minute = (eta & 0x3f);
			dp->ss.hour = ((eta >> 6) & 0x1f);
			dp->ss.day = ((eta >> 11) & 0x1f);
			dp->ss.month = ((eta >> 16) & 0xf);

			//Max present static draught
			short draught{ numberCodingTable[msg[49]] };
			draught <<= 2;
			draught |= (numberCodingTable[msg[50]] >> 4);
			dp->ss.draught = draught * 0.1f;

			//Destination
			std::string destination;
			for (int i = 50; i != 71; ++i)
			{
				long long temp{ numberCodingTable[msg[i]] & 0xf };
				temp <<= 2;
				temp |= (numberCodingTable[msg[i + 1]] >> 4);
				destination += charactorCodingTable[temp];
			}
			dp->ss.destination.append(destination);

			//DTE
			dp->ss.dte = (numberCodingTable[msg[70]] & 0x8);
			//Spare
			dp->ss.spare = (numberCodingTable[msg[70]] & 0x4);

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}

		void AISDataDecoder::parseMessage9(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->sar.repeatindicator = (numberCodingTable[msg[1]] >> 4);
			
			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->sar.mmsi.append((boost::format("%ld") % userID).str());

			//Altitude
			short altitude{ numberCodingTable[msg[6]] & 0xf };
			altitude <<= 6;
			altitude |= numberCodingTable[msg[7]];
			altitude <<= 2;
			altitude |= (numberCodingTable[msg[7]] >> 4);
			dp->sar.altitude = altitude;

			//SOG
			short sog{ numberCodingTable[msg[8]] & 0xf };
			sog <<= 6;
			sog |= numberCodingTable[msg[9]];
			dp->sar.sog = sog * 1.0f;

			//Position accuracy
			dp->sar.positionaccuracy = (numberCodingTable[msg[10]] >> 5);

			//Longitude
			int longitude{ numberCodingTable[msg[10]] & 0x1f };
			longitude <<= 6;
			longitude |= numberCodingTable[msg[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[13]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[14]];
			longitude >>= 1;
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			dp->sar.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[msg[14]] & 0x01 };
			latitude <<= 6;
			latitude |= numberCodingTable[msg[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[17]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[18]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[19]];
			latitude >>= 4;
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			dp->sar.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//COG
			int cog{ numberCodingTable[msg[19]] & 0x0f };
			cog <<= 6;
			cog |= numberCodingTable[msg[20]];
			cog <<= 6;
			cog |= numberCodingTable[msg[21]];
			cog >>= 4;
			dp->sar.cog = cog / 10.0f;

			//Timestamp
			char timestamp{ numberCodingTable[msg[21]] & 0x0f };
			timestamp <<= 2;
			timestamp |= (numberCodingTable[msg[22]] >> 4);
			dp->sar.timestamp = timestamp;

			//Altitude Sensor
			dp->sar.altitude = (numberCodingTable[msg[22]] & 0x8);

			//DTE
			char dte{ numberCodingTable[msg[23]] & 0x2 };
			dte >>= 1;

			//Assign mode flag
			char assignModeFlag{ numberCodingTable[msg[24]] & 0x8 };
			assignModeFlag >>= 3;
			dp->sar.assignmodeflag = assignModeFlag;

			//RAIM
			char raimFlag{ numberCodingTable[msg[24]] & 0x4 };
			raimFlag >>= 2;
			dp->sar.raimflag = raimFlag;

			//Comm. state selector flag
			char commStateSelectorFlag{ numberCodingTable[msg[24]] & 0x2 };
			commStateSelectorFlag >>= 1;
			dp->sar.commstateselectorflag = commStateSelectorFlag;

			//Comm. State
			int communicationState{ numberCodingTable[msg[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[27]];
			dp->sar.communicationstate = communicationState;

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}

		void AISDataDecoder::parseMessage18(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->pb.repeatindicator = (numberCodingTable[msg[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->pb.mmsi.append((boost::format("%ld") % userID).str());

			//Spare
			char spare{ numberCodingTable[msg[6]] & 0xf };
			spare <<= 4;
			spare |= (numberCodingTable[msg[7]] >> 2);
			dp->pb.spare1 = spare;

			//SOG
			short temp{ numberCodingTable[msg[7]] & 0x3 };
			temp <<= 6;
			temp |= numberCodingTable[msg[8]];
			temp <<= 2;
			temp |= (numberCodingTable[msg[9]] >> 4);
			dp->pb.sog = temp / 10.0f;

			//Position accuracy
			dp->pb.positionaccuracy = ((numberCodingTable[msg[9]] & 0x8) >> 3);

			//Longitude
			int longitude{ numberCodingTable[msg[9]] & 0x7 };
			longitude <<= 6;
			longitude |= numberCodingTable[msg[10]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[13]];
			longitude <<= 1;
			longitude |= (numberCodingTable[msg[14]] >> 5);
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			dp->pb.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			int latitude{ numberCodingTable[msg[14]] & 0x1f };
			latitude <<= 6;
			latitude |= numberCodingTable[msg[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[17]];
			latitude <<= 4;
			latitude |= (numberCodingTable[msg[18]] >> 2);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			dp->pb.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			int cog{ numberCodingTable[msg[18]] & 0x3 };
			cog <<= 6;
			cog |= numberCodingTable[msg[19]];
			cog <<= 4;
			cog |= (numberCodingTable[msg[20]] >> 2);
			dp->pb.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[msg[20]] & 0x3 };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[msg[21]];
			trueHeading <<= 1;
			trueHeading |= (numberCodingTable[msg[22]] >> 5);
			dp->pb.trueheading = trueHeading;

			char timestamp{ numberCodingTable[msg[22]] & 0x1f };
			timestamp <<= 1;
			timestamp |= (numberCodingTable[msg[23]] >> 5);
			dp->pb.timestamp = timestamp;

			//Class B unit flag
			dp->pb.bunitflag = ((numberCodingTable[msg[23]] >> 2) & 0x1);
			//Class B display flag
			dp->pb.bdisplayflag = ((numberCodingTable[msg[23]] >> 1) & 0x1);
			//Class B DSC flag
			dp->pb.bdscflag = (numberCodingTable[msg[23]] & 0x1);
			//Class B band flag
			dp->pb.bbandflag = (numberCodingTable[msg[24]] >> 5);
			//Class B message 22 flag
			dp->pb.bmessage22flag = ((numberCodingTable[msg[24]] >> 4) & 0x1);
			//Mode flag
			dp->pb.modeflag = ((numberCodingTable[msg[24]] >> 3) & 0x1);
			//RAIM flag
			dp->pb.raimflag = ((numberCodingTable[msg[24]] >> 2) & 0x1);
			//Comm. state selector flag
			dp->pb.commstateselectorflag = ((numberCodingTable[msg[24]] >> 1) & 0x1);

			int communicationState{ numberCodingTable[msg[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[msg[27]];

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}

		void AISDataDecoder::parseMessage19(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->pb.repeatindicator = (numberCodingTable[msg[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->pb.mmsi.append((boost::format("%ld") % userID).str());

			//SOG
			short sog{ numberCodingTable[msg[7]] & 0x3 };
			sog <<= 6;
			sog |= numberCodingTable[msg[8]];
			sog <<= 2;
			sog |= (numberCodingTable[msg[9]] >> 4);
			dp->pb.sog = sog * 0.1f;

			//Position accuracy
			dp->pb.positionaccuracy = ((numberCodingTable[msg[9]] >> 3) & 0x1);

			//Longitude
			int longitude{ numberCodingTable[msg[9]] & 0x7 };
			longitude <<= 6;
			longitude |= numberCodingTable[msg[10]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[13]];
			longitude <<= 1;
			longitude |= (numberCodingTable[msg[14]] >> 5);
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			dp->pb.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[msg[14]] & 0x3f };
			latitude <<= 6;
			latitude |= numberCodingTable[msg[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[17]];
			latitude <<= 4;
			latitude |= (numberCodingTable[msg[18]] >> 2);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			dp->pb.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//COG
			int cog{ numberCodingTable[msg[18]] & 0x3 };
			cog <<= 6;
			cog |= numberCodingTable[msg[19]];
			cog <<= 4;
			cog |= (numberCodingTable[msg[20]] >> 2);
			dp->pb.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[msg[20]] & 0x3 };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[msg[21]];
			trueHeading <<= 1;
			trueHeading |= (numberCodingTable[msg[22]] >> 5);
			dp->pb.trueheading = trueHeading;

			char timestamp{ numberCodingTable[msg[22]] & 0x1f };
			timestamp <<= 1;
			timestamp |= (numberCodingTable[msg[23]] >> 5);
			dp->pb.timestamp = timestamp;

			//Name
			std::string name;
			for (int i = 23; i != 43; ++i)
			{
				long long temp{ numberCodingTable[msg[i]] & 0x1 };
				temp <<= 5;
				temp |= (numberCodingTable[msg[i + 1]] >> 1);
				name += charactorCodingTable[temp];
			}

			//Type of ship & cargo
			char shipAndCargoType{ numberCodingTable[msg[43]] & 0x1 };
			shipAndCargoType <<= 6;
			shipAndCargoType |= numberCodingTable[msg[44]];
			shipAndCargoType <<= 1;
			shipAndCargoType |= (numberCodingTable[msg[45]] >> 5);

			//Ship dimension
			int shipDimension{ numberCodingTable[msg[45]] & 0x1f };
			for (int i = 46; i != 50; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[msg[i]];
			}
			shipDimension <<= 1;
			shipDimension |= (numberCodingTable[msg[50]] >> 5);
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };

			//Type of electronic position
			char electronicType{ (numberCodingTable[msg[50]] >> 1) & 0xf };
			//RAIM
			char raimFlag{ numberCodingTable[msg[50]] & 0x1 };
			//DTE
			char dte{ numberCodingTable[msg[51]] >> 5 };
			//Mode flag
			char modeFlag{ (numberCodingTable[msg[51]] >> 4) & 0x1 };
			//Spare
			char spare{ numberCodingTable[msg[51]] & 0xf };

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}

		void AISDataDecoder::parseMessage21(const char* msg /* = nullptr */)
		{
			boost::shared_ptr<AISData> dp{ boost::make_shared<AISData>() };
			//MessageID
			//0~5bit
			dp->msgID = numberCodingTable[msg[0]];

			//Repeat Indicator
			//6~7bit
			dp->nav.repeatindicator = (numberCodingTable[msg[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[msg[1]] };
			userID <<= 6;
			userID |= numberCodingTable[msg[2]];
			userID <<= 6;
			userID |= numberCodingTable[msg[3]];
			userID <<= 6;
			userID |= numberCodingTable[msg[4]];
			userID <<= 6;
			userID |= numberCodingTable[msg[5]];
			userID <<= 6;
			userID |= numberCodingTable[msg[6]];
			userID <<= 2;
			userID >>= 6;
			dp->nav.mmsi.append((boost::format("%ld") % userID).str());

			//Navigation Type
			short naviType{ numberCodingTable[msg[6]] & 0xf };
			naviType << 1;
			naviType |= (numberCodingTable[msg[7]] >> 5);
			dp->nav.navigationtype = naviType;

			//Name
			std::string name;
			for (int i = 7; i != 27; ++i)
			{
				long long temp{ numberCodingTable[msg[i]] & 0x1f };
				temp <<= 1;
				temp |= (numberCodingTable[msg[i + 1]] >> 5);
				name += charactorCodingTable[temp];
			}
			dp->nav.name.append(name);

			//Position Accuracy
			dp->nav.positionaccuracy = ((numberCodingTable[msg[27]] >> 4) & 0x01);

			//Longitude
			int longitude{ numberCodingTable[msg[27]] & 0xf };
			longitude <<= 6;
			longitude |= numberCodingTable[msg[28]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[29]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[30]];
			longitude <<= 6;
			longitude |= numberCodingTable[msg[31]];
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			dp->nav.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[msg[32]] };
			latitude <<= 6;
			latitude |= numberCodingTable[msg[33]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[34]];
			latitude <<= 6;
			latitude |= numberCodingTable[msg[35]];
			latitude <<= 3;
			latitude |= (numberCodingTable[msg[36]] >> 3);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			dp->nav.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//Ship dimension
			int shipDimension{ numberCodingTable[msg[36]] & 0x7 };
			for (int i = 36; i != 41; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[msg[i]];
			}
			shipDimension <<= 3;
			shipDimension |= (numberCodingTable[msg[41]] >> 3);
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };
			dp->nav.dimensions.append((boost::format("A:%d B:%d C:%d D:%d") % aval % bval % cval % dval).str());

			//EPFD Type
			char epfdType{ numberCodingTable[msg[41]] & 0x7 };
			epfdType <<= 1;
			epfdType |= (numberCodingTable[msg[42]] >> 5);
			dp->nav.epfd = epfdType;

			//Timestamp
			char timestamp{ (numberCodingTable[msg[42]] << 1) & 0x3f };
			timestamp |= ((numberCodingTable[msg[43]] >> 5) & 0x1);
			dp->nav.utctimestamp = timestamp;

			//On/Off Position Indicator
			dp->nav.onoffpositionindicator = ((numberCodingTable[msg[43]] >> 4) & 0x1);

			//AtoN Reg app
			char atonFlag{ numberCodingTable[msg[43]] & 0xf };
			atonFlag <<= 4;
			atonFlag |= (numberCodingTable[msg[44]] >> 2);
			dp->nav.atonflag = atonFlag;

			//RAIM status
			dp->nav.raimflag = ((numberCodingTable[msg[44]] >> 1) & 0x1);
			//Virtual AtoN Status
			dp->nav.virtualflag = (numberCodingTable[msg[44]] & 0x1);
			//Mode Indicator
			dp->nav.modeindicator = ((numberCodingTable[msg[45]] >> 5) & 0x1);

			if (dataNotificationCallback)
			{
				dataNotificationCallback(dp);
			}
		}
	}//namespace data
}//namespace framework
