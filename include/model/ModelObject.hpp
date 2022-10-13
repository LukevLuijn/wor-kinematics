#ifndef MODELOBJECT_HPP_
#define MODELOBJECT_HPP_

#include "Config.hpp"

#include "Notifier.hpp"
#include "ObjectId.hpp"

#include <iostream>
#include <memory>
#include <string>


namespace Model
{
    class ModelObject;
    typedef std::shared_ptr<ModelObject> ModelObjectPtr;

    /**
	 *
	 */
    class ModelObject : public Base::Notifier, public std::enable_shared_from_this<ModelObject>
    {
    public:
        /**
			 * @name Constructors and destructor
			 */
        //@{
        /**
			 *
			 */
        ModelObject();
        /**
			 * ModelObject may not be copied
			 */
        ModelObject(const ModelObject& aModelObject) = delete;
        /**
			 * ModelObject may not be copied
			 */
        ModelObject(ModelObject&& aModelObject) = delete;
        /**
			 *
			 */
        virtual ~ModelObject();
        //@}
        /**
			 * @name Operators
			 */
        //@{
        /**
			 * ModelObject may not be copied
			 */
        ModelObject& operator==(const ModelObject& aModelObject) = delete;
        /**
			 * ModelObject may not be copied
			 */
        ModelObject& operator==(ModelObject&& aModelObject) = delete;
        /**
			 * Equal to operator which compares the objectIds
			 *
			 * @param aModelObject
			 * @return true if objectIds are the same, false otherwise
			 */
        bool operator==(const ModelObject& aModelObject) const
        {
            return objectId == aModelObject.objectId;
        }
        /**
			 * Less than operator which compares the objectIds
			 *
			 * @param aModelObject
			 * @return true if lhs.objectId is less than rhs.objectId, false otherwise
			 */
        bool operator<(const ModelObject& aModelObject) const
        {
            return objectId < aModelObject.objectId;
        }
        //@}
        /**
			 *
			 * @return the objectId (identity) of the ModelObject
			 */
        const Base::ObjectId& getObjectId() const
        {
            return objectId;
        }
        /**
			 * Converts the contained ModelObject to a std::shared_ptr<DestinationType>
			 *
			 * @return std::shared_ptr<DestinationType>
			 */
        template<typename DestinationType>
        std::shared_ptr<DestinationType> toPtr()
        {
            return std::dynamic_pointer_cast<DestinationType>(shared_from_this());
        }
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
        Base::ObjectId objectId;
    };
}// namespace Model
#endif// MODELOBJECT_HPP_
