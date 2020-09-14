#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "Error.h"
#include "Xml/XmlCodec.h"

namespace base
{
	namespace xml
	{
		XMLParser::XMLParser(){}
		XMLParser::~XMLParser(){}

		int XMLParser::getValueByName(
			const std::string fileName, 
			const std::string fieldName, 
			std::string& valueStr)
		{
			int e{ 
				!fileName.empty() && !fieldName.empty() ? eSuccess : eInvalidParameter };
			boost::property_tree::ptree pt;

			if (eSuccess == e)
			{
				try
				{
					boost::property_tree::read_xml(fileName, pt);
					valueStr = pt.get<std::string>(fieldName);
					boost::property_tree::write_xml(fileName, pt);
				}
				catch (const std::exception&)
				{
					valueStr.clear();
					e = eBadOpenFile;
				}
			}

			return e;
		}

		XMLPacker::XMLPacker(){}
		XMLPacker::~XMLPacker(){}

		int XMLPacker::setValueWithName(
			const std::string fileName, 
			const std::string fieldName, 
			const std::string valueStr)
		{
			int e{
				!fileName.empty() && !fieldName.empty() && !valueStr.empty() ? eSuccess : eInvalidParameter };
			boost::property_tree::ptree pt;

			if (eSuccess == e)
			{
				try
				{
					boost::property_tree::read_xml(fileName, pt);
					pt.put(fieldName, valueStr);
					boost::property_tree::write_xml(fileName, pt);
				}
				catch (const std::exception&)
				{
					e = eBadWriteData;
				}
			}

			return e;
		}
	}//namespace xml
}//namespace base
