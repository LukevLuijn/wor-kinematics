#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "Config.hpp"

#include <string>

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
