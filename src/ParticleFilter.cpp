//
// Created by luke on 07-10-22.
//

#include "ParticleFilter.h"

#include "Logger.hpp"
#include "MathUtils.hpp"

#include "CompassSensor.h"
#include "LidarSensor.h"

#include "MainApplication.hpp"

#include <chrono>

namespace Model
{
    ParticleFilter::ParticleFilter()
        : AbstractFilter(Filters_e::PARTICLE_FILTER), gridSize(32), numParticles(gridSize * gridSize)
    {
        if (Application::MainApplication::isArgGiven("-particle"))
        {
            gridSize = static_cast<uint32_t>(std::stoi(Application::MainApplication::getArg("-particle").value));
            numParticles = gridSize * gridSize;
        }

        std::random_device randomDevice;
        randomEngine = std::mt19937(randomDevice());

        initializeParticles();
    }
    void ParticleFilter::iterate(Point& perceivedPosition, const Point& targetPosition,
                                 std::vector<AbstractSensorPtr>& sensors)
    {
        controlUpdate(perceivedPosition, targetPosition);
        measurementUpdate(sensors);
        resampleParticles();
        perceivedPosition = calculateNewPosition();
    }
    std::string ParticleFilter::asString() const
    {
        return "ParticleFilter";
    }
    void ParticleFilter::initializeParticles()
    {
        std::uniform_int_distribution<uint8_t> distribution(0, static_cast<uint8_t>(gridSize));
        uint32_t stepSize = 1024 / gridSize;

        for (std::size_t i = 0; i < numParticles; ++i)
        {
            weights.emplace_back(1);

            auto locationX = static_cast<int32_t>(stepSize * distribution(randomEngine));
            auto locationY = static_cast<int32_t>(stepSize * distribution(randomEngine));

            particles.emplace_back(Particle{Point(locationX, locationY), weights[i]});
        }
    }
    void ParticleFilter::controlUpdate(const Point& perceivedPosition, const Point& targetPosition)
    {
        uint8_t standardDeviation = 2;
        std::normal_distribution<double> distribution(0, standardDeviation);

        auto relativeMovementX =
                static_cast<int32_t>((targetPosition.x - perceivedPosition.x) + distribution(randomEngine));
        auto relativeMovementY =
                static_cast<int32_t>((targetPosition.y - perceivedPosition.y) + distribution(randomEngine));

        for (Particle& particle : particles)
        {
            particle.location += Point(relativeMovementX, relativeMovementY);
        }
    }
    void ParticleFilter::measurementUpdate(std::vector<AbstractSensorPtr>& sensors)
    {
        double orientation;
        LidarData lidarDataRobot;

        for (AbstractSensorPtr& sensor : sensors)
        {
            if (sensor.get()->asString() == "CompassSensor")
            {
                auto* compass = dynamic_cast<CompassSensor*>(sensor.get());
                std::shared_ptr<AbstractStimulus> stimulus = compass->getStimulus();
                std::shared_ptr<AbstractPercept> percept = compass->getPerceptFor(stimulus);

                orientation = dynamic_cast<CompassPercept*>(percept.get())->orientation;
            }
            else if (sensor.get()->asString() == "LidarSensor")
            {
                auto* odometer = dynamic_cast<LidarSensor*>(sensor.get());
                std::shared_ptr<AbstractStimulus> stimulus = odometer->getStimulus();
                std::shared_ptr<AbstractPercept> percept = odometer->getPerceptFor(stimulus);

                lidarDataRobot = dynamic_cast<LidarPercept*>(percept.get())->measurements;
            }
        }

        for (std::size_t i = 0; i < particles.size(); ++i)//x32^2
        {
            LidarData lidarDataParticle =
                    LidarSensor::getReadingFromLocation(particles[i].location, orientation);// 180 readings

            double totalDistanceDelta = 0;
            for (std::size_t j = 0; j < lidarDataRobot.size(); ++j)// x180
            {
                totalDistanceDelta += std::abs(lidarDataRobot[j].distance - lidarDataParticle[j].distance);
            }

            double newWeight = (totalDistanceDelta > Utils::ALMOST_ZERO) ? (1.0 / totalDistanceDelta) : 1;
            particles[i].weight = weights[i] = newWeight;
        }// total of 184320 readings taken and compared per iteration. (approx 100 ms)
    }
    void ParticleFilter::resampleParticles()
    {
        decltype(particles) selectedParticles;
        std::discrete_distribution<uint16_t> discreteDistribution(weights.begin(), weights.end());

        for (std::size_t i = 0; i < numParticles; ++i)
        {
            selectedParticles.emplace_back(particles[discreteDistribution(randomEngine)]);
        }
        particles = selectedParticles;
    }
    Point ParticleFilter::calculateNewPosition()
    {
        Particle bestParticle;
        double greatestWeight = 0;
        std::for_each(particles.begin(), particles.end(), [&bestParticle, &greatestWeight](const Particle& particle) {
            if (particle.weight > greatestWeight)
            {
                greatestWeight = particle.weight;
                bestParticle = particle;
            }
        });
        return bestParticle.location;
    }
}// namespace Model