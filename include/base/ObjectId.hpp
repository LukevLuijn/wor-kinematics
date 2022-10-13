#ifndef OBJECTID_HPP_
#define OBJECTID_HPP_

#include "Config.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


namespace Base
{
    /**
	 * The ObjectId class is also used in Dagda. Therefore it has the required
	 * PortableServer::ObjectId / unbounded sequence interface.
	 *
	 * The OID class is 16 bytes, this class is 24 bytes. If one is memory constraint
	 * the OID class may be of some help. Be warned: the OID class is not actively
	 * maintained anymore (but than, who reeds this besides me, the maintainer).
	 */
    class ObjectId : public std::vector<unsigned char>
    {
    public:
        typedef unsigned char value_type;
        typedef unsigned char* pointer;
        typedef const unsigned char* const_pointer;
        typedef unsigned char& reference;
        typedef const unsigned char& const_reference;
        typedef std::vector<unsigned char> base;

        /**
			 * If an ObjectId should be *really* universal unique every application should have its own namespace
			 */
        static std::string objectIdNamespace;

        /**
			 * This function returns an ObjectId that is guaranteed to be unique in the application it
			 * is generated in. If multiple application use the same library it is the responsibility of
			 * the applications to ensure uniqueness between applications.
			 */
        static ObjectId newObjectId();

        /**
			 * @name CORBA required interface for PortableServer::ObjectId and sequence< T >
			 */
        //@{
        /**
			 * Default ctor.
			 *
			 * Also specified by
			 */
        ObjectId() : base()
        {
        }
        /**
			 * Maximum ctor.
			 *
			 * @param max Initial elements reservation
			 */
        explicit ObjectId(unsigned long max) : base(max)
        {
        }
        /**
			 * Data ctor.
			 *
			 * @param max 		Initial elements reservation. Is actually ignored
			 * @param length 	Length of the data
			 * @param data 		Start of the data
			 * @param release 	If true the sequences owns and manages any containing pointers
			 * 				  	Is actually ignored and always true.
			 */
        ObjectId(unsigned long /*max*/, unsigned long len, unsigned char* aDataPointer, bool /*rel = true-*/)
            : base(aDataPointer, aDataPointer + len)
        {
        }
        /**
			 *	Makes a deep copy.
			 */
        ObjectId(const ObjectId& anObjectId) : base(anObjectId)
        {
        }
        /**
			 * Also specified by PortableServer::ObjectId and sequence< T >
			 */
        virtual ~ObjectId();
        /**
			 * @return The virtual maximum of bytes for this ObjectId. This maximum can safely be ignored.
			 *
			 */
        unsigned long maximum() const
        {
            return base::capacity();
        }

        /**
			 * If the current length of the sequence is less than the new length we append new elements
			 * which are created by calling the default constructor.  If the current length is more
			 * than the new length we just cut of the tail of the sequence.
			 *
			 * @param length The new length of the sequence
			 */
        void length(unsigned long len)
        {
            if (base::size() < len)
            {
                base::insert(base::end(), len - base::size(), value_type());
            }
            else
            {
                base::erase(base::begin() + len, base::end());
            }
        }

        /**
			 *
			 * @return The current length of the ObjectId
			 */
        unsigned long length() const
        {
            return base::size();
        }

        /**
			 *
			 * @param index
			 * @return The mutable character at index.
			 */
        reference operator[](unsigned long index)
        {
            return base::operator[](index);
        }

        /**
			 *
			 * @param index
			 * @return The non-mutable character at index.
			 */
        const_reference operator[](unsigned long index) const
        {
            return base::operator[](index);
        }

        /**
			 * This returns always true for a value container
			 *
			 * @return true if this container owns the data.
			 */
        bool release() const
        {
            return true;
        }

        /**
			 * @param max Is ignored
			 * @param rel Is ignored and is treated as true
			 */
        void replace(unsigned long /*max*/, unsigned long len, pointer newData, bool /*rel = true*/)
        {
            base::erase(base::begin(), base::end());
            base::insert(base::begin(), newData, newData + len);
        }

        /**
			 *
			 * @param 	orphan
			 * @return 	The ObjectId as a modifyable continues buffer.
			 * 			If the buffer is modified as that it expands the buffer this \will
			 * 			lead to failure eventually
			 */
        pointer get_buffer(bool orphan = false)
        {
            if (orphan)
            {
                pointer t = allocbuf(base::size());
                std::copy(base::begin(), base::end(), t);
                base::erase(base::begin(), base::end());
                return t;
            }
            else
            {
                return &base::operator[](0);
            }
        }

        /**
			 *
			 * @return The ObjectId as a readonly continues buffer.
			 */
        const_pointer get_buffer() const
        {
            return &base::operator[](0);
        }

        /**
			 * Do not use. It always throws an Danu::InvalidRequest exception.
			 *
			 * @param nelems
			 * @return a new-ed buffer
			 */
        static pointer allocbuf(unsigned long nelems)
        {
            return new value_type[nelems];
        }
        /**
			 * Do not use. It always throws an Danu::InvalidRequest exception.
			 *
			 * @param aT The buffer to free
			 */
        static void freebuf(pointer aT)
        {
            delete[] aT;
        }
        //@} // CORBA required interface

        /**
			 *
			 */
        template<typename InputIterator>
        ObjectId(InputIterator first, InputIterator last) : base(first, last)
        {
        }
        /**
			 *
			 */
        explicit ObjectId(const std::string& anObjectIdString) : base(anObjectIdString.begin(), anObjectIdString.end())
        {
        }
        /**
			 *
			 */
        ObjectId& operator=(const ObjectId& anObjectId)
        {
            if (this != &anObjectId)
            {
                base::operator=(anObjectId);
            }
            return *this;
        }

        /**
			 * @param anObjectId
			 * @return True if the Danu::ObjectId part is the same
			 */
        bool operator==(const ObjectId& anObjectId) const;
        /**
			 * @param anObjectId
			 * @return True if the Danu::ObjectId part is less
			 */
        bool operator<(const ObjectId& anObjectId) const;
        /**
			 * Calling obj1.fromString( obj2.toString()) has the same effect as assignment, obj1 = obj2.
			 *
			 * @return std::string
			 */
        std::string toString() const
        {
            return std::string(begin(), end());
        }
        /**
			 * Calling obj1.fromString( obj2.toString()) has the same effect as assignment, obj1 = obj2.
			 *
			 * @param anObjectIdString A string representation of an ObjectId.
			 */
        void fromString(const std::string& anObjectIdString)
        {
            clear();
            insert(begin(), anObjectIdString.begin(), anObjectIdString.end());
        }
        /**
			 *
			 */
        bool isNull() const;
        /**
			 *
			 */
        bool isValid() const;
        /**
			 * @name Debug functions
			 */
        //@{
        /**
			 * Returns a 1-line description of the object
			 */
        /*virtual*/
        std::string asString() const;
        /**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
        /*virtual*/
        std::string asDebugString() const;
        //@}
    };
    //	class ObjectId

    std::ostream& operator<<(std::ostream& os, const ObjectId& anObjectId);
}// namespace Base
#endif// OBJECTID_HPP_
