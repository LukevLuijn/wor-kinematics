#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include "Config.hpp"

namespace Base
{
	class Notifier;

	/**
	 * The Observer class is part of a straight forward implementation of the Observer/Notifier pattern
	 *
	 * @see Notifier
	 */
	class Observer
	{
		public:
			/**
			 * @name Constructors and destructor
			 */
			//@{
			/**
			 *
			 */
			Observer();
			/**
			 *
			 */
			virtual ~Observer();
			//@}
			/**
			 * @name Operators
			 */
			//@{
			/**
			 * Compares the this pointer by default.
			 */
			bool operator==( const Observer& aObserver) const;
			/**
			 * Compares the this pointer by default.
			 */
			bool operator<( const Observer& aObserver) const;
			//@}
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 *
			 * @param aNotifier The Notifier this Observer will observe
			 */
			virtual void handleNotificationsFor( Notifier& aNotifier);
			/**
			 *
			 * @param aNotifier The Notifier this Observer will not observe anymore
			 */
			virtual void stopHandlingNotificationsFor( Notifier& aNotifier);
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 *
			 */
			virtual void handleNotification() = 0;
			//@}

		private:
			friend class Notifier;
	};
	// class Observer
} // namespace Base
#endif // DANU_OBSERVER_HPP_
