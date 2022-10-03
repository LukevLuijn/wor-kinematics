#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include "Config.hpp"

#include "ObjectId.hpp"
#include "Point.hpp"
#include "ViewObject.hpp"
#include "Widgets.hpp"

namespace View
{
	class Shape;
	typedef std::shared_ptr<Shape> ShapePtr;

	/**
	 * This struct is a generic placeholder for any data a Shape may be displaying,
	 */
	struct ShapeData
	{
			/**
			 *
			 */
			virtual ~ShapeData()
			{
			}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const
			{
				return "ShapeData";

			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const
			{
				return asString();
			}
			//@}
	};
	//	struct ShapeData

	/**
	 *
	 */
	class Shape : public ViewObject
	{
		public:
			/**
			 *
			 */
			Shape() :
				ViewObject(),
				data( nullptr),
				selected( false)
		{
		}
			/**
			 *
			 */
			explicit Shape(Model::ModelObjectPtr aModelObject) :
				ViewObject(aModelObject),
				data( nullptr),
				selected( false)
			{
				handleNotificationsFor( *aModelObject);
			}
			/**
			 *
			 */
			virtual ~Shape()
			{
			}
			/**
			 * @name Pure virtual abstract Shape functions
			 */
			//@{
			/**
			 *
			 */
			virtual void draw( wxDC& dc) = 0;
			/**
			 *
			 * @param aPoint
			 * @return True if the point is in the shape
			 */
			virtual bool occupies( const Point& aPoint) const = 0;
			/**
			 *
			 */
			virtual Point getCentre() const = 0;
			/**
			 *
			 */
			virtual void setCentre( const Point& aPoint) = 0;
			//@}
			/**
			 * @name Accessors and mutators
			 */
			//@{
			/**
			 *
			 */
			ShapeData* getData() const
			{
				return data;
			}
			/**
			 *
			 */
			void setData( ShapeData* aData)
			{
				if (data)
				{
					delete data;
				}
				data = aData;
			}
			/**
			 *
			 */
			bool isSelected() const
			{
				return selected;
			}
			/**
			 *
			 */
			void setSelected( bool aSelected = true)
			{
				selected = aSelected;
			}
			/**
			 * By default this does the same as Shape::setSelected because
			 * by default a Shape has no volume or surface and the selection point can be
			 * assumed to be the centre.
			 */
			virtual void setSelectedAt( const Point& aPoint = DefaultPosition,
										bool aSelected = true)
			{
				UNUSEDCAST( aPoint);
				setSelected( aSelected);
			}
			/**
			 *
			 */
			virtual Point getSelectedPoint() const
			{
				return getCentre();
			}
			//@}
			/**
			 * This function is called by the RobotWorldCanvas if enableActivation is set for the RobotWorldCanvas.
			 * If this is not the desired behaviour override RobotWorldCanvas::handleItemActivated
			 * in a derived class.
			 */
			virtual void handleActivated()
			{
			}
			/**
			 * This function is called by the RobotWorldCanvas if enableSelection is set for the RobotWorldCanvas.
			 * If this is not the desired behaviour override RobotWorldCanvas::handleSelectionChanged
			 * in a derived class.
			 */
			virtual void handleSelection()
			{
			}
			/**
			 * This function is called by the RobotWorldCanvas when the user starts dragging this object.
			 */
			virtual void handleBeginLeftDrag()
			{
			}
			/**
			 * This function is called by the RobotWorldCanvas when the user drops this object after dragging.
			 */
			virtual void handleEndDrag()
			{
			}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const
			{
				return "Shape " + getObjectId().asString() + (selected ? " selected = true" : " selected = false");
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const
			{
				return Shape::asString() + (data ? "\n " + data->asDebugString() : "");
			}
			//@}

		private:
			ShapeData* data;
			bool selected;
	};
	//	class Shape
} // namespace View
#endif // SHAPE_HPP_
