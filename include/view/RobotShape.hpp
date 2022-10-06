#ifndef ROBOTSHAPE_HPP_
#define ROBOTSHAPE_HPP_

#include "Config.hpp"

#include "RectangleShape.hpp"
#include "Robot.hpp"
#include "Widgets.hpp"

#include <string>

namespace View
{
	class RobotWorldCanvas;

	/**
	 *
	 */
	class RobotShape :	public RectangleShape
	{
		public:
			/**
			 *
			 */
			explicit RobotShape( Model::RobotPtr aRobot);
			/**
			 *
			 */
			virtual ~RobotShape();
			/**
			 * @name Type safe accessors and mutators
			 */
			//@{
			/**
			 * Type safe accessor
			 */
			Model::RobotPtr getRobot() const;
			/**
			 * Type safe mutator
			 */
			void setRobot( Model::RobotPtr aRobot);
			//@}
			/**
			 *
			 */
			virtual std::string getNormalColour() const override
			{
				return "BLACK";
			}
			/**
			 *
			 */
			virtual std::string getSelectionColour() const override
			{
				return "GREY";
			}
			/**
			 *
			 */
			virtual std::string getActivationColour() const override
			{
				return "LIGHT GREY";
			}
			/**
			 * This function is called by the RobotWorldCanvas if enableActivation is set.
			 */
			virtual void handleActivated() override;
			/**
			 * This function is called by the RobotWorldCanvas if enableSelection is set.
			 */
			virtual void handleSelection() override;
			/**
			 * @name Observer functions
			 */
			//@{
			/**
			 * A Notifier will call this function if this Observer will handle the notifications of that
			 * Notifier. It is the responsibility of the Observer to filter any events it is interested in.
			 */
			virtual void handleNotification() override;
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
			virtual void setCentre( const Point& aPoint) override;
			//@}
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

			void setRobotWorldCanvas(RobotWorldCanvas* aRobotWorldCanvas)
			{
				robotWorldCanvas = aRobotWorldCanvas;
			}
		protected:
		private:
			RobotWorldCanvas* robotWorldCanvas;
	};
} // namespace View
#endif // ROBOTSHAPE_HPP_
