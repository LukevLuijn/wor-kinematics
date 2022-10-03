#include "Trace.hpp"

#include "StdOutTraceFunction.hpp"
#include "TimeStamp.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <thread>

namespace Base
{
	/**
	 * The number of spaces we indent
	 */
	const int INDENTIONLEVEL = 3;

	/* static */ bool Trace::traceOn = true;
	/* static */ bool Trace::traceWithTimeStamp = false;
	/* static */ bool Trace::traceLineNumber = true;
	/* static */ bool Trace::traceThreadId = true;
	/* static */ bool Trace::traceExceptions = true;

	/**
	 *
	 */
	/* static */ std::unique_ptr<ITraceFunction> Trace::traceFunction = std::make_unique<StdOutTraceFunction>();
	/**
	 *
	 */
	/* static */ std::mutex Trace::traceFunctionMutex;
	/**
	 * This map maintains the indention level per thread, based on the thread::id
	 */
	std::map< std::thread::id, unsigned long > threadIndentionLevels;

	/**
	 *
	 */
	Trace::Trace(	const std::string& aText,
					long aLineNumber 	/* = 0 */,
					bool withTimeStamp 	/* = false */,
					bool aTraceCDtor 	/* = true*/) :
					traceText( aText),
					traceTimeStamp( withTimeStamp),
					traceCDtor( aTraceCDtor)
	{
		if (traceCDtor == true)
		{
			traceConstructor( aLineNumber);
		}
	}

	/**
	 *
	 */
	Trace::Trace(	const std::string& aText1,
					const std::string& aText2,
					long aLineNumber 	/* = 0 */,
					bool withTimeStamp 	/* = false */,
					bool aTraceCDtor 	/* = true*/) :
					traceText(aText1 + " " + aText2),
					traceTimeStamp( withTimeStamp),
					traceCDtor( aTraceCDtor)
	{
		if (traceCDtor == true)
		{
			traceConstructor( aLineNumber);
		}
	}
	/**
	 *
	 */
	Trace::~Trace()
	{
		if (traceCDtor == true)
		{
			traceDestructor();
		}
	}
	/**
	 *
	 */
	/* static */void Trace::trace(	const std::string& aText,
									bool withTimeStamp /* = Trace::isTimeStampEnabled() */)
	{
		std::ostringstream os;

		if(withTimeStamp==true)
		{
			os << Utils::TimeStamp() << ": ";
		}
		os << aText;

		Trace::traceFormattedString(os.str(), '>');
	}
	/**
	 *
	 */
	/* static */void Trace::trace(	const std::string& aText,
									const char* aFunctionName,
									long aLineNumber,
									bool withTimeStamp /* = Trace::isTimeStampEnabled() */)
	{

		std::ostringstream os;

		if(withTimeStamp==true)
		{
			os << Utils::TimeStamp() << ": ";
		}

		os << aText << aFunctionName << " ( line " << aLineNumber << ")";

		Trace::traceFormattedString(os.str(), '>');

	}
	/**
	 *
	 */
	void Trace::traceConstructor( long aLineNumber) const
	{
		if(traceText.length() > 0)
		{
			std::ostringstream os;

			if(traceTimeStamp==true)
			{
				os << Utils::TimeStamp() <<  ": ";
			}

			os << traceText;

			if(Trace::traceLineNumber)
			{
				os << " (line " << aLineNumber << ")";

			}
			Trace::traceFormattedString( os.str(), '+');
		} else
		{
			Trace::traceFormattedString("Ctor: ??? No Text ???", '+');
		}
	}
	/**
	 *
	 */
	void Trace::traceDestructor() const
	{
		if(traceText.length() > 0)
		{
			std::ostringstream os;

			if(traceTimeStamp==true)
			{
				os << Utils::TimeStamp() << ": ";
			}

			os << traceText;
			Trace::traceFormattedString( os.str(), '-');
		} else
		{
			Trace::traceFormattedString("Dtor: ??? No Text ???", '-');
		}
	}
	/**
	 *
	 */
	/* static */void Trace::traceFormattedString(	const std::string& aText,
													char aTraceMarker)	// 	This text will be prepended
	{
		if(Trace::traceOn)
		{
			std::ostringstream os;
			std::string margin;

			if(Trace::traceThreadId)
			{
				std::thread::id currentThreadId = std::this_thread::get_id();
				#if __cplusplus <= 201703L
				if(threadIndentionLevels.find(currentThreadId)!=threadIndentionLevels.end())
				#else
				if(!threadIndentionLevels.contains(currentThreadId)) // @suppress("Method cannot be resolved")
				#endif
				{
					threadIndentionLevels.insert(std::make_pair(currentThreadId,0));
				}
				switch(aTraceMarker)
				{
					case '+':
					{
						margin.insert(0, threadIndentionLevels[currentThreadId], ' ');
						threadIndentionLevels[currentThreadId] += INDENTIONLEVEL;
						break;
					}
					case '-':
					{
						threadIndentionLevels[currentThreadId] -= INDENTIONLEVEL;
						margin.insert(0, threadIndentionLevels[currentThreadId], ' ');
						break;
					}
					default:
					{
						margin.insert(0, threadIndentionLevels[currentThreadId], ' ');
						break;
					}
				}

				os << margin << currentThreadId << ": " << aTraceMarker << " " << aText;
			}else
			{
				os << margin <<  aTraceMarker << " " << aText;
			}

			std::lock_guard< std::mutex > lock( traceFunctionMutex);
			traceFunction->trace(os.str());
		}
	}


} /* namespace Base */
