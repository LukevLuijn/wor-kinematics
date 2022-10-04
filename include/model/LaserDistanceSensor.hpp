#ifndef LASERDISTANCESENSOR_HPP_
#define LASERDISTANCESENSOR_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"

namespace Model
{
	/**
	 *
	 */
	class DistanceStimulus : public AbstractStimulus
	{
		public:
			DistanceStimulus( 	double anAngle,
								double aDistance) :
				angle(anAngle),
				distance( aDistance)
		{
		}
		double angle;
		double distance;
	};
	// class DistanceStimulus

	/**
	 *
	 */
	class DistancePercept : public AbstractPercept
	{
		public:
			explicit DistancePercept( const DistanceStimulus& aDistanceStimulus) :
				angle(aDistanceStimulus.angle),
				distance( aDistanceStimulus.distance)
		{
		}
		DistancePercept(double anAngle,
						double aDistance) :
			angle(anAngle),
			distance( aDistance)
		{
		}
		double angle;
		double distance;
	};
	//	class DistancePercept

	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	/**
	 *
	 */
	class LaserDistanceSensor : public AbstractSensor
	{
		public:
			/**
			 *
			 */
			LaserDistanceSensor();
			/**
			 *
			 */
			explicit LaserDistanceSensor( Robot* aRobot);
			/**
			 *
			 */
			virtual ~LaserDistanceSensor();
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractStimulus > getStimulus() const override;
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractPercept > getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) override;
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
		protected:
		private:

	};
} // namespace Model
#endif /* LASERDISTANCESENSOR_HPP_ */
