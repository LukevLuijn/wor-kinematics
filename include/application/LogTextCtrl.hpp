#ifndef LOGTEXTCTRL_HPP_
#define LOGTEXTCTRL_HPP_

#include "Config.hpp"

#include "Point.hpp"
#include "Size.hpp"
#include "Widgets.hpp"

namespace Application
{
	/**
	 *
	 */
	wxDECLARE_EVENT(ROBOTWORLD_TRACE_EVENT, CommandEvent);
	/**
	 * A text widget that accepts the tracing of a trace function.
	 */
	class LogTextCtrl : public wxTextCtrl
	{
		public:
			/**
			 * @name Constructors and destructor
			 */
			//@{
			/**
			 *
			 */
			explicit LogTextCtrl( 	Window* aParent,
									WindowId aWindowId = DEFAULT_ID,
									long aStyle = wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP,
									const std::string& anInitialText = "",
									const Point& aPoint = DefaultPosition,
									const Size& aSize = DefaultSize);
			virtual ~LogTextCtrl();
			//@}
			/**
			 *
			 * @param aString
			 */
			virtual void log( const std::string& aString);
		protected:
			/**
			 *
			 * @param anEvent
			 */
			void OnCommandEvent( CommandEvent& anEvent);
		private:
	};
	// class LogTextCtrl
} // namespace Application
#endif // LOGTEXTCTRL_HPP_
