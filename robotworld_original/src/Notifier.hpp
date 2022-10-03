#ifndef NOTIFIER_HPP_
#define NOTIFIER_HPP_

#include "Config.hpp"

#include "Observer.hpp"

#include <string>
#include <vector>

namespace Base
{
	class Observer;

	/**
	 * The Notifier class is part of a straight forward implementation of the Observer/Notifier pattern
	 *
	 * @see Observer
	 */
	class Notifier
	{
		public:
			/**
			 * @name Constructors and destructor
			 */
			//@{
			/**
			 * Default constructor, enables notification by default.
			 *
			 * @param enable If true, notification is enabled.
			 */
			Notifier( bool enable = true);
			/**
			 *
			 */
			virtual ~Notifier();
			//@}

			/**
			 * @name Notifier functions
			 */
			//@{
			/**
			 * En- or disables the notification by this Notifier
			 *
			 * @param enable If true (default) enables the notification for this Notifier
			 */
			virtual void enableNotification( bool enable = true);
			/**
			 * Disables the notification by this Notifier
			 */
			virtual void disableNotification();
			/**
			 *
			 * @return True if notification is enabled for this Notifier, false otherwise
			 */
			virtual bool isEnabledForNotification() const;
			/**
			 * Adds the Observer to the list of Observers if not in the list yet
			 *
			 * @param anObserver The observer to add
			 */
			virtual void addObserver( Observer& anObserver);
			/**
			 * Removes the Observer from the list of Observers if in the list
			 *
			 * @param anObserver The Observer to remove
			 */
			virtual void removeObserver( Observer& anObserver);
			/**
			 * Removes all observer from the list of Observers
			 */
			virtual void removeAllObservers();
			/**
			 * Notifies all observers
			 */
			virtual void notifyObservers();
			//@}

			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const;
			//@}

		private:
			bool notify;
			std::vector< Observer* > observers;

	};
	// class Notifier
} // namespace Base
#endif // DANU_NOTIFIER_HPP_
