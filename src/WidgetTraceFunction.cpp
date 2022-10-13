#include "WidgetTraceFunction.hpp"

#include "LogTextCtrl.hpp"

namespace Application
{
    /**
	 *
	 */
    WidgetTraceFunction::WidgetTraceFunction(Application::LogTextCtrl* aLogTextCtrl) : outputControl(aLogTextCtrl)
    {
    }
    /**
	 *
	 */
    void WidgetTraceFunction::trace(const std::string& aText)
    {
        // One can create any type of command event here but we use the
        // LogTextControl provided things
        CommandEvent event(Application::ROBOTWORLD_TRACE_EVENT);
        event.SetString(WXSTRING(aText));

        // TODO: should we decide this runtime?

        // If we need synchronous handling of the trace to prevent loosing
        // messages in case of a crash we must use ProcessEvent. It will
        // slow down the application though.
        //         outputCtrl->ProcessEvent(event);

        // In case we can get away with asynchronous event handling
        // If we do not want to recompile: use StdOutDebugTraceFunction
        wxPostEvent(outputControl, event);
    }
}// namespace Application
