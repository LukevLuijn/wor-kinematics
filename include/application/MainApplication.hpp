#ifndef MAINAPPLICATION_HPP_
#define MAINAPPLICATION_HPP_

#include "Config.hpp"

#include "CommandlineArgument.hpp"
#include "Widgets.hpp"

#include <string>
#include <vector>

/**
 *
 */
#define runGUI wxEntry

namespace Application
{
    class MainApplication;
    /**
	 * Global application accessor function.
	 *
	 * @return MainApplication The MainApplication object for this application.
	 */
    MainApplication& TheApp();

    /**
	 *
	 */
    class MainApplication : public wxApp
    {
    public:
        /**
			 * This one is called on application startup and is a good place for the app
			 * initialisation: doing it here and not in the ctor allows to have an error return
			 *
			 * @return If OnInit() returns false, the application terminates
			 */
        virtual bool OnInit();
        /**
			 * @name Command line handling functions
			 */
        //@{
        /**
			 * The handling of the arguments is:
			 * 1. Any argument starting with "-" that has "=" in it somewhere is treated as "argument = value". Spaces are not allowed.
			 * 2. Any argument starting with a "-" that has no "=" in it somewhere is treated as a boolean with the value "true". There are no variables that can be false.
			 * 3. Arguments without "-" prefix are assumed to be files.
			 * 4. The "-" is NOT stripped from the argument.
			 *
			 * @param argc the count of the arguments
			 * @param argv the array with the values of the arguments
			 */
        static void setCommandlineArguments(int theArgc, char* theArgv[]);
        /**
			 *
			 * @param aVariable The format of the variable is implementation defined.
			 * 					Be aware that "-" is NOT stripped from the argument.
			 * 					The comparisson is done by operator==( const string&).
			 * @return true if the argument is given, false otherwise.
			 */
        static bool isArgGiven(const std::string& aVariable);
        /**
			 *
			 * @param aVariable The requested variable
			 * @return The requested argument if available, throws an exception otherwise
			 */
        static CommandlineArgument& getArg(const std::string& aVariable);
        /**
			 *
			 * @param anArgumentNumber The requested variable
			 * @return The requested argument if available, throws an exception otherwise
			 */
        static CommandlineArgument& getArg(unsigned long anArgumentNumber);
        /**
			 *
			 * @return Any files that are given on the command line.
			 */
        static std::vector<std::string>& getCommandlineFiles();
        /**
             * Reads configuration values from config file.
             */
        static void readFileConfiguration();
        //@}

    private:
        static std::vector<CommandlineArgument> commandlineArguments;
        static std::vector<std::string> commandlineFiles;
    };
    //	class MainApplication
}// namespace Application

#endif// MAINAPPLICATION_HPP_
