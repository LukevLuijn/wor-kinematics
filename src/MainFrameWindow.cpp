#include "MainFrameWindow.hpp"

#include "Client.hpp"
#include "FileTraceFunction.hpp"
#include "LogTextCtrl.hpp"
#include "Logger.hpp"
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

#include "Button.hpp"
#include "Slider.h"
#include "ToggleButton.h"


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
        ID_QUIT = wxID_EXIT,         //!< ID_QUIT
        ID_OPTIONS = wxID_PROPERTIES,//!< ID_OPTIONS
        ID_ABOUT = wxID_ABOUT,       //!< ID_ABOUT
        ID_WIDGET_TRACE_FUNCTION,    //!< ID_WIDGET_TRACE_FUNCTION
        ID_STDCOUT_TRACE_FUNCTION,   //!< ID_STDCOUT_TRACE_FUNCTION
        ID_FILE_TRACE_FUNCTION       //!< ID_FILE_TRACE_FUNCTION

    };
    /**
	 *
	 */
    MainFrameWindow::MainFrameWindow(const std::string& aTitle)
        : Frame(nullptr, DEFAULT_ID, WXSTRING(aTitle), DefaultPosition, Size(1200, 600)), clientPanel(nullptr),
          menuBar(nullptr), splitterWindow(nullptr), lhsPanel(nullptr), robotWorldCanvas(nullptr), rhsPanel(nullptr),
          logTextCtrl(nullptr), buttonPanel(nullptr)
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
        SetMenuBar(initialiseMenuBar());

        GridBagSizer* sizer = new GridBagSizer(5, 5);

        sizer->Add(initialiseClientPanel(),
                   GBPosition(0, 0),// row ,col
                   GBSpan(1, 1),    // row ,col
                   EXPAND);

        sizer->AddGrowableCol(0);
        sizer->AddGrowableRow(0);

        sizer->SetSizeHints(this);

        Bind(
                wxEVT_COMMAND_MENU_SELECTED,
                [this](CommandEvent& anEvent) {
                    this->OnQuit(anEvent);
                },
                ID_QUIT);
        Bind(
                wxEVT_COMMAND_MENU_SELECTED,
                [this](CommandEvent& anEvent) {
                    this->OnWidgetTraceFunction(anEvent);
                },
                ID_WIDGET_TRACE_FUNCTION);
        Bind(
                wxEVT_COMMAND_MENU_SELECTED,
                [this](CommandEvent& anEvent) {
                    this->OnStdOutTraceFunction(anEvent);
                },
                ID_STDCOUT_TRACE_FUNCTION);
        Bind(
                wxEVT_COMMAND_MENU_SELECTED,
                [this](CommandEvent& anEvent) {
                    this->OnFileTraceFunction(anEvent);
                },
                ID_FILE_TRACE_FUNCTION);
        Bind(
                wxEVT_COMMAND_MENU_SELECTED,
                [this](CommandEvent& anEvent) {
                    this->OnAbout(anEvent);
                },
                ID_ABOUT);

        // By default we use the WidgettraceFunction as we expect that this is what the user wants....
        Base::Trace::setTraceFunction(std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
    }
    /**
	 *
	 */
    MenuBar* MainFrameWindow::initialiseMenuBar()
    {
        Menu* fileMenu = new Menu;
        fileMenu->Append(ID_QUIT, WXSTRING("E&xit\tAlt-X"), WXSTRING("Exit the application"));

        Menu* debugMenu = new Menu;
        debugMenu->AppendRadioItem(ID_WIDGET_TRACE_FUNCTION, WXSTRING("Widget"), WXSTRING("Widget"));
        debugMenu->AppendRadioItem(ID_STDCOUT_TRACE_FUNCTION, WXSTRING("StdOut"), WXSTRING("StdOut"));
        debugMenu->AppendRadioItem(ID_FILE_TRACE_FUNCTION, WXSTRING("File"), WXSTRING("File"));

        Menu* helpMenu = new Menu;
        helpMenu->Append(ID_ABOUT, WXSTRING("&About...\tF1"), WXSTRING("Show about dialog"));

        menuBar = new wxMenuBar;
        menuBar->Append(fileMenu, WXSTRING("&File"));
        menuBar->Append(debugMenu, WXSTRING("&Debug"));
        menuBar->Append(helpMenu, WXSTRING("&Help"));

        return menuBar;
    }
    /**
	 *
	 */
    Panel* MainFrameWindow::initialiseClientPanel()
    {
        if (!clientPanel)
        {
            clientPanel = new Panel(this, DEFAULT_ID);

            GridBagSizer* sizer = new GridBagSizer();

            sizer->Add(5, 5, GBPosition(0, 0));

            sizer->Add(initialiseSplitterWindow(), GBPosition(1, 1), GBSpan(1, 1), EXPAND);
            sizer->AddGrowableRow(1);
            sizer->AddGrowableCol(1);

            sizer->Add(5, 5, GBPosition(2, 2));

            clientPanel->SetSizer(sizer);
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
            splitterWindow = new SplitterWindow(clientPanel, DEFAULT_ID);
            splitterWindow->SplitVertically(initialiseLhsPanel(), initialiseRhsPanel());
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
            lhsPanel = new Panel(splitterWindow, DEFAULT_ID);

            GridBagSizer* sizer = new GridBagSizer();
            sizer->Add(5, 5, GBPosition(0, 0), GBSpan(1, 1), EXPAND);

            sizer->Add(robotWorldCanvas = new View::RobotWorldCanvas(lhsPanel),
                       GBPosition(1, 1),
                       GBSpan(1, 1) /*, EXPAND*/);

            robotWorldCanvas->SetMinSize(wxSize(1024, 1024));


            sizer->Add(5, 5, GBPosition(2, 2), GBSpan(1, 1), EXPAND);

            lhsPanel->SetSizer(sizer);
            sizer->SetSizeHints(lhsPanel);
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
            rhsPanel = new Panel(splitterWindow, DEFAULT_ID);

            GridBagSizer* sizer = new GridBagSizer();
            sizer->Add(5, 5, GBPosition(0, 0), GBSpan(1, 1), EXPAND);

            sizer->Add(logTextCtrl = new LogTextCtrl(rhsPanel, DEFAULT_ID, wxTE_MULTILINE | wxTE_DONTWRAP),
                       GBPosition(1, 1),
                       GBSpan(1, 1),
                       EXPAND);
            sizer->AddGrowableCol(1);
            sizer->AddGrowableRow(1);
            logTextCtrl->SetMinSize(Size(500, 300));

            sizer->Add(buttonPanel = initialiseButtonPanel(), GBPosition(2, 1), GBSpan(1, 1), EXPAND);
            sizer->AddGrowableRow(2);

            sizer->Add(5, 5, GBPosition(2, 2), GBSpan(1, 1), EXPAND);

            rhsPanel->SetSizer(sizer);
            sizer->SetSizeHints(rhsPanel);
        }
        return rhsPanel;
    }
    /**
	 *
	 */
    Panel* MainFrameWindow::initialiseButtonPanel()
    {
        auto* panel = new Panel(rhsPanel, DEFAULT_ID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        auto* sizer = new wxFlexGridSizer(3, 5, 0, 0);

        sizer->AddGrowableCol(3);
        sizer->SetFlexibleDirection(wxBOTH);
        sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        kalmanFilterToggle = makeToggleButton(panel, "Kalman filter", [this](CommandEvent& anEvent) {
            this->OnKalmanToggle(anEvent);
        });

        kalmanFilterToggle->SetValue(true);
        kalmanFilterToggle->SetBackgroundColour(KALMAN_BLUE);

        particleFilterToggle = makeToggleButton(panel, "Particle filter", [this](CommandEvent& anEvent) {
            this->OnParticleToggle(anEvent);
        });
        particleFilterToggle->SetValue(true);
        particleFilterToggle->SetBackgroundColour(PARTICLE_GREEN);


        compassSlider = makeSlider(panel, 0, 100, [this](CommandEvent& anEvent) {
            this->OnSliderCompass(anEvent);
        });

        if (MainApplication::isArgGiven("-compass"))
        {
            compassConfigValue = std::stoi(MainApplication::getArg("-compass").value);
            compassSlider->SetValue(static_cast<int32_t>(compassConfigValue));
            compassSlider->SetMax(static_cast<int32_t>(compassConfigValue * 100));
        }


        odometerSlider = makeSlider(panel, 0, 100, [this](CommandEvent& anEvent) {
            this->OnSliderOdometer(anEvent);
        });
        if (MainApplication::isArgGiven("-odometer"))
        {
            odometerConfigValue = std::stoi(MainApplication::getArg("-odometer").value);
            odometerSlider->SetValue(static_cast<int32_t>(odometerConfigValue));
            odometerSlider->SetMax(static_cast<int32_t>(odometerConfigValue * 100));

        }


        lidarSlider = makeSlider(panel, 0, 100, [this](CommandEvent& anEvent) {
            this->OnSliderLidar(anEvent);
        });
        if (MainApplication::isArgGiven("-lidar"))
        {
            lidarConfigValue = std::stoi(MainApplication::getArg("-lidar").value);
            lidarSlider->SetValue(static_cast<int32_t>(lidarConfigValue));
            lidarSlider->SetMax(static_cast<int32_t>(lidarConfigValue * 100));
        }


        sizer->Add(makeButton(panel,
                              "Populate world",
                              [this](CommandEvent& anEvent) {
                                  this->OnPopulate(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(makeButton(panel,
                              "Clear world",
                              [this](CommandEvent& anEvent) {
                                  this->OnUnpopulate(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(new wxStaticText(panel, DEFAULT_ID, WXSTRING("Compass:")),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(compassSlider, 1, wxALL | wxEXPAND, 5);
        sizer->Add(makeButton(panel,
                              "Reset compass",
                              [this](CommandEvent& anEvent) {
                                this->OnResetSliderCompass(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(makeButton(panel,
                              "Start robot",
                              [this](CommandEvent& anEvent) {
                                  this->OnStartRobot(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(makeButton(panel,
                              "Stop robot",
                              [this](CommandEvent& anEvent) {
                                  this->OnStopRobot(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(new wxStaticText(panel, DEFAULT_ID, WXSTRING("Odometer:")),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(odometerSlider, 1, wxALL | wxEXPAND, 5);
        sizer->Add(makeButton(panel,
                              "Reset odometer",
                              [this](CommandEvent& anEvent) {
                                this->OnResetSliderOdometer(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(kalmanFilterToggle, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
        sizer->Add(particleFilterToggle, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
        sizer->Add(new wxStaticText(panel, DEFAULT_ID, WXSTRING("Lidar:")),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        sizer->Add(lidarSlider, 1, wxALL | wxEXPAND, 5);
        sizer->Add(makeButton(panel,
                              "Reset lidar",
                              [this](CommandEvent& anEvent) {
                                this->OnResetSliderLidar(anEvent);
                              }),
                   1,
                   wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL,
                   5);
        panel->SetSizerAndFit(sizer);

        return panel;
    }
    /**
	 *
	 */
    void MainFrameWindow::OnQuit(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Base::Trace::setTraceFunction(std::make_unique<Base::StdOutTraceFunction>());
        Close(true);
    }
    /**
	 *
	 */
    void MainFrameWindow::OnWidgetTraceFunction(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Base::Trace::setTraceFunction(std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
    }
    /**
	 *
	 */
    void MainFrameWindow::OnStdOutTraceFunction(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Base::Trace::setTraceFunction(std::make_unique<Base::StdOutTraceFunction>());
    }
    /**
	 *
	 */
    void MainFrameWindow::OnFileTraceFunction(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Base::Trace::setTraceFunction(std::make_unique<Base::FileTraceFunction>("trace", "log", true));
    }
    /**
	 *
	 */
    void MainFrameWindow::OnAbout(CommandEvent& UNUSEDPARAM(anEvent))
    {
        wxMessageBox(WXSTRING("ESD 2012-present RobotWorld.\n"),
                     WXSTRING("About RobotWorld"),
                     wxOK | wxICON_INFORMATION,
                     this);
    }
    /**
	 *
	 */
    void MainFrameWindow::OnStartRobot(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");

        if (!robot)
        {
            LOG("There is no robot available");
        }
        else if (robot->isActing())
        {
            LOG("Robot is busy..");
        }
        else
        {
            LOG("Starting robot, this can take a while");
            robot->startActing();
        }
    }
    /**
	 *
	 */
    void MainFrameWindow::OnStopRobot(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");

        if (!robot)
        {
            LOG("There is no robot available");
        }
        else if (!robot->isActing())
        {
            LOG("Robot is already stopped");
        }
        else
        {
            LOG("Stopping robot..");
            robot->stopActing();
        }
    }
    /**
	 *
	 */
    void MainFrameWindow::OnPopulate(CommandEvent& UNUSEDPARAM(anEvent))
    {
        LOG("Populating world");

        std::vector<Model::Filters_e> filters;

        if (kalmanFilterToggle->GetValue())
        {
            filters.emplace_back(Model::Filters_e::KALMAN_FILTER);
        }
        if (particleFilterToggle->GetValue())
        {
            filters.emplace_back(Model::Filters_e::PARTICLE_FILTER);
        }


        // clear previous instances
        robotWorldCanvas->unpopulate();
        robotWorldCanvas->populate(filters, 4);
    }
    /**
	 *
	 */
    void MainFrameWindow::OnUnpopulate(CommandEvent& UNUSEDPARAM(anEvent))
    {
        LOG("Un-populating world");

        robotWorldCanvas->unpopulate();
    }
    /**
     *
     */
    void MainFrameWindow::OnKalmanToggle(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
        if (robot)
        {
            if (kalmanFilterToggle->GetValue())
            {
                LOG("adding kalman filter to robot");
                robot->addFilter(Model::Filters_e::KALMAN_FILTER);
                kalmanFilterToggle->SetBackgroundColour(KALMAN_BLUE);
            }
            else
            {
                LOG("removing kalman filter from robot");
                robot->removeFilter(Model::Filters_e::KALMAN_FILTER);
                kalmanFilterToggle->SetBackgroundColour(GetBackgroundColour());
            }
        }
        else
        {
            kalmanFilterToggle->SetValue(!kalmanFilterToggle->GetValue());
            LOG("There is no robot available");
        }
    }
    /**
     *
     */
    void MainFrameWindow::OnParticleToggle(CommandEvent& UNUSEDPARAM(anEvent))
    {
        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
        if (robot)
        {
            if (particleFilterToggle->GetValue())
            {
                LOG("adding particle filter to robot");
                robot->addFilter(Model::Filters_e::PARTICLE_FILTER);
                particleFilterToggle->SetBackgroundColour(PARTICLE_GREEN);
            }
            else
            {
                LOG("removing particle filter from robot");
                robot->removeFilter(Model::Filters_e::PARTICLE_FILTER);
                particleFilterToggle->SetBackgroundColour(GetBackgroundColour());
            }
        }
        else
        {
            particleFilterToggle->SetValue(!particleFilterToggle->GetValue());
            LOG("There is no robot available");
        }
    }
    /**
     *
     */
    void MainFrameWindow::OnSliderCompass(CommandEvent& UNUSEDPARAM(anEvent))
    {
        const std::string value = std::to_string(compassSlider->GetValue());

        if (MainApplication::isArgGiven("-compass"))
        {
            MainApplication::getArg("-compass").value = value;
        }
        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");

        if (robot)
        {
            robot->resetSensor("CompassSensor");
        }

        LOG("compass standard deviation set", value + " degrees");
    }
    /**
     *
     */
    void MainFrameWindow::OnSliderOdometer(CommandEvent& UNUSEDPARAM(anEvent))
    {
        const std::string value = std::to_string(odometerSlider->GetValue());

        if (MainApplication::isArgGiven("-odometer"))
        {
            MainApplication::getArg("-odometer").value = value;
        }

        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");

        if (robot)
        {
            robot->resetSensor("OdometerSensor");
        }

        LOG("odometer standard deviation set", value + " pixels");
    }
    /**
     *
     */
    void MainFrameWindow::OnSliderLidar(CommandEvent& UNUSEDPARAM(anEvent))
    {
        const std::string value = std::to_string(lidarSlider->GetValue());

        if (MainApplication::isArgGiven("-lidar"))
        {
            MainApplication::getArg("-lidar").value = value;
        }

        Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");

        if (robot)
        {
            robot->resetSensor("LidarSensor");

        }

        LOG("lidar standard deviation set", value + " pixels");
    }
    void MainFrameWindow::OnResetSliderCompass(CommandEvent& UNUSEDPARAM(anEvent))
    {
        compassSlider->SetValue(static_cast<int32_t>(compassConfigValue));

        LOG("resetting compass standard deviation");

        wxCommandEvent anEvent;
        OnSliderCompass(anEvent);
    }
    void MainFrameWindow::OnResetSliderOdometer(CommandEvent& UNUSEDPARAM(anEvent))
    {
        odometerSlider->SetValue(static_cast<int32_t>(odometerConfigValue));

        LOG("resetting odometer standard deviation");

        wxCommandEvent anEvent;
        OnSliderOdometer(anEvent);
    }
    void MainFrameWindow::OnResetSliderLidar(CommandEvent& UNUSEDPARAM(anEvent))
    {
        lidarSlider->SetValue(static_cast<int32_t>(lidarConfigValue));

        LOG("resetting lidar standard deviation");

        wxCommandEvent anEvent;
        OnSliderLidar(anEvent);

    }
}// namespace Application
