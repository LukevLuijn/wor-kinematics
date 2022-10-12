//
// Created by luke on 07-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_ABSTRACTFILTER_H
#define WOR_WORLD_KINEMATICA_ABSTRACTFILTER_H

#include <memory>

#include "Point.hpp"

namespace Model
{
    class AbstractSensor;
    typedef std::shared_ptr<AbstractSensor> AbstractSensorPtr;

    enum class Filters_e : uint8_t
    {
        KALMAN_FILTER,
        PARTICLE_FILTER
    };

    class AbstractFilter
    {
    public:
        explicit AbstractFilter(Filters_e aFilterType) : filterType(aFilterType)
        {
        }
        virtual ~AbstractFilter() = default;

        virtual void iterate(Point& perceivedPosition, const Point& targetPosition,
                             std::vector<AbstractSensorPtr>& sensors) = 0;

        virtual std::string asString() const = 0;
        virtual std::string asDebugString() const
        {
            return asString();
        }

        bool operator==(const AbstractFilter& other) const
        {
            return other.filterType == filterType;
        }
        [[nodiscard]] Filters_e getFilterType() const
        {
            return filterType;
        }

    protected:
        Filters_e filterType;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_ABSTRACTFILTER_H
