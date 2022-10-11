//
// Created by luke on 07-10-22.
//

#include "ParticleFilter.h"

#include "Logger.hpp"
#include "MathUtils.hpp"

#include "LidarSensor.h"
#include "CompassSensor.h"

#include <chrono>

namespace Model
{
    ParticleFilter::ParticleFilter()
    {
        std::random_device randomDevice;
        randomEngine = std::mt19937(randomDevice());

        initializeParticles();
    }
    ParticleFilter::ParticleFilter(const Point& aInitialPosition)
    {
        // TODO
        UNUSEDCAST(aInitialPosition);

        std::random_device randomDevice;
        randomEngine = std::mt19937(randomDevice());

        initializeParticles();
    }

    void ParticleFilter::iterate(Point& perceivedPosition, const Point& targetPosition,
                                 std::vector<AbstractSensorPtr>& sensors)
    {
        // == 1. CONTROL UPDATE ==

        uint8_t standardDeviation = 2;// TODO change maybe?
        std::normal_distribution<double> distribution(0, standardDeviation);

        auto relativeMovementX =
                static_cast<int32_t>((targetPosition.x - perceivedPosition.x) + distribution(randomEngine));
        auto relativeMovementY =
                static_cast<int32_t>((targetPosition.y - perceivedPosition.y) + distribution(randomEngine));

        for (Particle& particle : particles)
        {
            particle.location += Point(relativeMovementX, relativeMovementY);
        }

        // == 2. RETRIEVE MEASUREMENTS ==

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

        // == 3. MEASUREMENT UPDATE ==

        for (std::size_t i = 0; i < particles.size(); ++i)
        {
            LidarData lidarDataParticle = LidarSensor::getReadingFromLocation(particles[i].location, orientation);

            double totalDistanceDelta = 0;
            for (std::size_t j = 0; j < lidarDataRobot.size(); ++j)
            {
                totalDistanceDelta +=
                        std::sqrt(std::pow(lidarDataRobot[j].distance - lidarDataParticle[j].distance, 2));
            }

            double newWeight = (totalDistanceDelta > Utils::ALMOST_ZERO) ? 1.0 / totalDistanceDelta : 1;
            particles[i].weight = weights[i] = newWeight;
        }

        // == 4. RESAMPLE PARTICLES ==

        decltype(particles) selectedParticles;
        std::discrete_distribution<uint16_t> discreteDistribution(weights.begin(), weights.end());

        for (std::size_t i = 0; i < NUM_PARTICLES; ++i)
        {
            selectedParticles[i] = particles[discreteDistribution(randomEngine)];
        }
        particles = selectedParticles;

        // == 5. NEW POSITION ==

        Particle bestParticle;
        double greatestWeight = 0;
        std::for_each(particles.begin(), particles.end(), [&bestParticle, &greatestWeight](const Particle& particle) {
            if (particle.weight > greatestWeight)
            {
                greatestWeight = particle.weight;
                bestParticle = particle;
            }
        });

        perceivedPosition = bestParticle.location;
    }
    std::string ParticleFilter::asString() const
    {
        return "ParticleFilter";
    }
    void ParticleFilter::initializeParticles()
    {
        std::uniform_int_distribution<uint8_t> distribution(0, GRID_SIZE);
        uint16_t stepSize = 1024 / GRID_SIZE;

        for (std::size_t i = 0; i < NUM_PARTICLES; ++i)
        {
            weights[i] = 1;

            auto locationX = static_cast<int32_t>(stepSize * distribution(randomEngine));
            auto locationY = static_cast<int32_t>(stepSize * distribution(randomEngine));

            particles[i] = Particle{Point(locationX, locationY), weights[i]};
        }
    }
}// namespace Model