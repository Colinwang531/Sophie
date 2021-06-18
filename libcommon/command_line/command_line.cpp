#include "boost/checked_delete.hpp"
#include "boost/program_options.hpp"
#include "error.h"
#include "command_line.h"

namespace framework
{
	namespace libcommon
	{
		class ICommandLineParser
		{
		public:
			ICommandLineParser(void);
			~ICommandLineParser(void);

		public:
			CommonError setOption(
				const char* opt = nullptr, 
				const char* val = nullptr);
			const char* getValue(const char* opt = nullptr);
			CommonError parse(
				const int argc = 0, 
				const char** argv = nullptr);

		private:
			boost::program_options::options_description desc;
			boost::program_options::variables_map variables;
		};//class ICommandLineParser

		ICommandLineParser::ICommandLineParser()
		{}
		ICommandLineParser::~ICommandLineParser()
		{}

		CommonError ICommandLineParser::setOption(
			const char* option /* = nullptr */, 
			const char* param /* = nullptr */)
		{
			CommonError e{ 
				option && param ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				desc.add_options()(option, boost::program_options::value<std::string>(), param);
			}

			return e;
		}

		const char* ICommandLineParser::getValue(const char* option /* = nullptr */)
		{
			return 0 < variables.count(option) ? variables[option].as<std::string>().c_str() : nullptr;
		}

		CommonError ICommandLineParser::parse(
			const int argc/* = 0*/, 
			const char** argv/* = nullptr*/)
		{
			CommonError e{ 
				0 < argc && argv ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables);
			}

			return e;
		}

		CommandLineParser::CommandLineParser() : parser{new(std::nothrow) ICommandLineParser}
		{}
		CommandLineParser::~CommandLineParser()
		{
			boost::checked_delete(parser);		
		}

		int CommandLineParser::setOption(
			const char* option /* = nullptr */, 
			const char* param /* = nullptr */)
		{
			return static_cast<int>(parser ? parser->setOption(option, param) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
		}

		const char* CommandLineParser::getValue(const char* option /* = nullptr */)
		{
			return parser ? parser->getValue(option) : nullptr;
		}

		int CommandLineParser::parse(
			const int argc/* = 0*/, 
			const char** argv/* = nullptr*/)
		{
			return static_cast<int>(parser ? parser->parse(argc, argv) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
		}
	}//namespace libcommon
}//namespace framework
