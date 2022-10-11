//
// Created by luke on 11-10-22.
//

#include "PathShape.h"

#include "Logger.hpp"

namespace View
{
    PathShape::PathShape(const Model::PathPtr& aPath)
        : Shape(aPath), pathColor(wxColor(0, 0, 0)), positionColor(wxColor(0, 0, 0))
    {
    }
    PathShape::PathShape(const Model::PathPtr& aPath, const wxColor& aPathColor, const wxColor& aPositionColor)
        : Shape(aPath), pathColor(aPathColor), positionColor(aPositionColor)
    {
    }
    void PathShape::draw(wxDC& dc)
    {
        wxPen pathPen(pathColor, 1, wxPENSTYLE_SHORT_DASH);
        wxPen positionPen(positionColor, 1, wxPENSTYLE_SOLID);

        std::vector<Point> pathPoints = getPath()->getPoints();

        dc.SetBrush(positionColor);
        for (auto it = std::next(pathPoints.begin()); it != pathPoints.end(); ++it)
        {
            Point start = *(it - 1);

            dc.SetPen(pathPen);
            dc.DrawLine(start, *(it));

            dc.SetPen(positionPen);
            dc.DrawCircle(start, 2);
        }
    }
    bool PathShape::occupies(const Point& aPoint) const
    {
        std::vector<Point> pathPoints = getPath()->getPoints();

        if (std::any_of(pathPoints.begin(), pathPoints.end(), [&aPoint](const Point& point) {
                return point == aPoint;
            }))
        {
            return true;
        }
        return false;
    }
    Point PathShape::getCentre() const
    {
        std::vector<Point> pathPoints = getPath()->getPoints();

        uint64_t sumX = 0, sumY = 0;
        for (const Point& point : pathPoints)
        {
            sumX += static_cast<uint64_t>(point.x);
            sumY += static_cast<uint64_t>(point.y);
        }

        auto centreX = static_cast<int32_t>(sumX / pathPoints.size());
        auto centreY = static_cast<int32_t>(sumY / pathPoints.size());

        return Point(centreX, centreY);
    }
    void PathShape::setCentre(const Point& UNUSEDPARAM(aPoint))
    {
        LOG("cannot set center, all points are absolute.");
    }
    Model::PathPtr PathShape::getPath() const
    {
        return std::dynamic_pointer_cast<Model::Path>(getModelObject());
    }
}// namespace View