#include "LogTextCtrl.hpp"

namespace Application
{
	/**
	 *
	 */
	wxDEFINE_EVENT(ROBOTWORLD_TRACE_EVENT, CommandEvent);
	/**
	 *
	 */
	LogTextCtrl::LogTextCtrl( 	Window* aParent,
								WindowId aWindowId /*  = DEFAULT_ID */,
								long aStyle /* = wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP */,
								const std::string& anInitialText /* = "" */,
								const Point& aPoint /* = DefaultPosition */,
								const Size& aSize /* = DefaultSize */) :
								wxTextCtrl( aParent, aWindowId, WXSTRING( anInitialText), aPoint, aSize, aStyle)
	{
		Bind(ROBOTWORLD_TRACE_EVENT,
			 [this](CommandEvent& anEvent){ this->OnCommandEvent(anEvent);});
	}
	/**
	 *
	 */
	LogTextCtrl::~LogTextCtrl()
	{
	}
	/**
	 *
	 */
	void LogTextCtrl::log( const std::string& aString)
	{
		AppendText( WXSTRING( aString + "\n"));
	}
	/**
	 *
	 */
	void LogTextCtrl::OnCommandEvent( CommandEvent& anEvent)
	{
		log( STDSTRING( anEvent.GetString()));
	}
} // namespace Application
