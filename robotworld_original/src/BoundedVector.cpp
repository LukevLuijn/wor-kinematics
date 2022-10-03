#include "BoundedVector.hpp"

#include "MathUtils.hpp"

#include <cmath>
#include <sstream>


namespace Model
{
	/**
	 *
	 */
	double BoundedVector::getMagnitude()
	{
		return std::sqrt( x * x + y * y);
	}
	/**
	 *
	 */
	void BoundedVector::normalise()
	{
		double magnitude = getMagnitude();

		if (magnitude <= Utils::ALMOST_ZERO) magnitude = 1;
		x /= magnitude;
		y /= magnitude;

		if (std::fabs( x) < Utils::ALMOST_ZERO) x = 0.0;
		if (std::fabs( y) < Utils::ALMOST_ZERO) y = 0.0;

	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	BoundedVector BoundedVector::getNormalised()
	{
		BoundedVector polarCoord( *this);
		polarCoord.normalise();
		return polarCoord;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void BoundedVector::reverse()
	{
		x = -x;
		y = -y;
	}
	/**
	 *
	 */
	BoundedVector& BoundedVector::operator=( const BoundedVector& aVector)
	{
		if(this != &aVector)
		{
			x = aVector.x;
			y = aVector.y;
		}
		return *this;
	}
	/**
	 *
	 */
	BoundedVector& BoundedVector::operator+=( const BoundedVector& aVector)
	{
		x += aVector.x;
		y += aVector.y;

		return *this;
	}
	/**
	 *
	 */
	BoundedVector& BoundedVector::operator-=( const BoundedVector& aVector)
	{
		x -= aVector.x;
		y -= aVector.y;

		return *this;
	}
	/**
	 *
	 */
	BoundedVector& BoundedVector::operator*=( float aScalar)
	{
		x *= aScalar;
		y *= aScalar;

		return *this;
	}
	/**
	 *
	 */
	BoundedVector& BoundedVector::operator/=( float aScalar)
	{
		x /= aScalar;
		y /= aScalar;

		return *this;
	}
	/**
	 *
	 */
	BoundedVector BoundedVector::operator-()
	{
		return BoundedVector( -x, -y);
	}
	/**
	 *
	 */
	std::string BoundedVector::asString() const
	{
		std::ostringstream os;

		os << "(" << x << "," << y << ")";

		return os.str();
	}
	/**
	 *
	 */
	std::string BoundedVector::asDebugString() const
	{
		return asString();
	}
} // namespace Model
/**
 *
 */
Model::BoundedVector operator+( const Model::BoundedVector& lhs,
								const Model::BoundedVector& rhs)
{
	return Model::BoundedVector( lhs.x + rhs.x, lhs.y + rhs.y);
}
/**
 *
 */
Model::BoundedVector operator-( const Model::BoundedVector& lhs,
								const Model::BoundedVector& rhs)
{
	return Model::BoundedVector( lhs.x - rhs.x, lhs.y - rhs.y);
}
/**
 *
 */
double operator*(	const Model::BoundedVector& lhs,
					const Model::BoundedVector& rhs)
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}
/**
 *
 */
Model::BoundedVector operator*( float s,
								const Model::BoundedVector& rhs)
{
	return rhs * s;
}
/**
 *
 */
Model::BoundedVector operator*( const Model::BoundedVector& lhs,
								float s)
{
	return Model::BoundedVector( lhs.x * s, lhs.y * s);
}
/**
 *
 */
Model::BoundedVector operator/( const Model::BoundedVector& lhs,
								float s)
{
	return Model::BoundedVector( lhs.x / s, lhs.y / s);
}
