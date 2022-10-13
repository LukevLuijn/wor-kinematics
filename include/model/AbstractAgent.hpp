#ifndef ABSTRACTAGENT_HPP_
#define ABSTRACTAGENT_HPP_

#include "Config.hpp"

#include "ModelObject.hpp"
#include "Queue.hpp"

#include <memory>
#include <vector>

/**
 *
 */
namespace Model
{
    class AbstractSensor;
    typedef std::shared_ptr<AbstractSensor> AbstractSensorPtr;

    class AbstractStimulus;
    typedef std::shared_ptr<AbstractStimulus> AbstractStimulusPtr;

    class AbstractActuator;
    typedef std::shared_ptr<AbstractActuator> AbstractActuatorPtr;

    class AbstractPercept;
    typedef std::shared_ptr<AbstractPercept> AbstractPerceptPtr;

    class AbstractAgent;
    typedef std::shared_ptr<AbstractAgent> AbstractAgentPtr;

    /**
	 *
	 */
    class AbstractAgent : public ModelObject
    {
    public:
        /**
			 *
			 */
        AbstractAgent();
        /**
			 *
			 */
        virtual ~AbstractAgent();
        /**
			 *
			 */
        virtual void attachSensor(std::shared_ptr<AbstractSensor> aSensor, bool attachSensorToAgent = false);
        /**
			 *
			 */
        virtual void attachActuator(std::shared_ptr<AbstractActuator> anActuator, bool attachActuatorToAgent = false);
        /**
			 *
			 */
        virtual void addPercept(std::shared_ptr<AbstractPercept> anAbstractPercept);
        /**
			 *
			 */
        virtual void startActing() = 0;
        /**
			 *
			 */
        virtual void stopActing() = 0;
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
        std::vector<std::shared_ptr<AbstractSensor>> sensors;
        std::vector<std::shared_ptr<AbstractActuator>> actuators;
        Base::Queue<std::shared_ptr<AbstractPercept>> perceptQueue;

    private:
    };
}// namespace Model
#endif// ABSTRACTAGENT_HPP_
