#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "libcommon/error.h"
#include "xml.h"

namespace framework
{
	namespace libcommon
	{
		XmlParser::XmlParser()
		{}
		XmlParser::~XmlParser()
		{}

		const std::string XmlParser::getValue(
			const std::string fileName, 
			const std::string keyName)
		{
			CommonError e{ 
				!fileName.empty() && !keyName.empty() ? 
				CommonError::COMMON_ERROR_SUCCESS : 
				CommonError::COMMON_ERROR_INVALID_PARAMETER };
			std::string value;

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				try
				{
					boost::property_tree::ptree pt;
					const std::string filePath{
						boost::filesystem::initial_path<boost::filesystem::path>().string() };
#ifdef WINDOWS
					const std::string fullPath{ (boost::format("%s\\%s") % filePath % fileName).str() };
#else
					const std::string fullPath{ (boost::format("%s/%s") % filePath % fileName).str() };
#endif//WINDOWS
					boost::property_tree::read_xml(fullPath, pt);
					value.append(pt.get<std::string>(keyName));
					boost::property_tree::write_xml(fullPath, pt);
				}
				catch (const std::exception&)
				{
					e = CommonError::COMMON_ERROR_BAD_FILE_OPEN;
				}
			}

			return value;
		}

		int XmlParser::setValue(
			const std::string fileName, 
			const std::string keyName, 
			const std::string value)
		{
			CommonError e{ 
				!fileName.empty() && !keyName.empty() ? 
				CommonError::COMMON_ERROR_SUCCESS : 
				CommonError::COMMON_ERROR_INVALID_PARAMETER };

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				try
				{
					boost::property_tree::ptree pt;
					const std::string filePath{
						boost::filesystem::initial_path<boost::filesystem::path>().string() };
#ifdef WINDOWS
					const std::string fullPath{ (boost::format("%s\\%s") % filePath % fileName).str() };
#else
					const std::string fullPath{ (boost::format("%s/%s") % filePath % fileName).str() };
#endif//WINDOWS
					boost::property_tree::read_xml(fullPath, pt);
					pt.put(keyName, value);
					boost::property_tree::write_xml(fullPath, pt);
				}
				catch (const std::exception&)
				{
					e = CommonError::COMMON_ERROR_BAD_FILE_OPEN;
				}
			}

			return static_cast<int>(e);
		}
	}//namespace xml
}//namespace base
