#include "Logger.hpp"

#include "ITraceFunction.hpp"
#include "Trace.hpp"

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
} //namespace Application
