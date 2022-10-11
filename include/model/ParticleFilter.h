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
        ParticleFilter();
        explicit ParticleFilter(const Point& aInitialPosition);
        ~ParticleFilter() override = default;

        void iterate(Point& perceivedPosition, const Point& targetPosition,
                     std::vector<AbstractSensorPtr>& sensors) override;

        std::string asString() const override;

    private:
        void initializeParticles();

        void controlUpdate(const Point& perceivedPosition, const Point& targetPosition);
        void measurementUpdate(std::vector<AbstractSensorPtr>& sensors);
        void resampleParticles();
        Point calculateNewPosition();


    private:
        static constexpr uint8_t GRID_SIZE = 32;// 64^2 = 4096 particles TODO maybe to big.
        static constexpr uint16_t NUM_PARTICLES = GRID_SIZE * GRID_SIZE;

        std::array<Particle, NUM_PARTICLES> particles;
        std::array<double, NUM_PARTICLES> weights;

        std::mt19937 randomEngine;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_PARTICLEFILTER_H
