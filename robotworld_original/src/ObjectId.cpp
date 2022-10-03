#include "ObjectId.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/time_zone_base.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include <sys/timeb.h>
#include <ctime>
#include <climits>
#include <mutex>

namespace Base
{
	/**
	 * newObjectId() uses gmtime() as it is faster than localtime().
	 * We just calculates our offset to GMT once and cache it
	 */
	struct GMTOffset
	{
			GMTOffset()
			{
				// Get the current time in UTC and local form.
				auto utc_time = boost::posix_time::microsec_clock::universal_time();
				auto local_time = boost::date_time::c_local_adjustor< boost::posix_time::ptime >::utc_to_local( utc_time);

				// Calculate the difference in milliseconds between local time and the UTC time.
				auto time_t_diff = boost::posix_time::to_time_t( local_time) - boost::posix_time::to_time_t( utc_time);

				offset = time_t_diff / 3600; // @suppress("Avoid magic numbers")
			}
			long offset;

	};
	static GMTOffset gmtOffset;

	/**
	 *
	 */
	/* static */ std::string ObjectId::objectIdNamespace = "";

	static std::mutex newObjectIdMutex;

	/* static */ObjectId ObjectId::newObjectId()
	{
		std::lock_guard< std::mutex > lock( newObjectIdMutex);

		static time_t currentSecond = 0;
		static unsigned long fraction = 0;

		time_t newSecond = 0;

		struct timespec now;

		clock_gettime(CLOCK_REALTIME, &now);
		newSecond = now.tv_sec;

		// Reset seconds and fraction if in a new second
		if (currentSecond < newSecond)
		{
			currentSecond = newSecond;
			fraction = 0;
		}

		// Prevent overflow to 0 within the second. This should only happen on fast machines
		// as the creation of the fraction part is processor speed dependent
		if (fraction == ULONG_MAX)
		{
			// Wait for the next second
			do
			{
				clock_gettime(CLOCK_REALTIME, &now);
				newSecond = now.tv_sec;
			}while(currentSecond >= newSecond);
			currentSecond = newSecond;
			fraction = 0;
		}

		// See https://stackoverflow.com/questions/41544774/localtime-takes-24-times-more-than-gmtime-performance-issue-on-linux
		// for using gmtime + offset
		//struct tm* timeStruct = localtime( &currentSecond); //
		struct tm tm = *gmtime( &currentSecond);

		// We actually only use 27 chars but it could be 93, based on the maximum int values....
		char timestampBuffer[93] = {'\0'};
		sprintf( timestampBuffer,
				 "%04d-%02d-%02d-%02ld-%02d-%02d-%06lu",
				 tm.tm_year + 1900, // @suppress("Avoid magic numbers")
				 tm.tm_mon + 1,
				 tm.tm_mday,
				 tm.tm_hour + gmtOffset.offset,
				 tm.tm_min,
				 tm.tm_sec,
				 fraction++);

		return ObjectId(timestampBuffer);
	}

	ObjectId::~ObjectId()
	{
	}
	/**
	 *
	 */
	bool ObjectId::operator==( const ObjectId& anObjectId) const
	{
		return (static_cast<base>(*this) == static_cast<base>(anObjectId));
	}
	/**
	 *
	 */
	bool ObjectId::operator<( const ObjectId& anObjectId) const
	{
		return (static_cast<base>(*this) < static_cast<base>(anObjectId));
	}
	/**
	 *
	 */
	bool ObjectId::isNull() const
	{
		if (base::empty())
		{
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	bool ObjectId::isValid() const
	{
		if (isNull())
		{
			return false;
		}
		return true;
	}
	/**
	 *
	 */
	/*virtual*/std::string ObjectId::asString() const
	{
		if (isNull())
		{
			return "";
		}
		return std::string( base::begin(), base::end());
	}

	/**
	 *
	 */
	/*virtual*/std::string ObjectId::asDebugString() const
	{
		return asString();
	}

	std::ostream& operator<<( 	std::ostream& os,
								const ObjectId& anObjectId)
	{
		return os << anObjectId.asString();
	}
} // namespace Base
