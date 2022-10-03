#ifndef TRACE_HPP_
#define TRACE_HPP_

#include "Config.hpp"

#include "ITraceFunction.hpp"

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace Base
{
	/**
	 * This Trace class can be used instead of just using std::cout.
	 * Most of the time one just uses the tracing MACROs below and not the class itself.
	 * The tracing is done through an installable trace function.
	 */
	class Trace
	{
		public:
			/**
			 *
			 */
			explicit Trace(	const std::string& aText,
							long aLineNumber    = 0,
							bool withTimeStamp  = false,
							bool aTraceCDtor 	= true);
			/**
			 *
			 */
			Trace(	const std::string& aText1,
					const std::string& aText2,
					long aLineNumber    = 0,
					bool withTimeStamp  = false,
					bool aTraceCDtor 	= true);
			/**
			 *
			 */
			~Trace();

			/**
			 *
			 */
			static void	enableTrace( bool enable = true)
			{
				traceOn = enable;
			}
			/**
			 *
			 */
			static bool isTraceEnabled()
			{
				return traceOn;
			}
			/**
			 *
			 */
			static void	enableTimeStamp( bool enable = true)
			{
				traceWithTimeStamp = enable;
			}
			/**
			 *
			 */
			static bool isTimeStampEnabled()
			{
				return traceWithTimeStamp;
			}
			/**
			 *
			 */
			static void enableLineNumber( bool enable = true)
			{
				traceLineNumber = enable;
			}
			/**
			 *
			 */
			static bool isLineNumberEnabled()
			{
				return traceLineNumber;
			}
			/**
			 *
			 */
			static void enableThreadId( bool enable = true)
			{
				traceThreadId = enable;
			}
			/**
			 *
			 */
			static bool isThreadIdEnabled()
			{
				return traceThreadId;
			}
			/**
			 *
			 */
			static void setTraceFunction( std::unique_ptr<ITraceFunction> aTraceFunction)
			{
				traceFunction = std::move(aTraceFunction);
			}
			/**
			 *
			 */
			static void trace(	const std::string& aText,
								bool withTimeStamp = Trace::isTimeStampEnabled());
			/**
			 *
			 */
			static void trace(	const std::string& aText,
			                  	const char* aFunctionName,
			                  	long aLineNumber,
			                  	bool withTimeStamp = Trace::isTimeStampEnabled());
		private:
			/**
			 *
			 */
			void traceConstructor(long aLineNumber) const;
			/**
			 *
			 */
			void traceDestructor() const;
			/**
			 *
			 */
			static void traceFormattedString(	const std::string& aText,
												char aTraceMarker);	// 	This text will be prepended

			/**
			 *
			 */
			static bool traceOn;
			/**
			 *
			 */
			static bool traceWithTimeStamp;
			/**
			 *
			 */
			static bool traceLineNumber;
			/**
			 *
			 */
			static bool traceThreadId;
			/**
			 *
			 */
			static bool	traceExceptions;

			/**
			 *
			 */
			std::string traceText;
			/**
			 *
			 */
			bool traceTimeStamp;
			/**
			 *
			 */
			bool traceCDtor;
			/**
			 *
			 */
			static std::unique_ptr<ITraceFunction> traceFunction;
			/**
			 *
			 */
			static std::mutex traceFunctionMutex;
	}; // class Trace

} /* namespace Base */

#ifdef DEVELOPMENT
/**
 * @name Tracing MACROS.
 *
 * To enable tracing the compile time macro DEVELOPMENT must be defined (e.g. -DDEVELOPMENT).
 * If DEVELOPMENT is not defined not tracing is possible.
 *
 */
//@{
/**
 * @def FUNCTRACE_DEVELOP
 * The constructor and destructor trace the function name. If this is the first thing upon function entry
 * the start and finish of the function are traced.
 */
#define FUNCTRACE_DEVELOP()                 	Base::Trace trc(__PRETTY_FUNCTION__, __LINE__)
/**
 * @def FUNCTRACE_TEXT_DEVELOP(a_text)
 * The constructor and destructor trace the function name, appended with a_text. If this is the first thing upon function entry
 * the start and finish of the function are traced.
 */
#define FUNCTRACE_TEXT_DEVELOP(a_text)   		Base::Trace trc(__PRETTY_FUNCTION__, a_text, __LINE__)
/**
 * \def TRACE_DEVELOP()
 * Traces the text.
 */
#define TRACE_DEVELOP(a_text)               	Base::Trace::trace(a_text)
/**
 * The TS-variants do the same as the non-TS-variants but prepend a time stamp to the output.
 */
#define TSFUNCTRACE_DEVELOP()               	Base::Trace trc(__PRETTY_FUNCTION__, __LINE__,	true)
#define TSFUNCTRACE_TEXT_DEVELOP(a_text)   		Base::Trace trc( __PRETTY_FUNCTION__, a_text ,	__LINE__,	true)
#define TSTRACE_DEVELOP(a_text)             	Base::Trace::trace(a_text, true)
//@}
#else // !DEVELOPMENT
#define FUNCTRACE_DEVELOP()
#define FUNCTRACE_TEXT_DEVELOP(a_text)
#define TRACE_DEVELOP(a_text)
#define TSFUNCTRACE_DEVELOP()
#define TSFUNCTRACE_TEXT_DEVELOP(a_text)
#define TSTRACE_DEVELOP(a_text)
#endif

#endif // TRACE_HPP_ 
