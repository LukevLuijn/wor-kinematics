//
// Created by luke on 07-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_PARTICLEFILTER_H
#define WOR_WORLD_KINEMATICA_PARTICLEFILTER_H

#include "AbstractFilter.h"

#include <random>

namespace Model
{
    struct Particle {
        Point location;
        double weight = 1;
    };

    class ParticleFilter : public AbstractFilter
    {
    public:
        /**
         *
         */
        ParticleFilter();
        /**
         *
         */
        ~ParticleFilter() override = default;
        /**
         *
         */
        void iterate(Point& perceivedPosition, const Point& targetPosition,
                     std::vector<AbstractSensorPtr>& sensors) override;
        /**
         *
         */
        [[nodiscard]] std::string asString() const override;

    private:
        /**
         *
         */
        void initializeParticles();
        /**
         *
         */
        void controlUpdate(const Point& perceivedPosition, const Point& targetPosition);
        /**
         *
         */
        void measurementUpdate(std::vector<AbstractSensorPtr>& sensors);
        /**
         *
         */
        void resampleParticles();
        /**
         *
         */
        Point calculateNewPosition();

    private:
        uint32_t gridSize, numParticles;

        std::vector<Particle> particles;
        std::vector<double> weights;


        std::mt19937 randomEngine;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_PARTICLEFILTER_H
