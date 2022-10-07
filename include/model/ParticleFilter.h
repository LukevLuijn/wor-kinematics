//
// Created by luke on 07-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_PARTICLEFILTER_H
#define WOR_WORLD_KINEMATICA_PARTICLEFILTER_H

#include "AbstractFilter.h"

namespace Model
{
    class ParticleFilter : public AbstractFilter
    {
    public:
        ParticleFilter();
        explicit ParticleFilter(const Point& aInitialPosition);
        ~ParticleFilter() override = default;

        Point getMeasuredPosition(const Point& position,std::vector<AbstractSensorPtr>& sensors) override;
        void iterate(Point& perceivedPosition, const Point& targetPosition, const Point& measuredPosition) override;

        std::string asString() const override;
    };
}

#endif//WOR_WORLD_KINEMATICA_PARTICLEFILTER_H
