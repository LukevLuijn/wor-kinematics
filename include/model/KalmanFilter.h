//
// Created by luke on 07-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_KALMANFILTER_H
#define WOR_WORLD_KINEMATICA_KALMANFILTER_H

#include "AbstractFilter.h"
#include "Matrix.hpp"

namespace Model
{
    class KalmanFilter : public AbstractFilter
    {
    public:
        KalmanFilter();
        explicit KalmanFilter(const Point& aInitialPosition);
        ~KalmanFilter() override = default;

//        Point getMeasuredPosition(const Point& position, std::vector<AbstractSensorPtr>& sensors) override;
        void iterate(Point& perceivedPosition, const Point& targetPosition, std::vector<AbstractSensorPtr>& sensors) override;

        std::string asString() const override;

    private:
        Point getMeasuredPosition(const Point& position, std::vector<AbstractSensorPtr>& sensors);

    private:
        Utils::Matrix<double, 2, 1> belief;// previous state vector
        Utils::Matrix<double, 2, 2> error; // previous process covariance

        const Utils::Matrix<double, 2, 2> A = {{1, 0}, {0, 1}};
        const Utils::Matrix<double, 2, 2> R = {{1, 0}, {0, 1}};// sensor covariance matrix
    };
}

#endif//WOR_WORLD_KINEMATICA_KALMANFILTER_H
