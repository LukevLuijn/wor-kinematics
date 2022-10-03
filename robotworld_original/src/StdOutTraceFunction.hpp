#ifndef STDOUTTRACEFUNCTION_HPP_
#define STDOUTTRACEFUNCTION_HPP_

#include "Config.hpp"

#include "ITraceFunction.hpp"

namespace Base
{
	/**
	 *
	 */
	class StdOutTraceFunction : public ITraceFunction
	{
		public:
			/**
			 *
			 * @param aText The text that will be send to the final trace destination.
			 */
			virtual void trace( const std::string& aText)  override;

	};
	// class StdOutDebugTraceFunction
} // namespace Base
#endif // STDOUTTRACEFUNCTION_HPP_
