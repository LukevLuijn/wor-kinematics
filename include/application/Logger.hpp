#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "Config.hpp"

#include <iomanip>
#include <string>
#include <vector>


#define EXPAND(x) x
#define GET_MACRO(_1, _2, NAME, ...) NAME

#define LOG1(a) Application::Logger::log(a, __func__, __FILE__,__LINE__)
#define LOG2(a, b) Application::Logger::log(a, b, __func__,__FILE__,__LINE__)
#define LOG(...) EXPAND(GET_MACRO(__VA_ARGS__, LOG2, LOG1)(__VA_ARGS__))


namespace Application
{
	/**
	 *
	 */
	class Logger
	{
		public:
			/**
			 * If enabled, traces the message to the current ITraceFunction
			 *
			 * @param aMessage
			 */
			static void log( const std::string& aMessage);
            /**
             * If enabled, traces the message to the current ITraceFunction, also automatically adds function and line number of function.
             *
             * @param aMessage
             * @param func
             * @param lineNum
             */
            static void log ( const std::string& aMessage, const std::string& func, const std::string& file, int lineNum);

            template<typename T>
            static void log ( const std::string& aMessage, T var, const std::string& func, const std::string& file, int lineNum)
            {
                log (aMessage, std::vector<T>({var}), func, file, lineNum);
            }
            template<typename T>
            static void log ( const std::string& aMessage, std::vector<T> vars, const std::string& func,const std::string& file,  int lineNum)
            {
                std::string message = aMessage + " (";
                for (auto it = vars.begin(); it != vars.end(); ++it)
                {
                    std::ostringstream os;
                    os << std::fixed;
                    os << std::setprecision(2);
                    os << *it;

                    message += os.str() + ((std::next(it, 1) == vars.end()) ? "" : ", ");
                }
                message += ")";

                log (message, func, file, lineNum);
            }


        /**
			 *
			 * Disable/enable the logger. Called with true (default) enables the logger, with false disables the logger.
			 *
			 * @param aDisable, by default true
			 */
			static void setDisable( bool aDisable = true)
			{
				disable = aDisable;
			}
			/**
			 *
			 * @return true if enabled, false otherwise
			 */
			static bool isEnabled()
			{
				return !disable;
			}
			/**
			 *
			 */
		private:
			static bool disable;
	};
} // namespace Application
#endif /* LOGGER_HPP_ */
