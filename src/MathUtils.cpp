#include "MathUtils.hpp"

namespace Utils
{
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	/* static */ double MathUtils::toRadians( double aDegrees)
	{
		return aDegrees * PI / 180.0;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	/* static */ double MathUtils::toDegrees( double aRadian)
	{
		return aRadian * 180.0 / PI;
	}
} //namespace Utils
