//
// Created by luke on 04-10-22.
//

#include <chrono>

#include "MainApplication.hpp"
#include "NoisySensor.h"

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
        normalDistribution = std::normal_distribution<double>(normalDistribution.mean(), aDeviation);
    }
    void NoisySensor::readFromConfigFile(const std::string& sensor)
    {
        if (Application::MainApplication::isArgGiven(sensor))
        {
            double stdDeviation = std::stod(Application::MainApplication::getArg(sensor).value);
            setStandardDeviation(stdDeviation);
        }
    }

}// namespace Model