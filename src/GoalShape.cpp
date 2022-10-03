#include "GoalShape.hpp"

#include "Goal.hpp"
#include "Logger.hpp"

#include <sstream>

namespace View
{
	/**
	 *
	 */
	GoalShape::GoalShape( Model::GoalPtr aGoal) :
								WayPointShape( std::dynamic_pointer_cast<Model::WayPoint>(aGoal))
	{
	}
	/**
	 *
	 */
	GoalShape::~GoalShape()
	{
	}
	/**
	 *
	 */
	Model::GoalPtr GoalShape::getGoal() const
	{
		return std::dynamic_pointer_cast<Model::Goal>(getModelObject());
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	void GoalShape::setGoal( Model::GoalPtr aGoal)
	{
		setModelObject(std::dynamic_pointer_cast<Model::ModelObject>(aGoal));
	}
	/**
	 *
	 */
	void GoalShape::handleNotification()
	{
	}
	/**
	 *
	 */
	std::string GoalShape::asString() const
	{
		std::ostringstream os;

		os << "GoalShape " << WayPointShape::asString();

		return os.str();
	}
	/**
	 *
	 */
	std::string GoalShape::asDebugString() const
	{
		std::ostringstream os;

		os << "GoalShape:\n";
		os << WayPointShape::asDebugString() << "\n";
		// Goal has not use in this version so we do not print it
		//	if(getGoal())
		//	{
		//		os << getGoal()->asDebugString();
		//	}

		return os.str();
	}
} // namespace View
