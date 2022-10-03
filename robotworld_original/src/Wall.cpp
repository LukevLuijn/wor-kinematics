#include "Wall.hpp"

#include "Logger.hpp"
#include "Shape2DUtils.hpp"

#include <sstream>

namespace Model
{
	/**
	 *
	 */
	Wall::Wall()
	{
	}
	/**
	 *
	 */
	Wall::Wall( const Point& aPoint1,
				const Point& aPoint2) :
								point1( aPoint1),
								point2( aPoint2)
	{
	}
	/**
	 *
	 */
	Wall::~Wall()
	{
	}
	/**
	 *
	 */
	void Wall::setPoint1(	const Point& aPoint1,
							bool aNotifyObservers /*= true*/)
	{
		point1 = aPoint1;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	void Wall::setPoint2(	const Point& aPoint2,
							bool aNotifyObservers /*= true*/)
	{
		point2 = aPoint2;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	std::string Wall::asString() const
	{
		std::ostringstream os;

		os << "Wall: " << ModelObject::asString() << "," << Utils::Shape2DUtils::asString( point1) << " - " << Utils::Shape2DUtils::asString( point2);

		return os.str();
	}
	/**
	 *
	 */
	std::string Wall::asDebugString() const
	{
		std::ostringstream os;

		os << "Wall:\n";
		os << ModelObject::asDebugString() << "\n"<< Utils::Shape2DUtils::asString( point1) << " - " << Utils::Shape2DUtils::asString( point2);

		return os.str();
	}
} // namespace Model
