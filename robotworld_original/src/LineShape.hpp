#ifndef LINESHAPE_HPP_
#define LINESHAPE_HPP_

#include "Config.hpp"

#include "RectangleShape.hpp"
#include "Shape.hpp"

#include <memory>
#include <string>


namespace View
{
	/**
	 *
	 */
	class LineShape : public Shape
	{
		public:
			/**
			 *
			 */
			LineShape(	RectangleShapePtr aNode1,
						RectangleShapePtr aNode2,
						const std::string& aTitle = "",
						int aLineWidth = 1,
						int anArrowHeadSize = 10);
			/**
			 *
			 */
			LineShape(	Model::ModelObjectPtr aModelObject,
						RectangleShapePtr aNode1,
						RectangleShapePtr aNode2,
						const std::string& aTitle = "",
						int aLineWidth = 1,
						int anArrowHeadSize = 10);
			/**
			 *
			 */
			~LineShape();
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 */
			virtual void handleNotification() override {}
			//@}
			/**
			 * @name Pure virtual abstract Shape functions
			 */
			//@{
			/**
			 *
			 */
			virtual void draw( wxDC& dc) override;
			/**
			 *
			 * @param aPoint
			 * @return True if the point is in the shape
			 */
			virtual bool occupies( const Point& aPoint) const override;
			/**
			 *
			 */
			virtual Point getCentre() const override;
			/**
			 *
			 */
			virtual void setCentre( const Point& aPoint) override;
			//@}
			/**
			 *
			 */
			void drawHead( wxDC& dc);
			/**
			 *
			 */
			std::string getTitle() const;
			/**
			 *
			 */
			void setTitle( const std::string& aTitle);
			/**
			 *
			 */
			int getLineWidth() const;
			/**
			 *
			 */
			void setLineWidth( int aLineWidth);
			/**
			 *
			 */
			int getArrowHeadSize() const;
			/**
			 *
			 */
			void setArrowHeadSize( int anArrowHeadSize);
			/**
			 *
			 */
			Point getBegin() const;
			/**
			 *
			 */
			Point getEnd() const;
			/**
			 *
			 */
			double getLength() const;
			/**
			 *
			 */
			bool connects( RectangleShapePtr aNode) const;

			/**
			 *
			 */
			virtual void handleActivated() override;
			/**
			 *
			 */
			virtual void handleSelection() override;
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override;
			//@}

			void setNode1(RectangleShapePtr aNode1)
			{
				node1 = aNode1;
			}
			void setNode2(RectangleShapePtr aNode2)
			{
				node2 = aNode2;
			}

		protected:
			RectangleShapePtr getNode1()
			{
				return node1;
			}
			const RectangleShapePtr getNode1() const
			{
				return node1;
			}

			RectangleShapePtr getNode2()
			{
				return node2;
			}
			const RectangleShapePtr getNode2() const
			{
				return node2;
			}

		private:
			RectangleShapePtr node1;
			RectangleShapePtr node2;
			std::string title;
			int lineWidth;
			int arrowHeadSize;
			Point top;
			Point right;
			Point left;
	};
	//	class LineShape
} // namespace View
#endif // DANU_LINESHAPE_HPP_
