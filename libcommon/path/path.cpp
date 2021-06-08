#include "boost/filesystem/path.hpp" 
#include "boost/filesystem/operations.hpp"
#include "path/path.h"

namespace framework
{
	namespace libcommon
	{
		ProgramPathParser::ProgramPathParser()
		{}
		ProgramPathParser::~ProgramPathParser()
		{}

		const std::string ProgramPathParser::parseFileDirectory(const std::string fullPath)
		{
			int pos{-1};
			std::string path{
				fullPath.empty() ? boost::filesystem::initial_path<boost::filesystem::path>().string() : fullPath};

#ifdef WINDOWS
			pos = path.rfind('\\');
#else
			pos = path.rfind('/');
#endif//WINDOWS


			return -1 < pos ? path.substr(0, pos) : "";
		}

		const std::string ProgramPathParser::parseFileName(const std::string fullPath)
		{
			int pos{-1};
			std::string path{
				fullPath.empty() ? boost::filesystem::initial_path<boost::filesystem::path>().string() : fullPath};

#ifdef WINDOWS
			pos = path.rfind('\\');
#else
			pos = path.rfind('/');
#endif//WINDOWS


			return -1 < pos ? path.substr(pos + 1, path.length()) : "";
		}
	}//namespace libcommon
}//namespace framework
