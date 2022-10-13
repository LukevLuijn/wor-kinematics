#ifndef BOUNDEDVECTOR_HPP_
#define BOUNDEDVECTOR_HPP_

#include "Config.hpp"

#include "Point.hpp"

#include <string>

namespace Model
{
    /**
 *
 */
    class BoundedVector
    {
    public:
        /**
		 *
		 */
        BoundedVector() : x(0.0), y(0.0)
        {
        }
        /**
		 *
		 */
        BoundedVector(double anX, double anY) : x(anX), y(anY)
        {
        }
        /**
		 *
		 */
        BoundedVector(const Point& aPoint1, const Point& aPoint2) : x(aPoint1.x - aPoint2.x), y(aPoint1.y - aPoint2.y)
        {
        }
        /**
		 *
		 */
        BoundedVector(const BoundedVector& aPolarCoord) : x(aPolarCoord.x), y(aPolarCoord.y)
        {
        }
        /**
		 *
		 */
        virtual ~BoundedVector()
        {
        }
        /**
		 *
		 */
        double getMagnitude();
        /**
		 *
		 */
        void normalise();
        /**
		 *
		 */
        BoundedVector getNormalised();
        /**
		 *
		 */
        void reverse();
        /**
		 *
		 */
        BoundedVector& operator=(const BoundedVector& aVector);
        /**
		 *
		 */
        BoundedVector& operator+=(const BoundedVector& aVector);
        /**
		 *
		 */
        BoundedVector& operator-=(const BoundedVector& aVector);
        /**
		 *
		 */
        BoundedVector& operator*=(float aScalar);
        /**
		 *
		 */
        BoundedVector& operator/=(float aScalar);
        /**
		 *
		 */
        BoundedVector operator-();

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

        // No need for accessors and mutators?
        double x;
        double y;
    };
    // class PolarCoord
}//namespace Model

// Some global operators
Model::BoundedVector operator+(const Model::BoundedVector& lhs, const Model::BoundedVector& rhs);
Model::BoundedVector operator-(const Model::BoundedVector& lhs, const Model::BoundedVector& rhs);
double operator*(const Model::BoundedVector& lhs, const Model::BoundedVector& rhs);

Model::BoundedVector operator*(float s, const Model::BoundedVector& rhs);
Model::BoundedVector operator*(const Model::BoundedVector& lhs, float s);

Model::BoundedVector operator/(const Model::BoundedVector& lhs, float s);

#endif// BOUNDEDVECTOR_HPP_
