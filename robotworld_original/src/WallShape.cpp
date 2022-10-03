#include "WallShape.hpp"

#include "Logger.hpp"
#include "Shape2DUtils.hpp"

#include <sstream>

namespace View
{
	/**
	 *
	 */
	WallShape::WallShape( 	Model::WallPtr aWall) :
								LineShape( std::dynamic_pointer_cast<Model::ModelObject>(aWall),
										   RectangleShapePtr(new RectangleShape(aWall->getPoint1())),
										   RectangleShapePtr(new RectangleShape(aWall->getPoint2())),
										   "", 1, 0)
	{
	}
	/**
	 *
	 */
	WallShape::WallShape( 	Model::WallPtr aWall,
							RectangleShapePtr aRectangleShape1,
							RectangleShapePtr aRectangleShape2) :
								LineShape( std::dynamic_pointer_cast<Model::ModelObject>(aWall),
										   aRectangleShape1,
										   aRectangleShape2,
										   "", 1, 0)
	{
	}
	/**
	 *
	 */
	WallShape::~WallShape()
	{
	}
	/**
	 *
	 */
	Model::WallPtr WallShape::getWall() const
	{
		return std::dynamic_pointer_cast<Model::Wall>(getModelObject());
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void WallShape::setWall( Model::WallPtr aWall)
	{
		setModelObject(std::dynamic_pointer_cast<Model::ModelObject>(aWall));
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void WallShape::setSelectedAt(	const Point& aPoint /*= DefaultPosition*/,
									bool aSelected /*= true*/)
	{
		UNUSEDCAST( aPoint);
		setSelected( aSelected);
	}
	/**
	 *
	 */
	RectangleShapePtr WallShape::hasEndPointAt( const Point& aPoint)
	{
		if (getNode1()->occupies( aPoint))
		{
			return getNode1();
		}
		if (getNode2()->occupies( aPoint))
		{
			return getNode2();
		}
		return nullptr;
	}
	/**
	 *
	 */
	bool WallShape::hasEndPoint( RectangleShapePtr aRectangleShape)
	{
		return getNode1()->getObjectId() == aRectangleShape->getObjectId() || getNode2()->getObjectId() == aRectangleShape->getObjectId();
	}

	/**
	 *
	 */
	void WallShape::updateEndPoint( RectangleShapePtr aRectangleShape)
	{
		if (getNode1()->getObjectId() == aRectangleShape->getObjectId())
		{
			getWall()->setPoint1( aRectangleShape->getCentre(), false);
			return;
		}
		if (getNode2()->getObjectId() == aRectangleShape->getObjectId())
		{
			getWall()->setPoint2( aRectangleShape->getCentre(), false);
			return;
		}
	}
	/**
	 *
	 */
	void WallShape::draw( wxDC& dc)
	{
		if (isSelected())
		{
			dc.SetPen( wxPen( WXSTRING( "RED"), getLineWidth(), wxPENSTYLE_SOLID));
		} else
		{
			dc.SetPen( wxPen( WXSTRING( "BLACK"), getLineWidth(), wxPENSTYLE_SOLID));
		}

		dc.DrawLine( getNode1()->getCentre().x, getNode1()->getCentre().y, getNode2()->getCentre().x, getNode2()->getCentre().y);
	}
	/**
	 *
	 */
	bool WallShape::occupies( const Point& aPoint) const
	{
		if (getNode1()->occupies( aPoint) || getNode2()->occupies( aPoint) || Utils::Shape2DUtils::isOnLine( getBegin(), getEnd(), aPoint, 2))
		{
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	std::string WallShape::asString() const
	{
		std::ostringstream os;
		os << LineShape::asDebugString() << " WallShape: " << Utils::Shape2DUtils::asString( getBegin()) << "->" << Utils::Shape2DUtils::asString( getEnd());
		return os.str();
	}
	/**
	 *
	 */
	std::string WallShape::asDebugString() const
	{
		std::ostringstream os;
		os << "WallShape:\n";
		os << LineShape::asDebugString() << "\n";
		if (getWall())
		{
			os << getWall()->asDebugString();
		}
		return os.str();
	}
} // namespace View
