#ifndef COMMANDLINEARGUMENT_HPP_
#define COMMANDLINEARGUMENT_HPP_

#include "Config.hpp"

#include <limits>
#include <string>

namespace Application
{
    /**
	 * The CommandlineArgument class encapsulates any command line argument given to the application
	 */
    class CommandlineArgument
    {
    public:
        /**
			 * @name Constructors and destructor
			 */
        //@{
        CommandlineArgument() : argumentNumber(std::numeric_limits<unsigned long>::max()), variable(), value()
        {
        }
        /**
			 *
			 * @param anArgumentNumber
			 * @param aVariable
			 * @param aValue
			 */
        CommandlineArgument(unsigned long anArgumentNumber, const std::string& aVariable, const std::string& aValue)
            : argumentNumber(anArgumentNumber), variable(aVariable), value(aValue)
        {
        }
        /**
			 *
			 * @param aCommandlineArgument
			 */
        CommandlineArgument(const CommandlineArgument& aCommandlineArgument)
            : argumentNumber(aCommandlineArgument.argumentNumber), variable(aCommandlineArgument.variable),
              value(aCommandlineArgument.value)
        {
        }
        /**
			 *
			 */
        virtual ~CommandlineArgument()
        {
        }
        //@}

        /**
			 * @name Operators
			 */
        //@{
        /**
			 *
			 */
        CommandlineArgument& operator=(const CommandlineArgument& aCommandlineArgument)
        {
            if (this != &aCommandlineArgument)
            {
                argumentNumber = aCommandlineArgument.argumentNumber;
                variable = aCommandlineArgument.variable;
                value = aCommandlineArgument.value;
            }
            return *this;
        }
        /**
			 *
			 */
        bool operator==(const unsigned long anArgumentNumber) const
        {
            return argumentNumber == anArgumentNumber;
        }
        /**
			 *
			 */
        bool operator==(const std::string& aVariable) const
        {
            return variable == aVariable;
        }
        /**
			 *
			 */
        bool operator==(const CommandlineArgument& aCommandlineArgument) const
        {
            return (argumentNumber == aCommandlineArgument.argumentNumber) &&
                   (variable == aCommandlineArgument.variable) && (value == aCommandlineArgument.value);
        }
        /**
			 *	Only compares the argument number.
			 */
        bool operator<(const CommandlineArgument& aCommandlineArgument) const
        {
            return argumentNumber < aCommandlineArgument.argumentNumber;
        }
        //@}

        unsigned long argumentNumber;
        std::string variable;
        std::string value;

    protected:
    private:
    };
    // class CommandlineArgument
}// namespace Application
#endif// COMMANDLINEARGUMENT_HPP_
