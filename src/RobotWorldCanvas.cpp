#include "RobotWorldCanvas.hpp"

#include "Goal.hpp"
#include "GoalShape.hpp"
#include "LineShape.hpp"
#include "Logger.hpp"
#include "NotificationEvent.hpp"
#include "RectangleShape.hpp"
#include "Robot.hpp"
#include "RobotShape.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "WallShape.hpp"
#include "WayPoint.hpp"
#include "WayPointShape.hpp"

#include <algorithm>

namespace View
{
	enum
	{
		ID_ABOUT,
		ID_ADD_ROBOT,
		ID_EDIT_ROBOT,
		ID_DELETE_ROBOT,
		ID_ADD_WAYPOINT,
		ID_EDIT_WAYPOINT,
		ID_DELETE_WAYPOINT,
		ID_ADD_GOAL,
		ID_EDIT_GOAL,
		ID_DELETE_GOAL,
		ID_ADD_WALL,
		ID_EDIT_WALL,
		ID_DELETE_WALL,
		ID_SHAPE_INFO,
		ID_WORLD_INFO
	};

	/**
	 *
	 */
	RobotWorldCanvas::RobotWorldCanvas( Window* anOwner) :
								ScrolledCanvas( anOwner, DEFAULT_ID),
								popupPoint( DefaultPosition),
								startActionPoint( DefaultPosition),
								endActionPoint( DefaultPosition),
								actionOffset( DefaultPosition),
								startActionSize( DefaultSize),
								startActionShape( nullptr),
								endActionShape( nullptr),
								selectedShape( nullptr),
								activationEnabled( false),
								selectionEnabled( false),
								menuItemEnabled( false),
								dandEnabled( true),
								notificationHandler( nullptr)
	{
		// CppCheck gives a "virtualCallInConstructor" on initialise(). I don't know why.
		// It cannot be suppressed by a "cppcheck-suppress virtualCallInConstructor" (10-4-2022)
		initialise();
	}

	/**
	 *
	 */
	RobotWorldCanvas::RobotWorldCanvas( Window* anOwner,
										Model::ModelObjectPtr aModelObject) :

									ScrolledCanvas( anOwner, DEFAULT_ID),
									ViewObject(aModelObject),
									popupPoint( DefaultPosition),
									startActionPoint( DefaultPosition),
									endActionPoint( DefaultPosition),
									actionOffset( DefaultPosition),
									startActionSize( DefaultSize),
									startActionShape( nullptr),
									endActionShape( nullptr),
									selectedShape( nullptr),
									activationEnabled( false),
									selectionEnabled( false),
									menuItemEnabled( false),
									dandEnabled( true),
									notificationHandler( nullptr)
	{
		// CppCheck gives a "virtualCallInConstructor" on initialise(). I don't know why.
		// It cannot be suppressed by a "cppcheck-suppress virtualCallInConstructor" (10-4-2022)
		initialise();
	}
	/**
	 *
	 */
	RobotWorldCanvas::~RobotWorldCanvas()
	{
		shapes.clear();

		PopEventHandler();

		if (notificationHandler)
		{
			delete notificationHandler;
		}
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	Point RobotWorldCanvas::devicePointFor( const Point& aScreenPoint) const
	{
		Point devicePoint;
		/*owner->getCanvas()->*/CalcUnscrolledPosition( aScreenPoint.x, aScreenPoint.y, &devicePoint.x, &devicePoint.y);
		return devicePoint;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	Point RobotWorldCanvas::screenPointFor( const Point& aDevicePoint) const
	{
		Point screenPoint;
		CalcScrolledPosition( aDevicePoint.x, aDevicePoint.y, &screenPoint.x, &screenPoint.y);
		return screenPoint;
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableHandlePaint( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_PAINT, &RobotWorldCanvas::OnPaint, this);
		} else
		{
			Unbind( wxEVT_PAINT, &RobotWorldCanvas::OnPaint, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableHandleSize( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_SIZE, &RobotWorldCanvas::OnSize, this);
		} else
		{
			Unbind( wxEVT_SIZE, &RobotWorldCanvas::OnSize, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableLeftDownHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_LEFT_DOWN, &RobotWorldCanvas::OnLeftDown, this);
		} else
		{
			Unbind( wxEVT_LEFT_DOWN,  &RobotWorldCanvas::OnLeftDown, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableLeftUpHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_LEFT_UP, &RobotWorldCanvas::OnLeftUp, this);
		} else
		{
			Unbind( wxEVT_LEFT_UP, &RobotWorldCanvas::OnLeftUp, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableLeftDClickHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_LEFT_DCLICK, &RobotWorldCanvas::OnLeftDClick, this);
		} else
		{
			Unbind( wxEVT_LEFT_DCLICK, &RobotWorldCanvas::OnLeftDClick, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableMiddleDownHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_MIDDLE_DOWN, &RobotWorldCanvas::OnMiddleDown, this);
		} else
		{
			Unbind( wxEVT_MIDDLE_DOWN, &RobotWorldCanvas::OnMiddleDown, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableMiddleUpHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_MIDDLE_UP, &RobotWorldCanvas::OnMiddleUp, this);
		} else
		{
			Unbind( wxEVT_MIDDLE_UP, &RobotWorldCanvas::OnMiddleUp, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableMiddleDClickHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_MIDDLE_DCLICK, &RobotWorldCanvas::OnMiddleDClick, this);
		} else
		{
			Unbind( wxEVT_MIDDLE_DCLICK, &RobotWorldCanvas::OnMiddleDClick, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableRightDownHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_RIGHT_DOWN, &RobotWorldCanvas::OnRightDown, this);
		} else
		{
			Unbind( wxEVT_RIGHT_DOWN, &RobotWorldCanvas::OnRightDown, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableRightUpHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_RIGHT_UP, &RobotWorldCanvas::OnRightUp, this);
		} else
		{
			Unbind( wxEVT_RIGHT_UP, &RobotWorldCanvas::OnRightUp, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableRightDClickHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_RIGHT_DCLICK, &RobotWorldCanvas::OnRightDClick, this);
		} else
		{
			Unbind( wxEVT_RIGHT_DCLICK, &RobotWorldCanvas::OnRightDClick, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableMouseMotionHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			Bind( wxEVT_MOTION, &RobotWorldCanvas::OnMouseMotion, this);
		} else
		{
			Unbind( wxEVT_MOTION, &RobotWorldCanvas::OnMouseMotion, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableKeyHandling( bool enable /* = true */)
	{
		if (enable == true)
		{
			//Connect( wxEVT_KEY_DOWN, KeyEventHandler(RobotWorldCanvas::OnKeyDown));
			Bind( wxEVT_KEY_DOWN, &RobotWorldCanvas::OnKeyDown, this);
			Bind( wxEVT_CHAR, &RobotWorldCanvas::OnCharDown, this);
		} else
		{
			//Disconnect( wxEVT_KEY_DOWN, KeyEventHandler(RobotWorldCanvas::OnKeyDown));
			Unbind( wxEVT_KEY_DOWN, &RobotWorldCanvas::OnKeyDown, this);
			Unbind( wxEVT_CHAR, &RobotWorldCanvas::OnCharDown, this);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableActivationHandling( bool enable /* = true */)
	{
		activationEnabled = enable;
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableSelectionHandling( bool enable /* = true */)
	{
		selectionEnabled = enable;
	}
	/**
	 *
	 */
	void RobotWorldCanvas::enableItemMenuHandling( bool enable /* = true */)
	{
		menuItemEnabled = enable;
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotWorldCanvas::enableDragAndDropHandling( bool enable /*= true*/)
	{
		dandEnabled = enable;
	}
	/**
	 *
	 */
	bool RobotWorldCanvas::isShapeSelected() const
	{
		return selectedShape != nullptr;
	}
	/**
	 *
	 */
	ShapePtr RobotWorldCanvas::getSelectedShape() const
	{
		return selectedShape;
	}
	/**
	 *
	 */
	void RobotWorldCanvas::setSelectedShape( ShapePtr aSelectedShape)
	{
		ShapePtr previousSelectedShape = selectedShape;
		selectedShape = nullptr;

		// No de-selection if there is no previous or new shape
		// and no re-selection of the current shape
		if (previousSelectedShape && aSelectedShape && previousSelectedShape->getObjectId() != aSelectedShape->getObjectId())
		{
			previousSelectedShape->setSelected( false);
			if (selectionEnabled)
			{
				previousSelectedShape->handleSelection();
			}
		}

		selectedShape = aSelectedShape;

		if (selectedShape)
		{
			selectedShape->setSelected();
			if (selectionEnabled)
			{
				selectedShape->handleSelection();
			}
		}
	}
	/**
	 *
	 */
	bool RobotWorldCanvas::isShapeAt( const Point& aPoint) const
	{
		return  std::any_of(shapes.begin(), 
							shapes.end(), 
							[&aPoint](ShapePtr aShape)
							{ 
								return aShape->occupies( aPoint);
							});		
	}
	/**
	 *
	 */
	ShapePtr RobotWorldCanvas::getShapeAt( const Point& aPoint) const
	{
		if(	auto i = std::find_if(shapes.begin(),shapes.end(),[&aPoint](ShapePtr shape){return shape->occupies( aPoint);});
			i != shapes.end())
		{
			return *i;
		}
		return nullptr;
	}
	/**
	 *
	 */
	bool RobotWorldCanvas::selectShapeAt( const Point& aPoint)
	{
		if(	auto i = std::find_if(shapes.begin(),shapes.end(),[&aPoint](ShapePtr shape){return shape->occupies( aPoint);});
			i != shapes.end())
		{
			setSelectedShape( *i);
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	Model::RobotWorldPtr RobotWorldCanvas::getRobotWorld() const
	{
		return std::dynamic_pointer_cast<Model::RobotWorld>(getModelObject());
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotWorldCanvas::setRobotWorld( Model::RobotWorldPtr aRobotWorld)
	{
		setModelObject(std::dynamic_pointer_cast<Model::ModelObject>(aRobotWorld));
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleNotification()
	{
		handleBackGroundNotification();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleBackGroundNotification()
	{
		// Posting the message will put the notification event in the applications
		// message loop, hence making sure it is handled in the main thread.
		NotifyEvent event( Base::EVT_NOTIFICATIONEVENT, 1000);
		wxPostEvent( notificationHandler, event);
		//notificationHandler->ProcessEvent(event);
	}

	/**
	 *
	 */
	void RobotWorldCanvas::populate( int aNumberOfWalls)
	{
		Model::RobotWorld::getRobotWorld().populate(aNumberOfWalls);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::unpopulate()
	{
		shapes.clear();
		Model::RobotWorld::getRobotWorld().unpopulate();
	}

	/**
	 *
	 */
	void RobotWorldCanvas::initialise()
	{
		SetMinSize( Size( 500, 500)); // @suppress("Avoid magic numbers")

		notificationHandler = new Base::NotificationHandler< std::function< void( NotifyEvent&) > >( [this](NotifyEvent& anEvent){this->OnNotificationEvent(anEvent);});
		PushEventHandler( notificationHandler);

		Bind( wxEVT_PAINT, &RobotWorldCanvas::OnPaint, this);
		Bind( wxEVT_SIZE, &RobotWorldCanvas::OnSize, this);

		SetBackgroundColour( WXSTRING( "WHITE"));
		SetFocus();

		enableHandlePaint();
		enableHandleSize();

		enableLeftDownHandling();
		enableLeftUpHandling();
		enableLeftDClickHandling();

		enableMiddleDownHandling();
		enableMiddleUpHandling();
		enableMiddleDClickHandling();

		enableRightDownHandling();
		enableRightUpHandling();
		enableRightDClickHandling();

		enableMouseMotionHandling();

		enableKeyHandling();

		enableActivationHandling();
		enableSelectionHandling();

		enableItemMenuHandling();

		handleNotificationsFor( Model::RobotWorld::getRobotWorld());
	}
	/**
	 *
	 */
	void RobotWorldCanvas::render( wxDC& dc)
	{
		for (ShapePtr shape : shapes)
		{
			//		Logger::log("Drawing shape: " + shape->asString());
			shape->draw( dc);
			//		Logger::log("Done drawing shape: " + shape->asString());
		}
		if (startActionShape != nullptr && actionStatus == DRAWING)
		{
			dc.SetPen( wxPen( wxColor( 0, 0, 0), 1)); // black line, 1 pixels thick
			dc.DrawLine( startActionShape->getCentre().x, startActionShape->getCentre().y, endActionPoint.x, endActionPoint.y);
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handlePaint( PaintEvent& UNUSEDPARAM(event))
	{
		wxPaintDC dc( this);
		render( dc);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleSize( SizeEvent& event)
	{
		//	wxClientDC dc(const_cast<RobotWorldCanvas*>(this));
		//	timeTextSize = dc.GetTextExtent( _T("00:00"));
		//	Refresh();
		event.Skip();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleLeftDown( MouseEvent& event)
	{
		Point screenPoint = event.GetPosition();

		startActionPoint = screenPoint;
		endActionPoint = startActionPoint;
		actionStatus = IDLE;

		if (selectShapeAt( screenPoint))
		{
			ShapePtr shape = getSelectedShape();

			WallShapePtr wall = std::dynamic_pointer_cast<WallShape>( shape);
			if (wall)
			{
				RectangleShapePtr wallEndPoint = wall->hasEndPointAt( screenPoint);
				if (wallEndPoint)
				{
					setSelectedShape( wallEndPoint);
					shape = getSelectedShape();
				}
			}else
			{
			}

			if (shape)
			{
				startActionShape = shape;
				endActionShape = nullptr;
				actionOffset = shape->getCentre() - startActionPoint;

				if (event.ControlDown())
				{
					actionStatus = DRAWING;
				} else
				{
					actionStatus = DRAGGING;
				}
			}
		}else
		{
			Application::Logger::log( "Nothing selected...");
		}

		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleLeftUp( MouseEvent& event)
	{
		RectangleShapePtr startRectangleShape = std::dynamic_pointer_cast<RectangleShape>( startActionShape);
		RectangleShapePtr endRectangeShape = std::dynamic_pointer_cast<RectangleShape>( getShapeAt( event.GetPosition()));

		switch (actionStatus)
		{
			case IDLE:
			{
				break;
			}
			case DRAWING:
			{
				if (startRectangleShape && endRectangeShape)
				{
					ShapePtr lineShape( new LineShape( startRectangleShape, endRectangeShape));
					shapes.push_back( lineShape);
				}
				break;
			}
			case RESIZING:
			{
				break;
			}
			case STARTDRAGGING:
			{
				break;
			}
			case DRAGGING:
			{
				if (dandEnabled && startActionPoint != endActionPoint)
				{
					handleEndDrag( selectedShape);
				}
				break;
			}
			default:
			{
				break;
			}
		}
		startActionPoint = DefaultPosition;
		endActionPoint = DefaultPosition;

		startActionShape = nullptr;
		endActionShape = nullptr;
		actionStatus = IDLE;

		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleLeftDClick( MouseEvent& event)
	{
		Point screenPoint = event.GetPosition();
		if (isShapeAt( screenPoint))
		{
			ShapePtr shape = getShapeAt( screenPoint);
			if (shape && activationEnabled)
			{
				shape->handleActivated();
			}
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleMiddleDown( MouseEvent& UNUSEDPARAM(event))
	{
		// We must set the focus or any keyboard events will get lost
		//SetFocus();

		//Point devicePoint = devicePointFor(event.GetPosition());
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleMiddleUp( MouseEvent& UNUSEDPARAM(event))
	{
		Application::Logger::log( __PRETTY_FUNCTION__);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleMiddleDClick( MouseEvent& UNUSEDPARAM(event))
	{
		Application::Logger::log( __PRETTY_FUNCTION__);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleRightDown( MouseEvent& event)
	{
		// We must set the focus or any keyboard events will get lost
		SetFocus();
		Point screenPoint = event.GetPosition();
		actionStatus = IDLE;

		if (selectShapeAt( screenPoint))
		{
		}
		Refresh();

	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleRightUp( MouseEvent& event)
	{
		actionStatus = IDLE;

		if (menuItemEnabled)
		{
			popupPoint = event.GetPosition();
			if (isShapeAt( popupPoint))
			{
				handleItemMenu( getShapeAt( popupPoint), popupPoint);
				return;
			}
			if (!isShapeAt( popupPoint))
			{
				handleMenu( popupPoint);
				return;
			}
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleRightDClick( MouseEvent& UNUSEDPARAM(event))
	{
		Application::Logger::log( __PRETTY_FUNCTION__);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleMouseMotion( MouseEvent& event)
	{
		if (event.Moving() == true && event.Dragging() == false)
		{
			return;
		}
		if (event.Moving() == false && event.Dragging() == true && startActionShape != nullptr)
		{
			int tolerance = 2;
			int dx = std::abs( event.GetPosition().x - startActionPoint.x);
			int dy = std::abs( event.GetPosition().y - startActionPoint.y);
			if (dx <= tolerance && dy <= tolerance)
			{
				return;
			}

			switch (actionStatus)
			{
				case RESIZING:
				{
//					Size size = startActionSize;
//
//					Point mousePosition = ScreenToClient(wxGetMousePosition());
//					int dx = mousePosition.x - startActionPoint.x;
//					int dy = mousePosition.y - startActionPoint.y;
//					if( mousePosition.x < startActionShape->getCentre().x)
//					{
//						dx = -dx;
//					}
//					if( mousePosition.y < startActionShape->getCentre().y)
//					{
//						dy = -dy;
//					}
//
//					size.x += 2*dx;
//					size.y += 2*dy;
//
//					startActionShape->setSize(size);
//					Refresh();
					break;
				}
				case DRAGGING:
				{
					startActionShape->setCentre( event.GetPosition() + actionOffset);
					endActionPoint = event.GetPosition();
					Refresh();
					break;
				}
				case DRAWING:
				{
					endActionPoint = event.GetPosition();
					Refresh();
					break;
				}
				default:
				{
					Application::Logger::log( "Just moving, nothing to see...");
				}
			}
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleKey( KeyEvent& event)
	{
		Application::Logger::log( __PRETTY_FUNCTION__);

		switch (event.GetKeyCode())
		{
			case WXK_DELETE:
			case WXK_NUMPAD_DELETE:
			{
				break;
			}
			case WXK_ESCAPE:
			{
				switch (actionStatus)
				{
					case DRAWING:
					{
						actionStatus = CANCELDRAWING;
						break;
					}
					case RESIZING:
					{
						actionStatus = CANCELRESIZING;
						break;
					}
					case DRAGGING:
					{
						actionStatus = CANCELDRAGGING;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleBeginLeftDrag( ShapePtr aShape)
	{
		aShape->handleBeginLeftDrag();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleEndDrag( ShapePtr aShape)
	{
		RobotShapePtr robotShape = std::dynamic_pointer_cast<RobotShape>(aShape);
		if (robotShape)
		{
			robotShape->getRobot()->setPosition( robotShape->getCentre(), false);
			return;
		}
		// Handles both WayPoint and Goal
		WayPointShapePtr wayPointShape = std::dynamic_pointer_cast<WayPointShape>(aShape);
		if (wayPointShape)
		{
			wayPointShape->getWayPoint()->setPosition( wayPointShape->getCentre(), false);
			return;
		}
		// Handle the RectangleShapes that are part of a wall
		RectangleShapePtr rectangleShape = std::dynamic_pointer_cast<RectangleShape>(aShape);
		if (rectangleShape)
		{
			for (ShapePtr shape : shapes)
			{
				WallShapePtr wall = std::dynamic_pointer_cast<WallShape>( shape);
				if (wall)
				{
					if (wall->hasEndPoint( rectangleShape))
					{
						wall->updateEndPoint( rectangleShape);
						return;
					}
				}
			}
		}
		/*aShape->handleEndDrag();*/
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleAddRobot( CommandEvent& UNUSEDPARAM(event))
	{
		RobotShapePtr robot( new RobotShape( Model::RobotWorld::getRobotWorld().newRobot( "Robot", popupPoint)));
		addShape(robot);
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleEditRobot( CommandEvent& UNUSEDPARAM(event))
	{
		RobotShapePtr shape = std::dynamic_pointer_cast<RobotShape>( getSelectedShape());
		if (shape)
		{
			std::string name = GetTextFromUser( "The new Robot name:", "Enter a new name");

			if (name != "" && name != shape->getRobot()->getName())
			{
				shape->setTitle( name);
				shape->getRobot()->setName( name);
			}
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleDeleteRobot( CommandEvent& UNUSEDPARAM(event))
	{
		RobotShapePtr shape = std::dynamic_pointer_cast<RobotShape>( getSelectedShape());
		if (shape)
		{
			removeShape( shape);
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleAddWayPoint( CommandEvent& UNUSEDPARAM(event))
	{
		WayPointShapePtr wayPoint( new WayPointShape( Model::RobotWorld::getRobotWorld().newWayPoint( "Joost", popupPoint)));
		addShape(wayPoint);
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleEditWayPoint( CommandEvent& UNUSEDPARAM(event))
	{
		WayPointShapePtr shape = std::dynamic_pointer_cast<WayPointShape>( getSelectedShape());
		if (shape)
		{
			std::string name = GetTextFromUser( "The new WayPoint name:", "Enter a new name");

			if (name != "" && name != shape->getWayPoint()->getName())
			{
				shape->setTitle( name);
				shape->getWayPoint()->setName( name);
			}
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleDeleteWayPoint( CommandEvent& UNUSEDPARAM(event))
	{
		WayPointShapePtr shape = std::dynamic_pointer_cast<WayPointShape>( getSelectedShape());
		if (shape)
		{
			removeShape( shape);
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleAddGoal( CommandEvent& UNUSEDPARAM(event))
	{
		GoalShapePtr goal( new GoalShape( Model::RobotWorld::getRobotWorld().newGoal( "Goal", popupPoint)));
		addShape(goal);
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleEditGoal( CommandEvent& UNUSEDPARAM(event))
	{
		GoalShapePtr shape = std::dynamic_pointer_cast<GoalShape>( getSelectedShape());
		if (shape)
		{
			std::string name = GetTextFromUser( "The new Goal name:", "Enter a new name");

			if (name != "" && name != shape->getGoal()->getName())
			{
				shape->setTitle( name);
				shape->getGoal()->setName( name);
			}
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleDeleteGoal( CommandEvent& UNUSEDPARAM(event))
	{
		GoalShapePtr shape = std::dynamic_pointer_cast<GoalShape>( getSelectedShape());
		if (shape)
		{
			removeShape( shape);
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleAddWall( CommandEvent& UNUSEDPARAM(event))
	{
		RectangleShapePtr start( new RectangleShape( popupPoint));
		RectangleShapePtr end( new RectangleShape( popupPoint + Point( 50, 50)));

		ShapePtr wall( new WallShape( Model::RobotWorld::getRobotWorld().newWall( start->getCentre(), end->getCentre(),false),
									  start,
									  end));

		shapes.push_back( wall);
		shapes.push_back( start);
		shapes.push_back( end);

		Refresh();
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotWorldCanvas::handleEditWall( CommandEvent& UNUSEDPARAM(event))
	{
		Application::Logger::log( __PRETTY_FUNCTION__);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleDeleteWall( CommandEvent& UNUSEDPARAM(event))
	{
		WallShapePtr shape = std::dynamic_pointer_cast<WallShape>( getSelectedShape());
		if (shape)
		{
			removeShape( shape);
		}
		Refresh();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleShapeInfo( CommandEvent& UNUSEDPARAM(event))
	{
		if (isShapeSelected())
		{
			Application::Logger::log( getSelectedShape()->asDebugString());
		}
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleNotification( NotifyEvent& UNUSEDPARAM(aNotifyEvent))
	{
		remove<Model::Robot,View::RobotShape>( Model::RobotWorld::getRobotWorld().getRobots());
		add<Model::Robot,View::RobotShape>( Model::RobotWorld::getRobotWorld().getRobots());

		remove<Model::WayPoint,View::WayPointShape>( Model::RobotWorld::getRobotWorld().getWayPoints());
		add<Model::WayPoint,View::WayPointShape>( Model::RobotWorld::getRobotWorld().getWayPoints());

		remove<Model::Goal,View::GoalShape>( Model::RobotWorld::getRobotWorld().getGoals());
		add<Model::Goal,View::GoalShape>( Model::RobotWorld::getRobotWorld().getGoals());

		remove<Model::Wall,View::WallShape>( Model::RobotWorld::getRobotWorld().getWalls());
		add<Model::Wall,View::WallShape>( Model::RobotWorld::getRobotWorld().getWalls());

		Refresh();
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotWorldCanvas::handleActivation( ShapePtr aShape)
	{
		aShape->handleActivated();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleSelection( ShapePtr aShape)
	{
		aShape->handleSelection();
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleMenu( const Point& UNUSEDPARAM(aScreenPoint))
	{
		Menu popupMenu;

		popupMenu.Append( ID_ADD_ROBOT, _T( "Add robot"), _T( "ID_ADD_ROBOT2"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnAddRobot(anEvent);},
			 ID_ADD_ROBOT);

		popupMenu.Append( ID_ADD_WAYPOINT, _T( "Add waypoint"), _T( "ID_ADD_WAYPOINT2"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnAddWayPoint(anEvent);},
			 ID_ADD_WAYPOINT);

		popupMenu.Append( ID_ADD_GOAL, _T( "Add goal"), _T( "ID_ADD_GOAL2"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnAddGoal(anEvent);},
			 ID_ADD_GOAL);

		popupMenu.Append( ID_ADD_WALL, _T( "Add wall"), _T( "ID_ADD_WALL2"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnAddWall(anEvent);},
			 ID_ADD_WALL);

		popupMenu.Append( ID_WORLD_INFO, _T( "World info"), _T( "ID_WORLD_INFO2"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnWorldInfo(anEvent);},
			 ID_WORLD_INFO);

		popupMenu.Append( wxID_ABOUT, _T( "About"), _T( "Show about dialog"));
		PopupMenu( &popupMenu);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::handleItemMenu( 	ShapePtr aSelectedShape,
											const Point& UNUSEDPARAM(aPoint))
	{
		Menu popupMenu;

		if(std::dynamic_pointer_cast<RobotShape>(aSelectedShape))
		{
			popupMenu.Append( ID_EDIT_ROBOT, _T( "Edit robot"), _T( "ID_EDIT_ROBOT"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnEditRobot(anEvent);},
				 ID_EDIT_ROBOT);
			popupMenu.Append( ID_DELETE_ROBOT, _T( "Delete robot"), _T( "ID_DELETE_ROBOT"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnDeleteRobot(anEvent);},
				 ID_DELETE_ROBOT);
		} else if(std::dynamic_pointer_cast<GoalShape>(aSelectedShape))
		{
			popupMenu.Append( ID_EDIT_GOAL, _T( "Edit goal"), _T( "ID_EDIT_GOAL"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnEditGoal(anEvent);},
				 ID_EDIT_GOAL);
			popupMenu.Append( ID_DELETE_GOAL, _T( "Delete goal"), _T( "ID_DELETE_GOAL"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnDeleteGoal(anEvent);},
				 ID_DELETE_GOAL);
		} else if(std::dynamic_pointer_cast<WayPointShape>(aSelectedShape))
		{
			popupMenu.Append( ID_EDIT_WAYPOINT, _T( "Edit waypoint"), _T( "ID_EDIT_WAYPOINT"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnEditWayPoint(anEvent);},
				 ID_EDIT_WAYPOINT);
			popupMenu.Append( ID_DELETE_WAYPOINT, _T( "Delete waypoint"), _T( "ID_DELETE_WAYPOINT"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnDeleteWayPoint(anEvent);},
				 ID_DELETE_WAYPOINT);
		} else if(std::dynamic_pointer_cast<WallShape>(aSelectedShape))
		{
			popupMenu.Append( ID_DELETE_WALL, _T( "Delete wall"), _T( "ID_DELETE_WALL"));
			Bind(wxEVT_COMMAND_MENU_SELECTED,
				 [this](CommandEvent& anEvent){ this->OnDeleteWall(anEvent);},
				 ID_DELETE_WALL);
		} else
		{
			Application::Logger::log( "Huh?: " + aSelectedShape->asDebugString());
		}

		popupMenu.Append( ID_SHAPE_INFO, _T( "Show info"), _T( "ID_SHAPE_INFO"));
		Bind(wxEVT_COMMAND_MENU_SELECTED,
			 [this](CommandEvent& anEvent){ this->OnShapeInfo(anEvent);},
			 ID_SHAPE_INFO);

		popupMenu.Append( wxID_ABOUT, _T( "About"), _T( "Show about dialog"));
		PopupMenu( &popupMenu);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::addShape( RobotShapePtr aRobotShape)
	{
		aRobotShape->setRobotWorldCanvas(this);
		aRobotShape->handleNotificationsFor(*aRobotShape->getRobot());
		shapes.push_back( std::dynamic_pointer_cast< Shape >( aRobotShape));
	}
	/**
	 *
	 */
	void RobotWorldCanvas::addShape( GoalShapePtr aGoalShape)
	{
		aGoalShape->handleNotificationsFor(*aGoalShape->getGoal());
		shapes.push_back( std::dynamic_pointer_cast< Shape >( aGoalShape));
	}
	/**
	 *
	 */
	void RobotWorldCanvas::addShape( WayPointShapePtr aWayPointShape)
	{
		aWayPointShape->handleNotificationsFor(*aWayPointShape->getWayPoint());
		shapes.push_back( std::dynamic_pointer_cast< Shape >( aWayPointShape));
	}
	/**
	 *
	 */
	void RobotWorldCanvas::addShape( WallShapePtr aWallShape)
	{
		aWallShape->handleNotificationsFor(*aWallShape->getWall());

		RectangleShapePtr start( new RectangleShape( aWallShape->getWall()->getPoint1()));
		RectangleShapePtr end( new RectangleShape( aWallShape->getWall()->getPoint2()));

		aWallShape->setNode1(start);
		aWallShape->setNode2(end);

		shapes.push_back( start);
		shapes.push_back( end);
		shapes.push_back( aWallShape);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::removeShape( RobotShapePtr aRobotShape)
	{
		aRobotShape->stopHandlingNotificationsFor( *aRobotShape->getRobot());
		Model::RobotWorld::getRobotWorld().deleteRobot( aRobotShape->getRobot(), false);
		removeGenericShape( aRobotShape);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::removeShape( GoalShapePtr aGoalShape)
	{
		aGoalShape->stopHandlingNotificationsFor( *aGoalShape->getGoal());
		Model::RobotWorld::getRobotWorld().deleteGoal( aGoalShape->getGoal(), false);
		removeGenericShape( aGoalShape);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::removeShape( WayPointShapePtr aWayPointShape)
	{
		aWayPointShape->stopHandlingNotificationsFor( *aWayPointShape->getWayPoint());
		Model::RobotWorld::getRobotWorld().deleteWayPoint( aWayPointShape->getWayPoint(), false);
		removeGenericShape( aWayPointShape);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::removeShape( WallShapePtr aWallShape)
	{
		RectangleShapePtr start = aWallShape->hasEndPointAt( aWallShape->getBegin());
		RectangleShapePtr end = aWallShape->hasEndPointAt( aWallShape->getEnd());
		Model::RobotWorld::getRobotWorld().deleteWall( aWallShape->getWall(), false);
		removeGenericShape( aWallShape);
		removeGenericShape( start);
		removeGenericShape( end);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::removeGenericShape( ShapePtr aShape)
	{
		if (isShapeSelected() && aShape->getObjectId() == getSelectedShape()->getObjectId())
		{
			setSelectedShape( nullptr);
		}
		shapes.erase( std::find_if( shapes.begin(),
									shapes.end(),
									[aShape](ShapePtr s)
									{
										return aShape->getObjectId() == s->getObjectId();
									}));
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnPaint( PaintEvent& event)
	{
		handlePaint( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnSize( SizeEvent& event)
	{
		handleSize( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnLeftDown( MouseEvent& event)
	{
		handleLeftDown( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnLeftUp( MouseEvent& event)
	{
		handleLeftUp( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnLeftDClick( MouseEvent& event)
	{
		handleLeftDClick( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnMiddleDown( MouseEvent& event)
	{
		handleMiddleDown( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnMiddleUp( MouseEvent& event)
	{
		handleMiddleUp( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnMiddleDClick( MouseEvent& event)
	{
		handleMiddleDClick( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnRightDown( MouseEvent& event)
	{
		handleRightDown( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnRightUp( MouseEvent& event)
	{
		handleRightUp( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnRightDClick( MouseEvent& event)
	{
		handleRightDClick( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnMouseMotion( MouseEvent& event)
	{
		handleMouseMotion( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnKeyDown( KeyEvent& event)
	{
		handleKey( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnCharDown( KeyEvent& event)
	{
		handleKey( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnAddRobot( CommandEvent& event)
	{
		handleAddRobot( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnEditRobot( CommandEvent& event)
	{
		handleEditRobot( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnDeleteRobot( CommandEvent& event)
	{
		handleDeleteRobot( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnAddWayPoint( CommandEvent& event)
	{
		handleAddWayPoint( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnEditWayPoint( CommandEvent& event)
	{
		handleEditWayPoint( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnDeleteWayPoint( CommandEvent& event)
	{
		handleDeleteWayPoint( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnAddGoal( CommandEvent& event)
	{
		handleAddGoal( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnEditGoal( CommandEvent& event)
	{
		handleEditGoal( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnDeleteGoal( CommandEvent& event)
	{
		handleDeleteGoal( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnAddWall( CommandEvent& event)
	{
		handleAddWall( event);
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void RobotWorldCanvas::OnEditWall( CommandEvent& UNUSEDPARAM(event))
	{

	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnDeleteWall( CommandEvent& event)
	{
		handleDeleteWall( event);
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnShapeInfo( CommandEvent& event)
	{
		handleShapeInfo( event);
	}
	/**
	 *
	 *
	 */
	void RobotWorldCanvas::OnWorldInfo( CommandEvent& UNUSEDPARAM(event))
	{
		Application::Logger::log( Model::RobotWorld::getRobotWorld().asDebugString());
	}
	/**
	 *
	 */
	void RobotWorldCanvas::OnNotificationEvent( NotifyEvent& aNotifyEvent)
	{
		handleNotification(aNotifyEvent);
	}
} // namespace View
