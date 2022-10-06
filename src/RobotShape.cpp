#include "RobotShape.hpp"

#include "Goal.hpp"
#include "Logger.hpp"
#include "Notifier.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "RobotWorldCanvas.hpp"
#include "Shape2DUtils.hpp"

#include <cmath>

// TODO remove
#include "LidarSensor.h"
#include "AbstractSensor.hpp"

namespace View
{
	/**
	 *
	 */
	RobotShape::RobotShape( Model::RobotPtr aRobot) :
								RectangleShape( std::dynamic_pointer_cast<Model::ModelObject>(aRobot), aRobot->getPosition(), aRobot->getName()),
								robotWorldCanvas(nullptr)
	{
	}
	/**
	 *
	 */
	RobotShape::~RobotShape()
	{
	}
	/**
	 *
	 */
	Model::RobotPtr RobotShape::getRobot() const
	{
		return std::dynamic_pointer_cast<Model::Robot>(getModelObject());
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotShape::setRobot( Model::RobotPtr aRobot)
	{
		setModelObject(std::dynamic_pointer_cast<Model::ModelObject>(aRobot));
	}
	/**
	 *
	 */
	void RobotShape::handleActivated()
	{
		Model::GoalPtr goal = Model::RobotWorld::getRobotWorld().getGoal( "Goal");
		if (goal)
		{
			Point goalPosition = goal->getPosition();
			Point robotPosition = getRobot()->getPosition();
			getRobot()->setFront( Model::BoundedVector( goalPosition, robotPosition), false);
		}
	}
	/**
	 *
	 */
	void RobotShape::handleSelection()
	{
//		if (robotWorldCanvas->isShapeSelected() && robotWorldCanvas->getSelectedShape()->getObjectId() == getObjectId())
//		{
//			setSelected();
//		} else
//		{
//			setSelected(false);
//		}
	}
	/**
	 *
	 */
	void RobotShape::handleNotification()
	{
		setCentre( getRobot()->getPosition());
		robotWorldCanvas->handleNotification();
	}
	/**
	 *
	 */
	void RobotShape::draw( wxDC& dc)
	{
		// The minimum size of the RectangleShape is the size of the title
		titleSize = dc.GetTextExtent( WXSTRING( title));
		if (size.x < (titleSize.x + 2 * spacing + 2 * borderWidth))
		{
			size.x = titleSize.x + 2 * spacing + 2 * borderWidth;
		}
		if (size.y < (titleSize.y + 2 * spacing + 2 * borderWidth))
		{
			size.y = titleSize.y + 2 * spacing + 2 * borderWidth;
		}
		if (getRobot()->getSize() != size)
		{
			getRobot()->setSize( size, false);
		}

		PathAlgorithm::OpenSet openSet = getRobot()->getOpenSet();
		if (openSet.size() != 0)
		{
			dc.SetPen( wxPen( WXSTRING( "PALE GREEN"), borderWidth, wxPENSTYLE_SOLID));
			for (const PathAlgorithm::Vertex& vertex : openSet)
			{
				dc.DrawPoint( vertex.asPoint());
			}
		}

		PathAlgorithm::Path path = getRobot()->getPath();
		if (path.size() != 0)
		{
			dc.SetPen( wxPen( WXSTRING( "BLACK"), borderWidth, wxPENSTYLE_SOLID));
			for (const PathAlgorithm::Vertex& vertex : path)
			{
				dc.DrawPoint( vertex.asPoint());
			}
		}

		// Draws a rectangle with the given top left corner, and with the given size.
		dc.SetBrush( *wxWHITE_BRUSH);
		if (isSelected())
		{
			dc.SetPen( wxPen( WXSTRING( getSelectionColour()), borderWidth, wxPENSTYLE_SOLID));
		} else
		{
			dc.SetPen( wxPen( WXSTRING( getNormalColour()), borderWidth, wxPENSTYLE_SOLID));
		}

		Point cornerPoints[] = { getRobot()->getFrontRight(), getRobot()->getFrontLeft(), getRobot()->getBackLeft(), getRobot()->getBackRight() };
		dc.DrawPolygon( 4, cornerPoints);

		dc.SetPen( wxPen( WXSTRING( "RED"), borderWidth, wxPENSTYLE_SOLID));
		dc.DrawPoint( cornerPoints[1]);
		dc.SetPen( wxPen( WXSTRING( "GREEN"), borderWidth, wxPENSTYLE_SOLID));
		dc.DrawPoint( cornerPoints[0]);
		dc.SetPen( wxPen( WXSTRING( "INDIAN RED"), borderWidth, wxPENSTYLE_SOLID));
		dc.DrawPoint( cornerPoints[2]);
		dc.SetPen( wxPen( WXSTRING( "PALE GREEN"), borderWidth, wxPENSTYLE_SOLID));
		dc.DrawPoint( cornerPoints[3]);

		double angle = Utils::Shape2DUtils::getAngle( getRobot()->getFront()) + 0.5 * Utils::PI;

		dc.SetPen( wxPen( WXSTRING( "BLACK"), 1, wxPENSTYLE_SOLID));
		dc.DrawLine( centre.x,
					 centre.y,
					 static_cast<int>(centre.x + std::cos( angle - 0.5 * Utils::PI) * 25),
					 static_cast<int>(centre.y + std::sin( angle - 0.5 * Utils::PI) * 25));

		// Bounty of 0.25 points for anyone who makes the name turn
		// with the front of the robot, while text centre being displayed in the
		// centre of the robot, bottom of the text to the back of the robot.
		dc.DrawRotatedText( WXSTRING( title), centre.x - titleSize.x / 2, centre.y - titleSize.y / 2, angle - Utils::PI);
	}
	/**
	 *
	 */
	bool RobotShape::occupies( const Point& aPoint) const
	{
		Point cornerPoints[] = { getRobot()->getFrontRight(), getRobot()->getFrontLeft(), getRobot()->getBackLeft(), getRobot()->getBackRight() };
		return Utils::Shape2DUtils::isInsidePolygon( cornerPoints, 4, aPoint);
	}
	/**
	 *
	 */
	void RobotShape::setCentre( const Point& aPoint)
	{
		getRobot()->setPosition( aPoint, false);
		RectangleShape::setCentre( getRobot()->getPosition());
	}
	/**
	 *
	 */
	std::string RobotShape::asString() const
	{
		std::ostringstream os;

		os << "RobotShape " << RectangleShape::asString();

		return os.str();
	}
	/**
	 *
	 */
	std::string RobotShape::asDebugString() const
	{
		std::ostringstream os;

		os << "RobotShape:\n";
		os << RectangleShape::asDebugString() << "\n";
		if (getRobot())
		{
			os << getRobot()->asDebugString();
		}

		return os.str();
	}
} // namespace View
