#ifndef VIEWOBJECT_HPP_
#define VIEWOBJECT_HPP_

#include "Config.hpp"

#include "ModelObject.hpp"
#include "ObjectId.hpp"
#include "Observer.hpp"

namespace View
{
    /**
	 *
	 */
    class ViewObject : public Base::Observer
    {
    public:
        /**
			 * @name Constructors and destructor
			 */
        //@{
        /**
			 * Default constructor
			 */
        ViewObject();
        /**
			 * ModelObject constructor
			 */
        explicit ViewObject(Model::ModelObjectPtr aModelObject);
        /**
			 * Destructor
			 */
        virtual ~ViewObject();
        //@}

        /**
			 * @name Accessors and mutators
			 */
        //@{
        /**
			 * Accessor for the ViewObject objectId
			 */
        const Base::ObjectId& getObjectId() const;
        /**
			 * Accessor for the ModelObject
			 */
        Model::ModelObjectPtr getModelObject() const;
        /**
			 * Mutator for the ModelObject
			 *
			 * See rebindModelObject and resetViewObject
			 */
        void setModelObject(Model::ModelObjectPtr aModelObject);
        //@}
    protected:
        /**
			 * @name ViewObject implementation
			 */
        //@{
        /**
			 * Stops observing the current ModelObject, starts observing the new ModelObjects and
			 * displays it by calling ViewObject::resetViewObject
			 */
        virtual void rebindModelObject(Model::ModelObjectPtr aModelObject);
        //@}

    private:
        Base::ObjectId objectId;
        Model::ModelObjectPtr modelObject;
    };
}// namespace View
#endif// VIEWOBJECT_HPP_
