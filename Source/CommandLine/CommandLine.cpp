#include "Error.h"
#include "CommandLine/CommandLine.h"

namespace base
{
	namespace commandline
	{
		CommandLine::CommandLine(){}
		CommandLine::~CommandLine(){}

		int CommandLine::setCommandOptions(const char* option /* = nullptr */, const char* param /* = nullptr */)
		{
			int e{ option && param ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				desc.add_options()(option, boost::program_options::value<std::string>(), param);
			}

			return e;
		}

		const char* CommandLine::getParameterByOption(const char* option /* = nullptr */)
		{
			char* param{ nullptr };

			if (option && 0 < variables.count(option))
			{
				param = const_cast<char*>(variables[option].as<std::string>().c_str());
			}

			return param;
		}

		int CommandLine::parseCommandLine(int argc, char** argv)
		{
			int e{ 0 < argc && argv ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables);
			}

			return e;
		}
	}//namespace commandline
}//namespace base
