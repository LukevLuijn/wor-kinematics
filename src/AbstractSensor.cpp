#include "AbstractSensor.hpp"

#include "AbstractAgent.hpp"
#include "Logger.hpp"

namespace Model
{
	/**
	 *
	 */
	AbstractSensor::AbstractSensor() :
								agent( nullptr),
								running( false)
	{
	}
	/**
	 *
	 */
	AbstractSensor::AbstractSensor( AbstractAgent* anAgent) :
								agent( anAgent),
								running( false)
	{

	}
	/**
	 *
	 */
	AbstractSensor::~AbstractSensor()
	{
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void AbstractSensor::setOn( unsigned long aSleepTime /*= 100*/)
	{
		std::unique_lock< std::recursive_mutex > lock( sensorMutex);
		if (running == false)
		{
			running = true;
			std::thread newSensorThread( [this, aSleepTime]
										  {	run(aSleepTime);});
			sensorThread.swap( newSensorThread);
		}
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void AbstractSensor::setOff()
	{
		std::unique_lock< std::recursive_mutex > lock( sensorMutex);

		running = false;
		//sensorThread.interrupt();
		sensorThread.join();
	}
	/**
	 *
	 */
	void AbstractSensor::sendPercept( std::shared_ptr< AbstractPercept > anAbstractPercept)
	{
		agent->addPercept( anAbstractPercept);
	}
	/**
	 *
	 */
	void AbstractSensor::run( unsigned long UNUSEDPARAM(aSleepTime))
	{
		try
		{
			while (running == true)
			{
				std::shared_ptr< AbstractStimulus > currentStimulus = getStimulus();
				std::shared_ptr< AbstractPercept > currentPercept = getPerceptFor( currentStimulus);
				sendPercept( currentPercept);

				//std::this_thread::sleep_for( std::chrono::milliseconds( aSleepTime));


				// this should be either the last call in the loop or
				// part of the while.
				//			if (sensorThread.interruption_requested())
				//			{
				//				break;
				//			}
			}
		}
		catch (std::exception& e)
		{
			std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
		}
		catch (...)
		{
			std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
		}
	}
	/**
	 *
	 */
	void AbstractSensor::attachAgent( AbstractAgent* anAgent)
	{
		agent = anAgent;
	}
	/**
	 *
	 */
	void AbstractSensor::detachAgent()
	{
		agent = nullptr;
	}
	/**
	 *
	 */
	std::string AbstractSensor::asString() const
	{
		return "AbstractSensor";
	}
	/**
	 *
	 */
	std::string AbstractSensor::asDebugString() const
	{
		return asString();
	}
} // namespace Model
