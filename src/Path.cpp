//
// Created by luke on 11-10-22.
//

#include "Path.h"

#include "Logger.hpp"

#include <utility>

namespace Model
{
    Path::Path(std::string  aName, const wxColor& aPathColor, const wxColor& aPositionColor)
        : name(std::move(aName)), pathColor(aPathColor), positionColor(aPositionColor)
    {
    }
    Path::Path(std::string  aName, std::vector<Point> aPath, const wxColor& aPathColor, const wxColor& aPositionColor)
        : name(std::move(aName)), points(std::move(aPath)), pathColor(aPathColor), positionColor(aPositionColor)
    {
    }
    bool Path::operator==(const Path& other)
    {
        bool isEqual = true;

        if ((name == other.name) && (points.size() == other.points.size()))
        {
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                if (points[i] != other.points[i])
                {
                    isEqual = false;
                    break;
                }
            }
        }
        else
        {
            isEqual = false;
        }
        return isEqual;
    }
    void Path::addPoint(const Point& aPoint)
    {
        points.emplace_back(aPoint);
    }
    void Path::removePoint(int32_t index)
    {
        points.erase(points.begin() + static_cast<int32_t>(index));
    }
    Point Path::getPoint(std::size_t index) const
    {
        Point point(0, 0);

        try
        {
            point = points.at(index);
        }
        catch (std::out_of_range& e)
        {
            LOG("exception caught", e.what());
            std::cerr << "exception caught, " << e.what() << std::endl;
        }

        return point;
    }
    std::vector<Point> Path::getPoints() const
    {
        return points;
    }
    void Path::setPoint(const Point& aPoint, std::size_t index)
    {
        try
        {
            points.at(index) = aPoint;
        }
        catch (std::out_of_range& e)
        {
            LOG("exception caught", e.what());
            std::cerr << "exception caught, " << e.what() << std::endl;
        }
    }
    void Path::setPath(const std::vector<Point>& aPath)
    {
        points = aPath;
    }
    std::string Path::asString() const
    {
        return "Path";
    }
    std::string Path::asDebugString() const
    {
        return asString();
    }
}// namespace Model