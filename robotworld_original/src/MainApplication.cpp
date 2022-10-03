#include "MainApplication.hpp"

#include "MainFrameWindow.hpp"
#include "ObjectId.hpp"

#include <stdexcept>
#include <algorithm>
#include <cstring>


namespace Application
{
	/* static */std::vector< CommandlineArgument > MainApplication::commandlineArguments;
	/* static */std::vector< std::string > MainApplication::commandlineFiles;

	// Create a new application object: this macro will allow wxWidgets to create
	// the application object during program execution (it's better than using a
	// static object for many reasons) and also implements the accessor function
	// wxGetApp() which will return the reference of the right type (i.e. MyApp and
	// not wxApp)
	wxIMPLEMENT_APP_NO_MAIN( MainApplication); // @suppress("C-Style cast instead of C++ cast")

	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	MainApplication& TheApp()
	{
		return wxGetApp();
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	bool MainApplication::OnInit()
	{
		// To make all platforms use all available images
		wxInitAllImageHandlers();

		MainApplication::setCommandlineArguments( argc, argv);

		MainFrameWindow* frame = nullptr;
		if(MainApplication::isArgGiven("-worldname"))
		{
			Base::ObjectId::objectIdNamespace = MainApplication::getArg("-worldname").value + "-";

			frame = new MainFrameWindow( "RobotWorld : " + MainApplication::getArg("-worldname").value);

		}else
		{
			frame = new MainFrameWindow( "RobotWorld");
		}

		SetTopWindow( frame);

		// and show it (the frames, unlike simple controls, are not shown when
		// created initially)
		frame->Show( true);

		// success: wxApp::OnRun() will be called which will enter the main message
		// loop and the application will run. If we returned false here, the
		// application would exit immediately.
		return true;
	}
	/**
	 *
	 */
	/* static */void MainApplication::setCommandlineArguments( 	int theArgc,
																char* theArgv[])
	{

		// argv[0] contains the executable name as one types on the command line (with or without extension)
		MainApplication::commandlineArguments.push_back( CommandlineArgument( 0, "Executable", theArgv[0]));

		for (unsigned int i = 1; i < static_cast<unsigned int >(theArgc); ++i)
		{
			char* currentArg = theArgv[i];
			size_t argLength = std::strlen( currentArg);


			// If the first char of the argument is not a "-" we assume that is is
			// a filename otherwise it is an ordinary argument

			if (currentArg[0] == '-') // ordinary argument
			{
				bool inserted = false;

				// First handle the arguments in the form of "variable=value", and find the "="

				for (size_t j = 0; j < argLength; ++j)
				{
					if (currentArg[j] == '=')
					{
						std::string variable( currentArg, j);
						std::string value( &currentArg[j + 1]);
						MainApplication::commandlineArguments.push_back( CommandlineArgument( i, variable, value));
						inserted = true;
					}
				}

				// Second handle the stand alone arguments.

				// If inserted is

				// It is assumed that they are actually booleans.
				// If given on the command line than the variable will be set to true as if
				// variable=true is passed
				if (inserted == false)
				{
					std::string variable( currentArg);
					std::string value( "true");
					MainApplication::commandlineArguments.push_back( CommandlineArgument( i, variable, value));
				}
			} else // file argument
			{
				MainApplication::commandlineFiles.push_back( currentArg);
			}
		}
	}
	/**
	 *
	 */
	/* static */bool MainApplication::isArgGiven( const std::string& aVariable)
	{
		std::vector< CommandlineArgument >::iterator i = std::find( MainApplication::commandlineArguments.begin(), MainApplication::commandlineArguments.end(), aVariable);
		return i != MainApplication::commandlineArguments.end();
	}

	/* static */CommandlineArgument& MainApplication::getArg( const std::string& aVariable)
	{
		std::vector< CommandlineArgument >::iterator i = std::find( MainApplication::commandlineArguments.begin(), MainApplication::commandlineArguments.end(), aVariable);
		if (i == MainApplication::commandlineArguments.end())
		{
			throw std::invalid_argument( "No such command line argument");
		}
		return *i; // @suppress("Returning the address of a local variable")
	}
	/**
	 *
	 */
	/* static */CommandlineArgument& MainApplication::getArg( unsigned long anArgumentNumber)
	{
		if(anArgumentNumber >= MainApplication::commandlineArguments.size())
		{
			throw std::invalid_argument( "No such command line argument");
		}
		return MainApplication::commandlineArguments[anArgumentNumber];
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	/* static */std::vector< std::string >& MainApplication::getCommandlineFiles()
	{
		return commandlineFiles;
	}
} // namespace Application
