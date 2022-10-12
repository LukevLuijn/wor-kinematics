//
// Created by luke on 11-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_PATH_H
#define WOR_WORLD_KINEMATICA_PATH_H

#include "ModelObject.hpp"
#include "Point.hpp"
#include "Widgets.hpp"

namespace Model
{
    class Path;
    typedef std::shared_ptr<Path> PathPtr;

    class Path : public ModelObject
    {
    public:
        Path(std::string  name, const wxColor& aPathColor, const wxColor& aPositionColor);
        explicit Path(std::string  name,std::vector<Point> aPath, const wxColor& aPathColor, const wxColor& aPositionColor);
        ~Path() override = default;

        bool operator==(const Path& other);

        void addPoint(const Point& aPoint);
        void removePoint(int32_t index);

        Point getPoint(std::size_t index) const;
        std::vector<Point> getPoints() const;

        void setPoint(const Point& aPoint, std::size_t index);
        void setPath(const std::vector<Point>& aPath);
        void setPathActive(bool active);

        std::string asString() const override;
        std::string asDebugString() const override;

        wxColour getPathColor() const
        {
            return pathColor;
        }
        wxColour getPositionColor() const
        {
            return positionColor;
        }

        bool isPathActive() const
        {
            return isActive;
        }
        std::string getName() const
        {
            return name;
        }
    private:
        std::string name;
        std::vector<Point> points;
        bool isActive;
        wxColor pathColor, positionColor;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_PATH_H
