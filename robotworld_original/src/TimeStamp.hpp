#ifndef TIMESTAMP_HPP_
#define TIMESTAMP_HPP_

#include "Config.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

namespace Utils
{
	/**
	 *
	 */
	inline std::string TimeStamp()
	{
		auto local_time = boost::posix_time::microsec_clock::local_time();
		return boost::posix_time::to_simple_string(local_time);
	}
} /* namespace Utils */

#endif // SRC_TIMESTAMP_HPP_ 
