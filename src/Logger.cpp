#include "Logger.hpp"

#include "ITraceFunction.hpp"
#include "Trace.hpp"

#include <filesystem>

namespace Application
{
	/* static */bool Logger::disable = false;
	/**
	 *
	 */
	/*static*/void Logger::log( const std::string& aMessage)
	{
		Base::Trace::trace(aMessage);
	}
    /*static*/ void Logger::log ( const std::string& aMessage, const std::string& func, const std::string& file, int lineNum)
    {
        std::string message = "[" + std::filesystem::path(file).filename().generic_string() + "@" + std::to_string(lineNum) + "](" + func + ") -> " + aMessage;
        log(message);
    }

} //namespace Application
