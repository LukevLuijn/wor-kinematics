#include "ModelObject.hpp"

#include "TypeInfo.hpp"

#include <sstream>

namespace Model
{
    /**
	 *
	 */
    ModelObject::ModelObject() : objectId(Base::ObjectId::newObjectId())
    {
    }
    /**
	 *
	 */
    ModelObject::~ModelObject()
    {
    }
    /**
	 *
	 */
    std::string ModelObject::asString() const
    {
        std::ostringstream os;
        os << __PRETTY_FUNCTION__ << " : " << Utils::typeinfoFor(*this) << ", "
           << "ObjectId: " << getObjectId();
        return os.str();
    }
    /**
	 *
	 */
    std::string ModelObject::asDebugString() const
    {
        std::ostringstream os;
        os << __PRETTY_FUNCTION__ << " : " << Utils::typeinfoFor(*this) << "\n";
        os << "ObjectId: " << getObjectId();
        return os.str();
    }
}// namespace Model
