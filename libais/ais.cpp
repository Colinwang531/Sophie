#include <unordered_map>
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/functional/factory.hpp"
#include "libcommon/error.h"
#include "ais.h"

namespace framework
{
	namespace libais
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

		class IAISParser
		{
		public:
			IAISParser(void);
			~IAISParser(void);

		public:
			CommonError parse(const std::string data);
            inline const AISDataType getDataType(void) const
			{
				return dataType;
			}
			inline const PositionA getPositionAInfo(void) const
			{
				return positionA;
			}
            inline const PositionB getPositionBInfo(void) const
			{
				return positionB;
			}
            inline const ShipStatic getShipStaticInfo(void) const
			{
				return shipStatic;
			}
            inline const StandardSAR getStandardSARInfo(void) const
			{
				return standardSAR;
			}
            inline const AidsToNavigation getAidsToNavigationInfo(void) const
			{
				return aidsToNav;
			}

		private:
			void parsePositionA(const std::string data);
			void parseShipStatic(const std::string data);
			void parseStandardSAR(const std::string data);
			void parsePositionB_18(const std::string data);
			void parsePositionB_19(const std::string data);
			void parseAidsToNav(const std::string data);

		private:
			AISDataType dataType;
			PositionA positionA;
			PositionB positionB;
			ShipStatic shipStatic;
			StandardSAR standardSAR;
			AidsToNavigation aidsToNav;
		};//class IAISParser

		IAISParser::IAISParser() : dataType{AISDataType::AIS_DATA_TYPE_NONE}
		{}
		IAISParser::~IAISParser(void)
		{}

		CommonError IAISParser::parse(const std::string data)
		{
			CommonError e{ CommonError::COMMON_ERROR_SUCCESS };
			const char messageID{ numberCodingTable[data[0]] };

			if (1 == messageID || 2 == messageID || 3 == messageID)
			{
				parsePositionA(data);
			} 
			else if (5 == messageID)
			{
				parseShipStatic(data);
			}
			else if (9 == messageID)
			{
				parseStandardSAR(data);
			}
			else if (18 == messageID)
			{ 
				parsePositionB_18(data);
			}
			else if (19 == messageID)
			{
				parsePositionB_19(data);
			}
			else if (21 == messageID)
			{
				parseAidsToNav(data);
			}
			else
			{
				e = CommonError::COMMON_ERROR_NOT_SUPPORT;
			}

			return e;
		}

		void IAISParser::parsePositionA(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_POSITION_A;

			//Repeat Indicator
			//6~7bit
			positionA.repeatindicator = (numberCodingTable[data[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			positionA.mmsi.append((boost::format("%ld") % userID).str());

			//Navigation Status
			//38~41
			positionA.navigationalstatus = (numberCodingTable[data[6]] & 0xf);

			//Rate of turn
			//42~49
			short rot{ numberCodingTable[data[7]] };
			rot <<= 6;
			rot |= numberCodingTable[data[8]];
			//rot >>= 4;
			positionA.rot = rot;

			//SOG
			short temp{ numberCodingTable[data[8]] };
			temp <<= 6;
			temp |= numberCodingTable[data[9]];
			temp &= 0x3ff;
			positionA.sog = temp / 10.0f;

			//Position accuracy
			positionA.positionaccuracy = (numberCodingTable[data[10]] >> 5);

			//Longitude
			int longitude{ numberCodingTable[data[10]] & 0x1f };
			longitude <<= 6;
			longitude |= numberCodingTable[data[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[13]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[14]];
			longitude >>= 1;
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			positionA.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			int latitude{ numberCodingTable[data[14]] & 0x01 };
			latitude <<= 6;
			latitude |= numberCodingTable[data[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[17]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[18]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[19]];
			latitude >>= 4;
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			positionA.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			int cog{ numberCodingTable[data[19]] & 0x0f };
			cog <<= 6;
			cog |= numberCodingTable[data[20]];
			cog <<= 6;
			cog |= numberCodingTable[data[21]];
			cog >>= 4;
			positionA.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[data[21]] & 0xf };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[data[22]];
			trueHeading >>= 1;
			positionA.trueheading = trueHeading;

			char timestamp{ numberCodingTable[data[22]] & 0x1 };
			timestamp <<= 6;
			timestamp |= numberCodingTable[data[23]];
			timestamp >>= 1;
			positionA.timestamp = timestamp;

			char regionalApplication{ numberCodingTable[data[23]] & 0x1 };
			regionalApplication <<= 6;
			regionalApplication |= numberCodingTable[data[24]];
			regionalApplication >>= 3;

			char spare{ numberCodingTable[data[24]] & 0x4 };
			spare >>= 2;
			positionA.spare = spare;

			char raimFlag{ numberCodingTable[data[24]] & 0x2 };
			spare >>= 1;

			int communicationState{ numberCodingTable[data[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[27]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[28]];
			communicationState <<= 1;
		}

		void IAISParser::parseShipStatic(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_SHIP_STATIC;

			//Repeat Indicator
			//6~7bit
			shipStatic.repeatindicator = (numberCodingTable[data[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			shipStatic.mmsi.append((boost::format("%ld") % userID).str());

			//AIS Version Indicator
			char aisVersionIndicator{ numberCodingTable[data[6]] & 0xc };
			aisVersionIndicator >>= 2;
			shipStatic.aisversion = aisVersionIndicator;

			//IMO Number
			long long imoNumber{ numberCodingTable[data[6]] & 0x3 };
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[data[7]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[data[8]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[data[9]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[data[10]];
			imoNumber <<= 6;
			imoNumber |= numberCodingTable[data[11]];
			imoNumber <<= 4;
			imoNumber >>= 6;
			shipStatic.imo = imoNumber;

			//Call Sign
			long long callSign{ numberCodingTable[data[11]] & 0x3 };
			callSign <<= 6;
			callSign |= numberCodingTable[data[12]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[13]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[14]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[15]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[16]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[17]];
			callSign <<= 6;
			callSign |= numberCodingTable[data[18]];
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
			shipStatic.callsign.append(callSign_);

			//Name
			std::string name;
			for (int i = 18; i != 38; ++i)
			{
				long long temp{ numberCodingTable[data[i]] & 0x3 };
				temp <<= 4;
				temp |= (numberCodingTable[data[i + 1]] >> 2);
				name += charactorCodingTable[temp];
			}
			shipStatic.name.append(name);

			//Type of ship & cargo
			char shipAndCargoType{ numberCodingTable[data[38]] & 0x3 };
			shipAndCargoType <<= 6;
			shipAndCargoType |= numberCodingTable[data[39]];
			shipStatic.typeofshipcargo = shipAndCargoType;

			//Ship dimension
			int shipDimension{ numberCodingTable[data[40]] };
			for (int i = 41; i != 45; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[data[i]];
			}
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };
			shipStatic.dimensions.append((boost::format("A:%d B:%d C:%d D:%d") % aval % bval % cval % dval).str());

			//Type of electronic position
			shipStatic.epfd = (numberCodingTable[data[45]] >> 2);

			//ETA
			int eta{ numberCodingTable[data[45]] & 0x3 };
			for (int i = 46; i != 49; ++i)
			{
				eta <<= 6;
				eta |= numberCodingTable[data[i]];
			}
			shipStatic.minute = (eta & 0x3f);
			shipStatic.hour = ((eta >> 6) & 0x1f);
			shipStatic.day = ((eta >> 11) & 0x1f);
			shipStatic.month = ((eta >> 16) & 0xf);

			//Max present static draught
			short draught{ numberCodingTable[data[49]] };
			draught <<= 2;
			draught |= (numberCodingTable[data[50]] >> 4);
			shipStatic.draught = draught * 0.1f;

			//Destination
			std::string destination;
			for (int i = 50; i != 71; ++i)
			{
				long long temp{ numberCodingTable[data[i]] & 0xf };
				temp <<= 2;
				temp |= (numberCodingTable[data[i + 1]] >> 4);
				destination += charactorCodingTable[temp];
			}
			shipStatic.destination.append(destination);

			//DTE
			shipStatic.dte = (numberCodingTable[data[70]] & 0x8);
			//Spare
			shipStatic.spare = (numberCodingTable[data[70]] & 0x4);
		}

		void IAISParser::parseStandardSAR(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_STANDARD_SAR;

			//Repeat Indicator
			//6~7bit
			standardSAR.repeatindicator = (numberCodingTable[data[1]] >> 4);
			
			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			standardSAR.mmsi.append((boost::format("%ld") % userID).str());

			//Altitude
			short altitude{ numberCodingTable[data[6]] & 0xf };
			altitude <<= 6;
			altitude |= numberCodingTable[data[7]];
			altitude <<= 2;
			altitude |= (numberCodingTable[data[7]] >> 4);
			standardSAR.altitude = altitude;

			//SOG
			short sog{ numberCodingTable[data[8]] & 0xf };
			sog <<= 6;
			sog |= numberCodingTable[data[9]];
			standardSAR.sog = sog * 1.0f;

			//Position accuracy
			standardSAR.positionaccuracy = (numberCodingTable[data[10]] >> 5);

			//Longitude
			int longitude{ numberCodingTable[data[10]] & 0x1f };
			longitude <<= 6;
			longitude |= numberCodingTable[data[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[13]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[14]];
			longitude >>= 1;
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			standardSAR.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[data[14]] & 0x01 };
			latitude <<= 6;
			latitude |= numberCodingTable[data[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[17]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[18]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[19]];
			latitude >>= 4;
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			standardSAR.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//COG
			int cog{ numberCodingTable[data[19]] & 0x0f };
			cog <<= 6;
			cog |= numberCodingTable[data[20]];
			cog <<= 6;
			cog |= numberCodingTable[data[21]];
			cog >>= 4;
			standardSAR.cog = cog / 10.0f;

			//Timestamp
			char timestamp{ numberCodingTable[data[21]] & 0x0f };
			timestamp <<= 2;
			timestamp |= (numberCodingTable[data[22]] >> 4);
			standardSAR.timestamp = timestamp;

			//Altitude Sensor
			standardSAR.altitude = (numberCodingTable[data[22]] & 0x8);

			//DTE
			char dte{ numberCodingTable[data[23]] & 0x2 };
			dte >>= 1;

			//Assign mode flag
			char assignModeFlag{ numberCodingTable[data[24]] & 0x8 };
			assignModeFlag >>= 3;
			standardSAR.assignmodeflag = assignModeFlag;

			//RAIM
			char raimFlag{ numberCodingTable[data[24]] & 0x4 };
			raimFlag >>= 2;
			standardSAR.raimflag = raimFlag;

			//Comm. state selector flag
			char commStateSelectorFlag{ numberCodingTable[data[24]] & 0x2 };
			commStateSelectorFlag >>= 1;
			standardSAR.commstateselectorflag = commStateSelectorFlag;

			//Comm. State
			int communicationState{ numberCodingTable[data[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[27]];
			standardSAR.communicationstate = communicationState;
		}

		void IAISParser::parsePositionB_18(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_POSITION_B;

			//Repeat Indicator
			//6~7bit
			positionB.repeatindicator = (numberCodingTable[data[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			positionB.mmsi.append((boost::format("%ld") % userID).str());

			//Spare
			char spare{ numberCodingTable[data[6]] & 0xf };
			spare <<= 4;
			spare |= (numberCodingTable[data[7]] >> 2);
			positionB.spare1 = spare;

			//SOG
			short temp{ numberCodingTable[data[7]] & 0x3 };
			temp <<= 6;
			temp |= numberCodingTable[data[8]];
			temp <<= 2;
			temp |= (numberCodingTable[data[9]] >> 4);
			positionB.sog = temp / 10.0f;

			//Position accuracy
			positionB.positionaccuracy = ((numberCodingTable[data[9]] & 0x8) >> 3);

			//Longitude
			int longitude{ numberCodingTable[data[9]] & 0x7 };
			longitude <<= 6;
			longitude |= numberCodingTable[data[10]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[13]];
			longitude <<= 1;
			longitude |= (numberCodingTable[data[14]] >> 5);
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			positionB.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			int latitude{ numberCodingTable[data[14]] & 0x1f };
			latitude <<= 6;
			latitude |= numberCodingTable[data[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[17]];
			latitude <<= 4;
			latitude |= (numberCodingTable[data[18]] >> 2);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			positionB.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			int cog{ numberCodingTable[data[18]] & 0x3 };
			cog <<= 6;
			cog |= numberCodingTable[data[19]];
			cog <<= 4;
			cog |= (numberCodingTable[data[20]] >> 2);
			positionB.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[data[20]] & 0x3 };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[data[21]];
			trueHeading <<= 1;
			trueHeading |= (numberCodingTable[data[22]] >> 5);
			positionB.trueheading = trueHeading;

			char timestamp{ numberCodingTable[data[22]] & 0x1f };
			timestamp <<= 1;
			timestamp |= (numberCodingTable[data[23]] >> 5);
			positionB.timestamp = timestamp;

			//Class B unit flag
			positionB.bunitflag = ((numberCodingTable[data[23]] >> 2) & 0x1);
			//Class B display flag
			positionB.bdisplayflag = ((numberCodingTable[data[23]] >> 1) & 0x1);
			//Class B DSC flag
			positionB.bdscflag = (numberCodingTable[data[23]] & 0x1);
			//Class B band flag
			positionB.bbandflag = (numberCodingTable[data[24]] >> 5);
			//Class B message 22 flag
			positionB.bmessage22flag = ((numberCodingTable[data[24]] >> 4) & 0x1);
			//Mode flag
			positionB.modeflag = ((numberCodingTable[data[24]] >> 3) & 0x1);
			//RAIM flag
			positionB.raimflag = ((numberCodingTable[data[24]] >> 2) & 0x1);
			//Comm. state selector flag
			positionB.commstateselectorflag = ((numberCodingTable[data[24]] >> 1) & 0x1);

			int communicationState{ numberCodingTable[data[24]] & 0x1 };
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[25]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[26]];
			communicationState <<= 6;
			communicationState |= numberCodingTable[data[27]];
		}

		void IAISParser::parsePositionB_19(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_POSITION_B;

			//Repeat Indicator
			//6~7bit
			positionB.repeatindicator = (numberCodingTable[data[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			positionB.mmsi.append((boost::format("%ld") % userID).str());

			//SOG
			short sog{ numberCodingTable[data[7]] & 0x3 };
			sog <<= 6;
			sog |= numberCodingTable[data[8]];
			sog <<= 2;
			sog |= (numberCodingTable[data[9]] >> 4);
			positionB.sog = sog * 0.1f;

			//Position accuracy
			positionB.positionaccuracy = ((numberCodingTable[data[9]] >> 3) & 0x1);

			//Longitude
			int longitude{ numberCodingTable[data[9]] & 0x7 };
			longitude <<= 6;
			longitude |= numberCodingTable[data[10]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[11]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[12]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[13]];
			longitude <<= 1;
			longitude |= (numberCodingTable[data[14]] >> 5);
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			positionB.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[data[14]] & 0x3f };
			latitude <<= 6;
			latitude |= numberCodingTable[data[15]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[16]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[17]];
			latitude <<= 4;
			latitude |= (numberCodingTable[data[18]] >> 2);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			positionB.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//COG
			int cog{ numberCodingTable[data[18]] & 0x3 };
			cog <<= 6;
			cog |= numberCodingTable[data[19]];
			cog <<= 4;
			cog |= (numberCodingTable[data[20]] >> 2);
			positionB.cog = cog / 10.0f;

			short trueHeading{ numberCodingTable[data[20]] & 0x3 };
			trueHeading <<= 6;
			trueHeading |= numberCodingTable[data[21]];
			trueHeading <<= 1;
			trueHeading |= (numberCodingTable[data[22]] >> 5);
			positionB.trueheading = trueHeading;

			char timestamp{ numberCodingTable[data[22]] & 0x1f };
			timestamp <<= 1;
			timestamp |= (numberCodingTable[data[23]] >> 5);
			positionB.timestamp = timestamp;

			//Name
			std::string name;
			for (int i = 23; i != 43; ++i)
			{
				long long temp{ numberCodingTable[data[i]] & 0x1 };
				temp <<= 5;
				temp |= (numberCodingTable[data[i + 1]] >> 1);
				name += charactorCodingTable[temp];
			}

			//Type of ship & cargo
			char shipAndCargoType{ numberCodingTable[data[43]] & 0x1 };
			shipAndCargoType <<= 6;
			shipAndCargoType |= numberCodingTable[data[44]];
			shipAndCargoType <<= 1;
			shipAndCargoType |= (numberCodingTable[data[45]] >> 5);

			//Ship dimension
			int shipDimension{ numberCodingTable[data[45]] & 0x1f };
			for (int i = 46; i != 50; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[data[i]];
			}
			shipDimension <<= 1;
			shipDimension |= (numberCodingTable[data[50]] >> 5);
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };

			//Type of electronic position
			char electronicType{ (numberCodingTable[data[50]] >> 1) & 0xf };
			//RAIM
			char raimFlag{ numberCodingTable[data[50]] & 0x1 };
			//DTE
			char dte{ numberCodingTable[data[51]] >> 5 };
			//Mode flag
			char modeFlag{ (numberCodingTable[data[51]] >> 4) & 0x1 };
			//Spare
			char spare{ numberCodingTable[data[51]] & 0xf };
		}

		void IAISParser::parseAidsToNav(const std::string data)
		{
			dataType = AISDataType::AIS_DATA_TYPE_AIDS_TO_NAVIGATION;

			//Repeat Indicator
			//6~7bit
			aidsToNav.repeatindicator = (numberCodingTable[data[1]] >> 4);

			//User ID
			//8~37
			long long userID{ numberCodingTable[data[1]] };
			userID <<= 6;
			userID |= numberCodingTable[data[2]];
			userID <<= 6;
			userID |= numberCodingTable[data[3]];
			userID <<= 6;
			userID |= numberCodingTable[data[4]];
			userID <<= 6;
			userID |= numberCodingTable[data[5]];
			userID <<= 6;
			userID |= numberCodingTable[data[6]];
			userID <<= 2;
			userID >>= 6;
			aidsToNav.mmsi.append((boost::format("%ld") % userID).str());

			//Navigation Type
			short naviType{ numberCodingTable[data[6]] & 0xf };
			naviType << 1;
			naviType |= (numberCodingTable[data[7]] >> 5);
			aidsToNav.navigationtype = naviType;

			//Name
			std::string name;
			for (int i = 7; i != 27; ++i)
			{
				long long temp{ numberCodingTable[data[i]] & 0x1f };
				temp <<= 1;
				temp |= (numberCodingTable[data[i + 1]] >> 5);
				name += charactorCodingTable[temp];
			}
			aidsToNav.name.append(name);

			//Position Accuracy
			aidsToNav.positionaccuracy = ((numberCodingTable[data[27]] >> 4) & 0x01);

			//Longitude
			int longitude{ numberCodingTable[data[27]] & 0xf };
			longitude <<= 6;
			longitude |= numberCodingTable[data[28]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[29]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[30]];
			longitude <<= 6;
			longitude |= numberCodingTable[data[31]];
			char longitudeFlag{ 0 == (longitude & 0x8000000) ? 'E' : 'W' };
			aidsToNav.longitude.append((boost::format("%f%s") % (longitude / 600000.0f) % longitudeFlag).str());

			//Latitude
			int latitude{ numberCodingTable[data[32]] };
			latitude <<= 6;
			latitude |= numberCodingTable[data[33]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[34]];
			latitude <<= 6;
			latitude |= numberCodingTable[data[35]];
			latitude <<= 3;
			latitude |= (numberCodingTable[data[36]] >> 3);
			char latitudeFlag{ 0 == (latitude & 0x4000000) ? 'N' : 'S' };
			aidsToNav.latitude.append((boost::format("%f%s") % (latitude / 600000.0f) % latitudeFlag).str());

			//Ship dimension
			int shipDimension{ numberCodingTable[data[36]] & 0x7 };
			for (int i = 36; i != 41; ++i)
			{
				shipDimension <<= 6;
				shipDimension |= numberCodingTable[data[i]];
			}
			shipDimension <<= 3;
			shipDimension |= (numberCodingTable[data[41]] >> 3);
			short dval{ shipDimension & 0x3f }, cval{ (shipDimension >> 6) & 0x3f }, bval{ (shipDimension >> 12) & 0x1ff }, aval{ (shipDimension >> 21) & 0x1ff };
			aidsToNav.dimensions.append((boost::format("A:%d B:%d C:%d D:%d") % aval % bval % cval % dval).str());

			//EPFD Type
			char epfdType{ numberCodingTable[data[41]] & 0x7 };
			epfdType <<= 1;
			epfdType |= (numberCodingTable[data[42]] >> 5);
			aidsToNav.epfd = epfdType;

			//Timestamp
			char timestamp{ (numberCodingTable[data[42]] << 1) & 0x3f };
			timestamp |= ((numberCodingTable[data[43]] >> 5) & 0x1);
			aidsToNav.utctimestamp = timestamp;

			//On/Off Position Indicator
			aidsToNav.onoffpositionindicator = ((numberCodingTable[data[43]] >> 4) & 0x1);

			//AtoN Reg app
			char atonFlag{ numberCodingTable[data[43]] & 0xf };
			atonFlag <<= 4;
			atonFlag |= (numberCodingTable[data[44]] >> 2);
			aidsToNav.atonflag = atonFlag;

			//RAIM status
			aidsToNav.raimflag = ((numberCodingTable[data[44]] >> 1) & 0x1);
			//Virtual AtoN Status
			aidsToNav.virtualflag = (numberCodingTable[data[44]] & 0x1);
			//Mode Indicator
			aidsToNav.modeindicator = ((numberCodingTable[data[45]] >> 5) & 0x1);
		}

		AISParser::AISParser() : parser{boost::factory<IAISParser*>()()}
		{}
		AISParser::~AISParser(void)
		{
			boost::checked_delete(parser);			
		}

		int AISParser::parse(const std::string data)
		{
			CommonError e{
				!data.empty() ? 
				CommonError::COMMON_ERROR_SUCCESS : 
				CommonError::COMMON_ERROR_INVALID_PARAMETER};

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				e = (parser ? parser->parse(data) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}
			
			return static_cast<int>(e);
		}

		const AISDataType AISParser::getDataType() const
		{
			return parser ? parser->getDataType() : AISDataType::AIS_DATA_TYPE_NONE;
		}

		const PositionA AISParser::getPositionAInfo() const
		{
			PositionA nullPositionA;
			return parser ? parser->getPositionAInfo() : nullPositionA;
		}

		const PositionB AISParser::getPositionBInfo() const
		{
			PositionB nullPositionB;
			return parser ? parser->getPositionBInfo() : nullPositionB;
		}

		const ShipStatic AISParser::getShipStaticInfo() const
		{
			ShipStatic nullShipStatic;
			return parser ? parser->getShipStaticInfo() : nullShipStatic;
		}

		const StandardSAR AISParser::getStandardSARInfo() const
		{
			StandardSAR nullStandardSAR;
			return parser ? parser->getStandardSARInfo() : nullStandardSAR;
		}

		const AidsToNavigation AISParser::getAidsToNavigationInfo() const
		{
			AidsToNavigation nullAidsToNav;
			return parser ? parser->getAidsToNavigationInfo() : nullAidsToNav;
		}
	}//namespace libais
}//namespace framework
