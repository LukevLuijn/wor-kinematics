#ifndef MAINFRAMEWINDOW_HPP_
#define MAINFRAMEWINDOW_HPP_

#include "Config.hpp"

#include "Widgets.hpp"

namespace Base
{
    class ITraceFunction;
}// namespace Base

namespace View
{
    class RobotWorldCanvas;
}

namespace Application
{
    class LogTextCtrl;

    /**
	 *
	 */
    class MainFrameWindow : public Frame
    {
    public:
        /**
			 *
			 * @param aTitle The title which is shown in the title bar
			 */
        explicit MainFrameWindow(const std::string& aTitle);
        /**
			 *
			 */
        virtual ~MainFrameWindow();

    protected:
        /**
			 *
			 */
        void initialise();
        /**
			 *
			 */
        MenuBar* initialiseMenuBar();
        /**
			 *
			 */
        Panel* initialiseClientPanel();
        /**
			 *
			 */
        SplitterWindow* initialiseSplitterWindow();
        /**
			 *
			 */
        Panel* initialiseLhsPanel();
        /**
			 *
			 */
        Panel* initialiseRhsPanel();
        /**
			 *
			 */
        Panel* initialiseButtonPanel();

    protected:
    private:
        Panel* clientPanel;
        MenuBar* menuBar;
        SplitterWindow* splitterWindow;

        Panel* lhsPanel;
        View::RobotWorldCanvas* robotWorldCanvas;

        Panel* rhsPanel;
        LogTextCtrl* logTextCtrl;
        Panel* buttonPanel;

        ToggleButton* kalmanFilterToggle;
        ToggleButton* particleFilterToggle;

        Slider* compassSlider;
        Slider* odometerSlider;
        Slider* lidarSlider;

        double compassConfigValue;
        double odometerConfigValue;
        double lidarConfigValue;

        void OnQuit(CommandEvent& anEvent);
        void OnWidgetTraceFunction(CommandEvent& anEvent);
        void OnStdOutTraceFunction(CommandEvent& anEvent);
        void OnFileTraceFunction(CommandEvent& anEvent);
        void OnAbout(CommandEvent& anEvent);

        void OnStartRobot(CommandEvent& anEvent);
        void OnStopRobot(CommandEvent& anEvent);
        void OnPopulate(CommandEvent& anEvent);
        void OnUnpopulate(CommandEvent& anEvent);
        void OnKalmanToggle(CommandEvent& anEvent);
        void OnParticleToggle(CommandEvent& anEvent);

        void OnSliderCompass(CommandEvent& anEvent);
        void OnSliderOdometer(CommandEvent& anEvent);
        void OnSliderLidar(CommandEvent& anEvent);

        void OnResetSliderCompass(CommandEvent& anEvent);
        void OnResetSliderOdometer(CommandEvent& anEvent);
        void OnResetSliderLidar(CommandEvent& anEvent);
    };
    //	class MainFrameWindow
}//namespace Application

#endif// MAINFRAMEWINDOW_HPP_
