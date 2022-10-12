//
// Created by luke on 04-10-22.
//

#include <chrono>

#include "NoisySensor.h"

// TODO remove
#include "Logger.hpp"

namespace Model
{
    NoisySensor::NoisySensor(double mean, double deviation)
        : normalDistribution(mean, deviation),
          randomEngine(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()))
    {

    }
    double NoisySensor::noisify(double value)
    {
        return value + normalDistribution(randomEngine);
    }
    void NoisySensor::setStandardDeviation(double aDeviation)
    {
        LOG("deviation reset: ", aDeviation);
        normalDistribution = std::normal_distribution<double> (normalDistribution.mean(), aDeviation);
    }

}