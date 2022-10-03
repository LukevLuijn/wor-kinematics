#include "Shape2DUtils.hpp"

#include "Widgets.hpp"

#include <algorithm>
#include <sstream>

namespace Utils
{
	/**
	 *
	 */
	double GetAngle(	const Point& aStartpoint,
						const Point& anEndPoint)
	{
		double dX = anEndPoint.x - aStartpoint.x;
		double dY = anEndPoint.y - aStartpoint.y;

		//double s = std::sqrt( (dX*dX) + (dY*dY));

		double angle = std::atan2( dY, dX);

		if (angle < 0)
		{
			angle = 2.0 * PI + angle; // @suppress("Avoid magic numbers")
		}
		return angle;
	}

	/**
	 *
	 */
	/* static */double Shape2DUtils::getAngle( const Model::BoundedVector& aVector)
	{
		return GetAngle( Point( 0, 0), Point( static_cast<int>(aVector.x), static_cast<int>(aVector.y)));
	}
	/**
	 *
	 */
	/* static */double Shape2DUtils::getAngle(	const Point& aStartpoint,
												const Point& anEndPoint)
	{
		return GetAngle( aStartpoint, anEndPoint);
	}

	/**
	 *
	 */
	/* static */bool Shape2DUtils::intersect(	const Point& aStartLine1,
												const Point& aEndLine1,
												const Point& aStartLine2,
												const Point& anEndLine2)
	{
		double x1 = aStartLine1.x;
		double x2 = aEndLine1.x;
		double x3 = aStartLine2.x;
		double x4 = anEndLine2.x;
		double y1 = aStartLine1.y;
		double y2 = aEndLine1.y;
		double y3 = aStartLine2.y;
		double y4 = anEndLine2.y;

		double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (std::fabs(d-0.0) <= std::numeric_limits<float>::epsilon())
			return false;

		// Get the x and y
		double pre = (x1 * y2 - y1 * x2);
		double post = (x3 * y4 - y3 * x4);
		double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
		double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

		// Check if the x and y coordinates are within both lines
		if (x < std::min( x1, x2) || x > std::max( x1, x2) || x < std::min( x3, x4) || x > std::max( x3, x4))
		{
			return false;
		}
		if (y < std::min( y1, y2) || y > std::max( y1, y2) || y < std::min( y3, y4) || y > std::max( y3, y4))
		{
			return false;
		}

		return true;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	/* static */Point Shape2DUtils::getIntersection(	const Point& aStartLine1,
														const Point& aEndLine1,
														const Point& aStartLine2,
														const Point& anEndLine2)
	{
		double x1 = aStartLine1.x;
		double x2 = aEndLine1.x;
		double x3 = aStartLine2.x;
		double x4 = anEndLine2.x;
		double y1 = aStartLine1.y;
		double y2 = aEndLine1.y;
		double y3 = aStartLine2.y;
		double y4 = anEndLine2.y;

		double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (std::fabs(d-0.0) <= std::numeric_limits<float>::epsilon())
			return DefaultPosition;

		// Get the x and y
		double pre = (x1 * y2 - y1 * x2);
		double post = (x3 * y4 - y3 * x4);
		double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
		double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

		// Check if the x and y coordinates are within both lines
		if (x < std::min( x1, x2) || x > std::max( x1, x2) || x < std::min( x3, x4) || x > std::max( x3, x4))
		{
			return DefaultPosition;
		}
		if (y < std::min( y1, y2) || y > std::max( y1, y2) || y < std::min( y3, y4) || y > std::max( y3, y4))
		{
			return DefaultPosition;
		}

		// Return the point of intersection
		return Point( static_cast<int>(x), static_cast<int>(y));
	}
	/**
	 *
	 */
	/* static */bool Shape2DUtils::isInsidePolygon( const Point* aPolygon,
													int aNumberOfPoints,
													const Point& aPoint)
	{
		int counter = 0;
		Point p1 = aPolygon[0];
		Point p2;

		for (int i = 1; i <= aNumberOfPoints; i++)
		{
			p2 = aPolygon[i % aNumberOfPoints];
			if (aPoint.y > std::min( p1.y, p2.y))
			{
				if (aPoint.y <= std::max( p1.y, p2.y))
				{
					if (aPoint.x <= std::max( p1.x, p2.x))
					{
						if (p1.y != p2.y)
						{
							double dx = p2.x - p1.x;
							double dy = p2.y - p1.y;
							double xinters = (dx / dy) * (aPoint.y - p1.y) + p1.x;
							if (p1.x == p2.x || aPoint.x <= xinters)
							{
								counter++;
							}
						}
					}
				}
			}
			p1 = p2;
		}

		if (counter % 2 == 0)
		{
			return false;
		}
		return true;
	}
	/**
	 *
	 */
	/* static */bool Shape2DUtils::isOnLine(	const Point& aStartPoint,
												const Point& anEndPoint,
												const Point& aPoint,
												int aRadius /* = 6 */)
	{
		if (aPoint.x < (std::min( aStartPoint.x, anEndPoint.x) - aRadius))
		{
			return false;
		}
		if (aPoint.x > (std::max( aStartPoint.x, anEndPoint.x) + aRadius))
		{
			return false;
		}

		if (aPoint.y < (std::min( aStartPoint.y, anEndPoint.y) - aRadius))
		{
			return false;
		}
		if (aPoint.y > (std::max( aStartPoint.y, anEndPoint.y) + aRadius))
		{
			return false;
		}

		double normalLength = std::sqrt( (aStartPoint.x - anEndPoint.x) * (aStartPoint.x - anEndPoint.x) + (aStartPoint.y - anEndPoint.y) * (aStartPoint.y - anEndPoint.y));

		double distance = std::abs( (aPoint.x - anEndPoint.x) * (aStartPoint.y - anEndPoint.y) - (aPoint.y - anEndPoint.y) * (aStartPoint.x - anEndPoint.x)) / normalLength;

		return distance < aRadius;

		// After trying it myself I found someone who made less errors...
		// See http://local.wasp.uwa.edu.au/~pbourke/geometry/sphereline/

		// Basically it solves
		// y = ax+b through aStartPoint and anEndPoint
		// and
		// (x^2-p) + (y^2-q) = r^2 at aPoint with aRadius
		// which, after substitution leads to ax^2+bx+c=0
		// If this has no solution the line does not intersect with the circle
		// If it has 1 solution the line is the tangent
		// If it has 2 solutions the line intersects with 2 points on the circle

		// Must use long long as int is to small????
		// P1 at (x1,y1), start of the line
		//	long long x1 = aStartPoint.x;
		//	long long y1 = aStartPoint.y;
		// P2 at (x2,y2 ), end of the line
		//	long long x2 = anEndPoint.x;
		//	long long y2 = anEndPoint.y;
		// P3 at (x3,y3), the clicking point
		//	long long x3 = aPoint.x;
		//	long long y3 = aPoint.y;
		// radius, the amount of pixels we are allowed to miss
		//	int r = aRadius;
		//
		//	long long a = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
		//	long long b = 2 * ((x2 - x1) * (x1 - x3) + (y2 - y1) * (y1 - y3));
		//	long long c = (x3 * x3) + (y3 * y3) + (x1 * x1) + (y1 * y1) - (2 * ((x3 * x1) + (y3 * y1))) - (r * r);
		//	long long i = (b * b) - (4 * a * c);
		//
		//	if (i < 0)
		//	{
		// No intersection
		//		return false;
		//	} else //  ( i >= 0 )
		//	{
		// 0 = 1 intersection, > 0 = 2 intersections
		//		return true;
		//	}
	}
	/**
	 *
	 */
	/* static */bool Shape2DUtils::isOnLine( 	const Point* aPolygon,
												int aNumberOfPoints,
												const Point& aPoint,
												int aRadius /* = 6 */,
												bool aClosedShape /* = true */)
	{
		Point p1 = aPolygon[0];
		Point p2;

		int actualNumberOfPoints = aNumberOfPoints + aClosedShape;
		for (int i = 1; i < actualNumberOfPoints; i++)
		{
			p2 = aPolygon[i % aNumberOfPoints];
			if (Shape2DUtils::isOnLine( p1, p2, aPoint, aRadius))
			{
				return true;
			}
			p1 = p2;
		}
		return false;
	}
	/**
	 *
	 */
	/* static */Point Shape2DUtils::rotate( const Point& aPoint,
											double anAngle)
	{
		return Point( static_cast<int>(Shape2DUtils::rotateX( aPoint, anAngle)), static_cast<int>(rotateY( aPoint, anAngle)));
	}
	/**
	 *
	 */
	/* static */Shape2DUtils::CompassPoint Shape2DUtils::getCompassPoint(	const Point& aPoint,
																			const Size& aSize,
																			const Point& aBorderPoint,
																			int aRadius /*= 3*/)
	{
		Point centre = aPoint;
		Size size = aSize;
		Point northWest( centre.x - (size.x / 2), centre.y - (size.y / 2)); // upper left
		Point northEast( centre.x + (size.x / 2), centre.y - (size.y / 2)); // upper right
		Point southEast( centre.x + (size.x / 2), centre.y + (size.y / 2)); // right under
		Point southWest( centre.x - (size.x / 2), centre.y + (size.y / 2)); // left under

		if (aBorderPoint == northWest)
		{
			return CP_NORTHWEST;
		} else if (aBorderPoint == northEast)
		{
			return CP_NORTHEAST;
		} else if (aBorderPoint == southEast)
		{
			return CP_SOUTHEAST;
		} else if (aBorderPoint == southWest)
		{
			return CP_SOUTHWEST;
		} else if (Shape2DUtils::isOnLine( northWest, northEast, aBorderPoint, aRadius))
		{
			return CP_NORTH;
		} else if (Shape2DUtils::isOnLine( northEast, southEast, aBorderPoint, aRadius))
		{
			return CP_EAST;
		} else if (Shape2DUtils::isOnLine( southEast, southWest, aBorderPoint, aRadius))
		{
			return CP_SOUTH;
		} else if (Shape2DUtils::isOnLine( southWest, northWest, aBorderPoint, aRadius))
		{
			return CP_WEST;
		}

		return CP_NORTH;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	/* static */bool Shape2DUtils::isCompassPoint(	const Point& aPoint,
													const Size& aSize,
													const Point& aBorderPoint,
													CompassPoint aCompassPoint,
													int aRadius /* = 6 */)
	{
		bool result = getCompassPoint( aPoint, aSize, aBorderPoint, aRadius) == aCompassPoint;
		if (result == true)
		{
			return result;
		}
		return result;
		//return getCompassPoint( aPoint, aSize, aBorderPoint,aRadius) == aCompassPoint;
	}
	/**
	 *
	 */
	/* static */std::string Shape2DUtils::asString( const Point& aPoint)
	{
		std::ostringstream os;
		os << "(" << aPoint.x << "," << aPoint.y << ")";
		return os.str();
	}
	/**
	 *
	 */
	/* static */std::string Shape2DUtils::asString( const Size& aSize)
	{
		std::ostringstream os;
		os << aSize.x << " x " << aSize.y;
		return os.str();
	}
	/**
	 *
	 */
	/* static */double Shape2DUtils::rotateX( 	int anX,
												int anY,
												double anAngle)
	{
		return (anX * cos( anAngle) - anY * sin( anAngle));
	}
	/**
	 *
	 */
	/* static */double Shape2DUtils::rotateY( 	int anX,
												int anY,
												double anAngle)
	{
		return (anX * sin( anAngle) + anY * cos( anAngle));
	}
	/**
	 *
	 */
	/* static */double Shape2DUtils::rotateX(	const Point& aPoint,
												double anAngle)
	{
		return Shape2DUtils::rotateX( aPoint.x, aPoint.y, anAngle);
	}
	/**
	 *
	 */
	/* static */double Shape2DUtils::rotateY(	const Point& aPoint,
												double anAngle)
	{
		return Shape2DUtils::rotateY( aPoint.x, aPoint.y, anAngle);
	}
} // namespace Utils

