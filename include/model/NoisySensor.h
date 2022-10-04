//
// Created by luke on 04-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_REALWORLDSENSOR_H
#define WOR_WORLD_KINEMATICA_REALWORLDSENSOR_H

#include <random>

namespace Model
{
    class NoisySensor
    {
    public:
        NoisySensor(double minDistribution, double maxDistribution);
        virtual ~NoisySensor() = default;

    protected:
        double noisify(double value);

    private:
        std::normal_distribution<double> distribution;
        std::default_random_engine generator;
    };
}

#endif//WOR_WORLD_KINEMATICA_REALWORLDSENSOR_H
