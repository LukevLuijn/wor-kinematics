#include "FileTraceFunction.hpp"

#include "TimeStamp.hpp"

#include <string>

namespace Base
{
	/**
	 *
	 */
	FileTraceFunction::FileTraceFunction( const std::string& aFileStem /* = "trace" */,
										  const std::string& aFileExtension /* = "log" */,
										  bool timeStamped /* = false */ )

						:	logFile((aFileStem + (timeStamped ? "-" + Utils::TimeStamp() : "") + "." + aFileExtension).c_str(), std::ios_base::app)
	{
		FileTraceFunction::trace( " ******************* START " + Utils::TimeStamp() + " START *******************");
	}
	/**
	 *
	 */
	FileTraceFunction::~FileTraceFunction()
	{
		FileTraceFunction::trace( " ******************* END " + Utils::TimeStamp() + " END *******************");
		logFile.close();
	}
	/**
	 *
	 */
	void FileTraceFunction::trace( const std::string& aText)
	{
		logFile << aText << std::endl;
	}
} /* namespace Base */
