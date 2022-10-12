#ifndef ROBOTWORLDCANVAS_HPP_
#define ROBOTWORLDCANVAS_HPP_

#include "Config.hpp"

#include "NotificationHandler.hpp"
#include "RobotWorld.hpp"
#include "Shape.hpp"
#include "Size.hpp"
#include "ViewObject.hpp"
#include "Widgets.hpp"

#include <vector>

namespace Model
{
    enum class Filters_e : uint8_t;
}

namespace View
{
	class Shape;
	typedef std::shared_ptr<Shape> ShapePtr;
	class RobotShape;
	typedef std::shared_ptr<RobotShape> RobotShapePtr;
	class GoalShape;
	typedef std::shared_ptr<GoalShape> GoalShapePtr;
	class WayPointShape;
	typedef std::shared_ptr<WayPointShape> WayPointShapePtr;
	class WallShape;
	typedef std::shared_ptr<WallShape> WallShapePtr;
    class PathShape;
    typedef std::shared_ptr<PathShape> PathShapePtr;

	/**
	 *
	 */
	class RobotWorldCanvas :	public ScrolledCanvas,
								public ViewObject
	{
		public:
			/**
			 *
			 */
			explicit RobotWorldCanvas( Window* anOwner);
			/**
			 *
			 */
			RobotWorldCanvas( Window* anOwner,
							  Model::ModelObjectPtr aModelObject);
			/**
			 *
			 */
			virtual ~RobotWorldCanvas();
			/**
			 * The canvas is partially displayed in a scrolling window. Hence, the user viewable part of this window
			 * is probably just a part of this window. This function takes a point in the (visible) scrolling window coordinates
			 * and translates it to this canvas windows coordinates. It takes into account the number of lines and number of
			 * pixels between lines.
			 */
			Point devicePointFor( const Point& aScreenPoint) const;
			/**
			 * This function is the opposite of devicePointFor(): it translates canvas coordinates to scrolling window coordinates
			 *
			 * @see devicePointFor( const Point&)
			 */
			Point screenPointFor( const Point& aDevicePoint) const;
			/**
			 * @name Event handling enabling functions
			 *
			 */
			//@{
			void enableHandlePaint( bool enable = true);
			void enableHandleSize( bool enable = true);

			void enableLeftDownHandling( bool enable = true);
			void enableLeftUpHandling( bool enable = true);
			void enableLeftDClickHandling( bool enable = true);

			void enableMiddleDownHandling( bool enable = true);
			void enableMiddleUpHandling( bool enable = true);
			void enableMiddleDClickHandling( bool enable = true);

			void enableRightDownHandling( bool enable = true);
			void enableRightUpHandling( bool enable = true);
			void enableRightDClickHandling( bool enable = true);

			void enableMouseMotionHandling( bool enable = true);

			void enableKeyHandling( bool enable = true);

			void enableActivationHandling( bool enable = true);
			void enableSelectionHandling( bool enable = true);

			void enableItemMenuHandling( bool enable = true);

			void enableDragAndDropHandling( bool enable = true);
			//@}
			/**
			 *
			 * @return true if any shape is selected, false otherwise
			 */
			bool isShapeSelected() const;
			/**
			 *
			 * @return A selected Shape if any, nullptr otherwise.
			 */
			virtual ShapePtr getSelectedShape() const;
			/**
			 * Sets the given Shape as the selected shape.
			 * If selectionEnabled is set to true, handleSelection() of both the old and new
			 * selected shape will be called after setting their selection as appropriate.
			 *
			 * @param 	aSelectedShape 	The new Shape that will be selected. If a nullptr is passed  it acts as a mere de-selection of the current
			 * 							selected shape
			 */
			virtual void setSelectedShape( ShapePtr aSelectedShape);

			/**
			 *
			 * @param 	aPoint
			 * @return 	True if any Shape returns true for Shape.ocuppies(aPoint), false otherwise.
			 *
			 * @see Shape::occupies(const Point&)
			 */
			virtual bool isShapeAt( const Point& aPoint) const;
			/**
			 *
			 * @param 	aPoint A screen point, i.e. a point on the screen, not on the (scrollable) canvas.
			 * @return 	The first Shape in iteration order that returns true for Shape.ocuppies(aPoint). If
			 * 			no such Shape exists nullptr will be returned.
			 */
			virtual ShapePtr getShapeAt( const Point& aPoint) const;
			/**
			 * Selects the Shape that returns true for Shape.ocuppies(aPoint).
			 * @param aPoint
			 * @return
			 */
			virtual bool selectShapeAt( const Point& aPoint);

			/**
			 * @name Type safe accessors and mutators
			 */
			//@{
			/**
			 * Type safe accessor
			 */
			Model::RobotWorldPtr getRobotWorld() const;
			/**
			 * Type safe mutator
			 */
			void setRobotWorld( Model::RobotWorldPtr aRobotWorld);
			//@}
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 * This function should only be called from the main thread because wxWidgets does not allow
			 * for painting widgets in a background thread: all painting should be done in the main thread.
			 * Therefore all notifications are routed to handleBackGroundNotification() as a convenience.
			 *
			 */
			virtual void handleNotification() override;
			//@}
			/**
			 * A Notifier that runs in a background thread should call this function instead of handleNotification().
			 * handleNotification() is routed to this function as a convenience. Bad for performance though.
			 */
			virtual void handleBackGroundNotification();
			/**
			 * Asks the world to populates itself with a robot, a goal and the given number of walls
			 */
			void populate(const std::vector<Model::Filters_e>& activeFilters, int aNumberOfWalls = 2);
			/**
			 * Removes everything from the world
			 */
			void unpopulate();
		protected:
			/**
			 * Common initialise function
			 */
			void initialise();
			/**
			 *
			 */
			void render( wxDC& dc);
			/**
			 * @name Event handling functions
			 *
			 * Override these functions if the default handling is not what you want
			 */
			//@{
			virtual void handlePaint( PaintEvent& event);
			virtual void handleSize( SizeEvent& event);

			virtual void handleLeftDown( MouseEvent& event);
			virtual void handleLeftUp( MouseEvent& event);
			virtual void handleLeftDClick( MouseEvent& event);

			virtual void handleMiddleDown( MouseEvent& event);
			virtual void handleMiddleUp( MouseEvent& event);
			virtual void handleMiddleDClick( MouseEvent& event);

			virtual void handleRightDown( MouseEvent& event);
			virtual void handleRightUp( MouseEvent& event);
			virtual void handleRightDClick( MouseEvent& event);

			virtual void handleMouseMotion( MouseEvent& event);

			virtual void handleKey( KeyEvent& event);

			virtual void handleBeginLeftDrag( ShapePtr aShape);
			virtual void handleEndDrag( ShapePtr aShape);

			virtual void handleAddRobot( CommandEvent& event);
			virtual void handleEditRobot( CommandEvent& event);
			virtual void handleDeleteRobot( CommandEvent& event);

			virtual void handleAddWayPoint( CommandEvent& event);
			virtual void handleEditWayPoint( CommandEvent& event);
			virtual void handleDeleteWayPoint( CommandEvent& event);

			virtual void handleAddGoal( CommandEvent& event);
			virtual void handleEditGoal( CommandEvent& event);
			virtual void handleDeleteGoal( CommandEvent& event);

			virtual void handleAddWall( CommandEvent& event);
			virtual void handleEditWall( CommandEvent& event);
			virtual void handleDeleteWall( CommandEvent& event);

			virtual void handleShapeInfo( CommandEvent& event);

			virtual void handleNotification( NotifyEvent& aNotifyEvent);

			//@}
			virtual void handleActivation( ShapePtr aShape);
			virtual void handleSelection( ShapePtr aShape);
			/**
			 *
			 */
			virtual void handleMenu( const Point& aScreenPoint);
			/**
			 *
			 */
			virtual void handleItemMenu( 	ShapePtr aSelectedShape,
											const Point& aPoint);
			/**
			 *
			 */
			void addShape( RobotShapePtr aRobotShape);
			/**
			 *
			 */
			void addShape( GoalShapePtr aGoalShape);
			/**
			 *
			 */
			void addShape( WayPointShapePtr aWayPointShape);
			/**
			 *
			 */
			void addShape( WallShapePtr aWallShape);
			/**
			 *
			 */
			 void addShape(PathShapePtr aPathShape);
             /**
              *
              */
             void removeShape( RobotShapePtr aRobotShape);
			/**
			 *
			 */
			void removeShape( GoalShapePtr aGoalShape);
			/**
			 *
			 */
			void removeShape( WayPointShapePtr aWayPointShape);
			/**
			 *
			 */
			void removeShape( WallShapePtr aWallShape);
			/**
			 *
			 */
            void removeShape(PathShapePtr aPathShape);
			 /**
			  *
			  */
			void removeGenericShape( ShapePtr aShape);
		private:
			/**
			 * @name Event handlers
			 *
			 */
			//@{
			void OnPaint( PaintEvent& event);
			void OnSize( SizeEvent& event);

			void OnLeftDown( MouseEvent& event);
			void OnLeftUp( MouseEvent& event);
			void OnLeftDClick( MouseEvent& event);

			void OnMiddleDown( MouseEvent& event);
			void OnMiddleUp( MouseEvent& event);
			void OnMiddleDClick( MouseEvent& event);

			void OnRightDown( MouseEvent& event);
			void OnRightUp( MouseEvent& event);
			void OnRightDClick( MouseEvent& event);

			void OnMouseMotion( MouseEvent& event);

			void OnKeyDown( KeyEvent& event);
			void OnCharDown( KeyEvent& event);

			void OnAddRobot( CommandEvent& event);
			void OnEditRobot( CommandEvent& event);
			void OnDeleteRobot( CommandEvent& event);

			void OnAddWayPoint( CommandEvent& event);
			void OnEditWayPoint( CommandEvent& event);
			void OnDeleteWayPoint( CommandEvent& event);

			void OnAddGoal( CommandEvent& event);
			void OnEditGoal( CommandEvent& event);
			void OnDeleteGoal( CommandEvent& event);

			void OnAddWall( CommandEvent& event);
			void OnEditWall( CommandEvent& event);
			void OnDeleteWall( CommandEvent& event);

			void OnShapeInfo( CommandEvent& event);

			void OnWorldInfo( CommandEvent& event);

			void OnNotificationEvent( NotifyEvent& aNotifyEvent);
			//@}

			std::vector< ShapePtr > shapes;

			enum
			{
				IDLE,
				DRAWING,
				CANCELDRAWING,
				RESIZING,
				CANCELRESIZING,
				STARTDRAGGING,
				DRAGGING,
				CANCELDRAGGING
			} actionStatus;

			Point popupPoint;
			Point startActionPoint;
			Point endActionPoint;
			Point actionOffset;
			Size startActionSize;

			ShapePtr startActionShape;
			ShapePtr endActionShape;

			ShapePtr selectedShape;

			bool activationEnabled;
			bool selectionEnabled;
			bool menuItemEnabled;
			bool dandEnabled;

			Base::NotificationHandler< std::function< void( NotifyEvent&) > > * notificationHandler;

			/**
			 * This function removes all Shapes that look at a ModelObject that is not longer in RobotWorld
			 */
			template< typename T, typename S >
			void remove( const std::vector<std::shared_ptr< T > >& aTs)
			{
				std::vector< ShapePtr> shapePtrs;
				std::copy_if(	shapes.begin(),
								shapes.end(),
								std::back_inserter(shapePtrs),
								[&aTs](ShapePtr aShape)
								{
									std::shared_ptr< S > shape = std::dynamic_pointer_cast< S >( aShape);
									if (shape)
									{
										return std::find_if(	aTs.begin(),
																aTs.end(),
																[shape](std::shared_ptr< T > aT)
																{
																	return shape->getModelObject()->getObjectId() == aT->getObjectId();
																}) == aTs.end();
									}
									return false;
								});
				for(ShapePtr shape : shapePtrs)
				{
					removeShape( std::dynamic_pointer_cast< S >( shape));
				}
			}
			/**
			 * This function add Shapes for ModelObjects that are in RobotWorld but that have no Shape yet
			 */
			template< typename T, typename S >
			void add( const std::vector<std::shared_ptr< T > >& aTs)
			{
				for(std::shared_ptr< T > t : aTs)
				{
					auto result = std::find_if(	shapes.begin(),
												shapes.end(),
												[t](ShapePtr aShape)
												{
													if(aShape->getModelObject())
													{
														return aShape->getModelObject()->getObjectId() == t->getObjectId();
													}
													return false;
												});
					if( result == shapes.end())
					{
						addShape(std::shared_ptr< S >(new S(t)));
					}
				}
			}
	};
} // namespace View
#endif /* ROBOTWORLDCANVAS_HPP_ */
