#include "MainFrameWindow.hpp"

#include "Button.hpp"
#include "Client.hpp"
#include "FileTraceFunction.hpp"
#include "Logger.hpp"
#include "LogTextCtrl.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "RobotWorldCanvas.hpp"
#include "Shape2DUtils.hpp"
#include "StdOutTraceFunction.hpp"
#include "WidgetTraceFunction.hpp"

#include <iostream>

namespace Application
{
	/**
	 * IDs for the controls and the menu commands
	 * If there are (default) wxWidget ID's: try to maintain
	 * compatibility, especially wxID_ABOUT because on a Mac it is special
	 */
	enum
	{
		ID_QUIT 	= wxID_EXIT,         	//!< ID_QUIT
		ID_OPTIONS 	= wxID_PROPERTIES,		//!< ID_OPTIONS
		ID_ABOUT 	= wxID_ABOUT,        	//!< ID_ABOUT
		ID_WIDGET_TRACE_FUNCTION, 			//!< ID_WIDGET_TRACE_FUNCTION
		ID_STDCOUT_TRACE_FUNCTION, 			//!< ID_STDCOUT_TRACE_FUNCTION
		ID_FILE_TRACE_FUNCTION 				//!< ID_FILE_TRACE_FUNCTION

	};
	/**
	 *
	 */
	MainFrameWindow::MainFrameWindow( const std::string& aTitle) :
//TODO//            Frame( nullptr, DEFAULT_ID, WXSTRING( aTitle), DefaultPosition, Size( 1200, 600)),
            Frame( nullptr, DEFAULT_ID, WXSTRING( aTitle), DefaultPosition, Size( 1200+524, 600+524)),
								clientPanel( nullptr),
								menuBar( nullptr),
								splitterWindow( nullptr),
								lhsPanel( nullptr),
								robotWorldCanvas( nullptr),
								rhsPanel( nullptr),
								logTextCtrl( nullptr),
								buttonPanel( nullptr)
	{
		initialise();
	}
	/**
	 *
	 */
	MainFrameWindow::~MainFrameWindow()
	{
	}
	/**
	 *
	 */
	void MainFrameWindow::initialise()
	{
		SetMenuBar( initialiseMenuBar());

		GridBagSizer* sizer = new GridBagSizer( 5, 5);

		sizer->Add( initialiseClientPanel(),
					GBPosition( 0, 0), 	// row ,col
					GBSpan( 1, 1), 		// row ,col
					EXPAND);

		sizer->AddGrowableCol( 0);
		sizer->AddGrowableRow( 0);

		sizer->SetSizeHints( this);

		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](CommandEvent& anEvent){ this->OnQuit(anEvent);},
			  ID_QUIT);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](CommandEvent& anEvent){ this->OnWidgetTraceFunction(anEvent);},
			  ID_WIDGET_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](CommandEvent& anEvent){ this->OnStdOutTraceFunction(anEvent);},
			  ID_STDCOUT_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](CommandEvent& anEvent){ this->OnFileTraceFunction(anEvent);},
			  ID_FILE_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](CommandEvent& anEvent){ this->OnAbout(anEvent);},
			  ID_ABOUT);

		// By default we use the WidgettraceFunction as we expect that this is what the user wants....
		Base::Trace::setTraceFunction( std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
	}
	/**
	 *
	 */
	MenuBar* MainFrameWindow::initialiseMenuBar()
	{
		Menu* fileMenu = new Menu;
		fileMenu->Append( ID_QUIT, WXSTRING( "E&xit\tAlt-X"), WXSTRING( "Exit the application"));

		Menu* debugMenu = new Menu;
		debugMenu->AppendRadioItem( ID_WIDGET_TRACE_FUNCTION, WXSTRING( "Widget"), WXSTRING( "Widget"));
		debugMenu->AppendRadioItem( ID_STDCOUT_TRACE_FUNCTION, WXSTRING( "StdOut"), WXSTRING( "StdOut"));
		debugMenu->AppendRadioItem( ID_FILE_TRACE_FUNCTION, WXSTRING( "File"), WXSTRING( "File"));

		Menu* helpMenu = new Menu;
		helpMenu->Append( ID_ABOUT, WXSTRING( "&About...\tF1"), WXSTRING( "Show about dialog"));

		menuBar = new wxMenuBar;
		menuBar->Append( fileMenu, WXSTRING( "&File"));
		menuBar->Append( debugMenu, WXSTRING( "&Debug"));
		menuBar->Append( helpMenu, WXSTRING( "&Help"));

		return menuBar;
	}
	/**
	 *
	 */
	Panel* MainFrameWindow::initialiseClientPanel()
	{
		if (!clientPanel)
		{
			clientPanel = new Panel( this, DEFAULT_ID);

			GridBagSizer* sizer = new GridBagSizer();

			sizer->Add( 5, 5,
						GBPosition( 0, 0));

			sizer->Add( initialiseSplitterWindow(),
						GBPosition( 1, 1),
						GBSpan( 1, 1), EXPAND);
			sizer->AddGrowableRow( 1);
			sizer->AddGrowableCol( 1);

			sizer->Add( 5, 5,
						GBPosition( 2, 2));

			clientPanel->SetSizer( sizer);
		}
		return clientPanel;
	}
	/**
	 *
	 */
	SplitterWindow* MainFrameWindow::initialiseSplitterWindow()
	{
		if (!splitterWindow)
		{
			splitterWindow = new SplitterWindow( clientPanel, DEFAULT_ID);
			splitterWindow->SplitVertically( initialiseLhsPanel(), initialiseRhsPanel());
		}
		return splitterWindow;
	}
	/**
	 *
	 */
	Panel* MainFrameWindow::initialiseLhsPanel()
	{
		if (!lhsPanel)
		{
			lhsPanel = new Panel( splitterWindow, DEFAULT_ID);

			GridBagSizer* sizer = new GridBagSizer();
			sizer->Add( 5, 5,
						GBPosition( 0, 0),
						GBSpan( 1, 1), EXPAND);

			sizer->Add( robotWorldCanvas = new View::RobotWorldCanvas( lhsPanel),
						GBPosition( 1, 1),
						GBSpan( 1, 1)/*, EXPAND*/);
// TODO
//            robotWorldCanvas->SetMinSize( wxSize( 500, 500));
            robotWorldCanvas->SetMinSize( wxSize( 500+524, 500+524));


			sizer->Add( 5, 5,
						GBPosition( 2, 2),
						GBSpan( 1, 1), EXPAND);

			lhsPanel->SetSizer( sizer);
			sizer->SetSizeHints( lhsPanel);
		}
		return lhsPanel;
	}
	/**
	 *
	 */
	Panel* MainFrameWindow::initialiseRhsPanel()
	{
		if (!rhsPanel)
		{
			rhsPanel = new Panel( splitterWindow, DEFAULT_ID);

			GridBagSizer* sizer = new GridBagSizer();
			sizer->Add( 5, 5,
						GBPosition( 0, 0),
						GBSpan( 1, 1), EXPAND);

			sizer->Add( logTextCtrl = new LogTextCtrl( rhsPanel, DEFAULT_ID, wxTE_MULTILINE | wxTE_DONTWRAP),
						GBPosition( 1, 1),
						GBSpan( 1, 1), EXPAND);
			sizer->AddGrowableCol( 1);
			sizer->AddGrowableRow( 1);
			logTextCtrl->SetMinSize( Size( 500, 300));

			sizer->Add( buttonPanel = initialiseButtonPanel(),
						GBPosition( 2, 1),
						GBSpan( 1, 1), SHRINK);
			sizer->AddGrowableRow( 2);

			sizer->Add( 5, 5,
						GBPosition( 2, 2),
						GBSpan( 1, 1), EXPAND);

			rhsPanel->SetSizer( sizer);
			sizer->SetSizeHints( rhsPanel);
		}
		return rhsPanel;
	}
	/**
	 *
	 */
	Panel* MainFrameWindow::initialiseButtonPanel()
	{
		Panel* panel = new Panel( rhsPanel);

		GridBagSizer* sizer = new GridBagSizer();


		sizer->Add( makeButton( panel,
								"Populate",
								[this](CommandEvent &anEvent){this->OnPopulate(anEvent);}),
					GBPosition( 0, 0),
					GBSpan( 1, 1), EXPAND);
		sizer->Add( makeButton( panel,
								"Unpopulate",
								[this](CommandEvent &anEvent){this->OnUnpopulate(anEvent);}),
					GBPosition( 0, 1),
					GBSpan( 1, 1), EXPAND);

		sizer->Add( makeButton( panel,
								"Start robot",
								[this](CommandEvent &anEvent){this->OnStartRobot(anEvent);}),
					GBPosition( 1, 0),
					GBSpan( 1, 1), EXPAND);
		sizer->Add( makeButton( panel,
								"Stop robot",
								[this](CommandEvent &anEvent){this->OnStopRobot(anEvent);}),
					GBPosition( 1, 1),
					GBSpan( 1, 1), EXPAND);


		sizer->Add( makeButton( panel,
								"Start listening",
								[this](CommandEvent &anEvent){this->OnStartListening(anEvent);}),
					GBPosition( 2, 0),
					GBSpan( 1, 1), EXPAND);
		sizer->Add( makeButton( panel,
								"Send message",
								[this](CommandEvent &anEvent){this->OnSendMessage(anEvent);}),
					GBPosition( 2, 1),
					GBSpan( 1, 1), EXPAND);
		sizer->Add( makeButton( panel,
								"Stop listening",
								[this](CommandEvent &anEvent){this->OnStopListening(anEvent);}),
					GBPosition( 2, 2),
					GBSpan( 1, 1), EXPAND);

		panel->SetSizerAndFit( sizer);

		return panel;
	}
	/**
	 *
	 */
	void MainFrameWindow::OnQuit( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::StdOutTraceFunction>());
		Close( true);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnWidgetTraceFunction( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStdOutTraceFunction( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::StdOutTraceFunction>());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnFileTraceFunction( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::FileTraceFunction>("trace", "log", true));
	}
	/**
	 *
	 */
	void MainFrameWindow::OnAbout( CommandEvent& UNUSEDPARAM(anEvent))
	{
		wxMessageBox( WXSTRING( "ESD 2012-present RobotWorld.\n"), WXSTRING( "About RobotWorld"), wxOK | wxICON_INFORMATION, this);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStartRobot( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot && !robot->isActing())
		{
			robot->startActing();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStopRobot( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot && robot->isActing())
		{
			robot->stopActing();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnPopulate( CommandEvent& UNUSEDPARAM(anEvent))
	{
		robotWorldCanvas->populate( 2);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnUnpopulate( CommandEvent& UNUSEDPARAM(anEvent))
	{
		robotWorldCanvas->unpopulate();
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStartListening( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			robot->startCommunicating();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnSendMessage( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			std::string remoteIpAdres = "localhost";
			std::string remotePort = "12345";

			if (MainApplication::isArgGiven( "-remote_ip"))
			{
				remoteIpAdres = MainApplication::getArg( "-remote_ip").value;
			}
			if (MainApplication::isArgGiven( "-remote_port"))
			{
				remotePort = MainApplication::getArg( "-remote_port").value;
			}

			// We will request an echo message. The response will be "Hello World", if all goes OK,
			// "Goodbye cruel world!" if something went wrong.
			Messaging::Client c1ient( remoteIpAdres,
									  static_cast<unsigned short>(std::stoi(remotePort)),
									  robot);
			Messaging::Message message( Messaging::EchoRequest, "Hello world!");
			c1ient.dispatchMessage( message);
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStopListening( CommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			robot->stopCommunicating();
		}
	}
} // namespace Application
