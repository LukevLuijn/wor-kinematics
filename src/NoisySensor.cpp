//
// Created by luke on 04-10-22.
//

#include <chrono>

#include "NoisySensor.h"


namespace Model
{
    NoisySensor::NoisySensor(double minDistribution, double maxDistribution)
        : distribution(minDistribution, maxDistribution),
          generator(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()))
    {

    }
    double NoisySensor::noisify(double value)
    {
        // TODO
        return value + distribution(generator);
    }
}