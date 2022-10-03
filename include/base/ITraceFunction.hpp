#ifndef ITRACEFUNCTION_HPP_
#define ITRACEFUNCTION_HPP_

#include "Config.hpp"

#include <string>

namespace Base
{
	/**
	 *
	 */
	class ITraceFunction
	{
		public:
			virtual ~ITraceFunction(){}
			/**
			 *
			 */
			virtual void trace(	const std::string& aText) = 0;
	}; // class ITraceFunction
} // namespace Base

#endif // SRC_ITRACEFUNCTION_HPP_ 
