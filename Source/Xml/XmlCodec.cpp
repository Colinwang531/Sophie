#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
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
					const std::string filePath{
						boost::filesystem::initial_path<boost::filesystem::path>().string() };
#ifdef WINDOWS
					const std::string fullPath{ (boost::format("%s\\%s") % filePath % fileName).str() };
#else
					const std::string fullPath{ (boost::format("%s/%s") % filePath % fileName).str() };
#endif//WINDOWS
					boost::property_tree::read_xml(fullPath, pt);
					valueStr = pt.get<std::string>(fieldName);
					boost::property_tree::write_xml(fullPath, pt);
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
					const std::string filePath{
						boost::filesystem::initial_path<boost::filesystem::path>().string() };
#ifdef WINDOWS
					const std::string fullPath{ (boost::format("%s\\%s") % filePath % fileName).str() };
#else
					const std::string fullPath{ (boost::format("%s/%s") % filePath % fileName).str() };
#endif//WINDOWS
					boost::property_tree::read_xml(fullPath, pt);
					pt.put(fieldName, valueStr);
					boost::property_tree::write_xml(fullPath, pt);
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
