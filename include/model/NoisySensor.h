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
        /**
         *
         */
        NoisySensor(double mean, double deviation);
        /**
         *
         */
        virtual ~NoisySensor() = default;

    protected:
        /**
         * Add preconfigured noise to sensor value.
         */
        double noisify(double value);
        /**
         *
         */
        void setStandardDeviation(double aDeviation);
        /**
         *
         */
        void readFromConfigFile(const std::string& sensor);
    private:
        std::normal_distribution<double> normalDistribution;
        std::default_random_engine randomEngine;
    };
}

#endif//WOR_WORLD_KINEMATICA_REALWORLDSENSOR_H
