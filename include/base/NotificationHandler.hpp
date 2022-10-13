#ifndef NOTIFICATIONHANDLER_HPP_
#define NOTIFICATIONHANDLER_HPP_

#include "Config.hpp"

#include "NotificationEvent.hpp"
#include "NotificationFunctionTypeTraits.hpp"
#include "Widgets.hpp"

namespace Base
{
    /**
	 *
	 */
    template<typename NotificationFunction>
    class NotificationHandler : public EventHandler
    {
    public:
        explicit NotificationHandler(const NotificationFunction& aNotificationFunction)
            : function(aNotificationFunction)
        {
// TODO: Change this code so we don't have to use the #pragma's nor the reinterpret_cast.
// See @https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas for the #pragma explanation.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"

            Connect(EVT_NOTIFICATIONEVENT,
                    (wxObjectEventFunction)(wxEventFunction) reinterpret_cast<NotificationEventFunction>(
                            &NotificationHandler::OnNotificationEvent));

#pragma GCC diagnostic pop
        }
        virtual ~NotificationHandler()
        {
        }
        /**
			 *
			 */
        void OnNotificationEvent(NotifyEvent& aNotifyEvent)
        {
            NotificationFunctionTypeTraits<NotificationFunction> caller;
            caller.call(function, aNotifyEvent);
        }

    private:
        NotificationFunction function;
    };
}//namespace Base
#endif// NOTIFICATIONHANDLER_HPP_
