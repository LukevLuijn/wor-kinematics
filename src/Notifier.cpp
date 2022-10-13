#include "Notifier.hpp"

#include "Logger.hpp"
#include "Observer.hpp"

#include <algorithm>
#include <sstream>
#include <typeinfo>

namespace Base
{
    /**
	 *
	 */
    Notifier::Notifier(bool enable /*= true*/) : notify(enable)
    {
    }
    /**
	 *
	 */
    Notifier::~Notifier()
    {
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    void Notifier::enableNotification(bool enable /* = true */)
    {
        notify = enable;
    }
    /**
	 *
	 */
    void Notifier::disableNotification()
    {
        notify = false;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    bool Notifier::isEnabledForNotification() const
    {
        return notify;
    }
    /**
	 *	The implementation of operator== uses pointer comparison!
	 */
    void Notifier::addObserver(Observer& anObserver)
    {
        if (std::find_if(observers.begin(), observers.end(), [&anObserver](const Observer* observer) {
                return *observer == anObserver;
            }) != observers.end())
        {
            return;
        }

        observers.push_back(&anObserver);
    }
    /**
	 *	The implementation of operator== uses pointer comparison!
	 */
    void Notifier::removeObserver(Observer& anObserver)
    {
        for (std::vector<Observer*>::iterator i = observers.begin(); i != observers.end(); ++i)
        {
            if (*(*i) == anObserver)
            {
                observers.erase(i);
                break;
            }
        }
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    void Notifier::removeAllObservers()
    {
        observers.erase(observers.begin(), observers.end());
    }
    /**
	 *
	 */
    void Notifier::notifyObservers()
    {
        if (notify)
        {
            for (Observer* observer : observers)
            {
                observer->handleNotification();
            }
        }
    }
    /**
	 *
	 */
    std::string Notifier::asString() const
    {
        std::ostringstream os;
        os << typeid(*this).name();
        return os.str();
    }
    /**
	 *
	 */
    std::string Notifier::asDebugString() const
    {
        return asString();
    }
}//namespace Base
