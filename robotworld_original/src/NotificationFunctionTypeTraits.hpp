#ifndef NOTIFICATIONFUNCTIONTYPETRAITS_HPP_
#define NOTIFICATIONFUNCTIONTYPETRAITS_HPP_

#include "Config.hpp"

#include "Widgets.hpp"

#include <functional>

namespace Base
{
	struct NotificationFunctionTypeTraitsTracing
	{
	};

	template< typename NotificationFunction >
	struct NotificationFunctionTypeTraits
	{
			static void call( 	const NotificationFunction& aNotificationFunction,
						NotifyEvent&)
			{
				aNotificationFunction();
			}
	};

	template< >
	struct NotificationFunctionTypeTraits< std::function< void( NotifyEvent&) > >
	{
			static void call( 	const std::function< void( NotifyEvent&) >& aNotificationFunction,
								NotifyEvent& event)
			{
				aNotificationFunction( event);
			}
	};
	/**
	 * Dummy function to allow for usage of not-yet implemented
	 * std::function<void (CommandEvent&)> functions
	 * @param
	 */
	void Ooops( NotifyEvent&);
} //namespace Base
#endif /* NOTIFICATIONFUNCTIONTYPETRAITS_HPP_ */
