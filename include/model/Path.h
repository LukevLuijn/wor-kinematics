//
// Created by luke on 11-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_PATH_H
#define WOR_WORLD_KINEMATICA_PATH_H

#include "ModelObject.hpp"
#include "Point.hpp"

namespace Model
{
    class Path;
    typedef std::shared_ptr<Path> PathPtr;

    class Path : public ModelObject
    {
    public:
        Path() = default;
        explicit Path(std::vector<Point> aPath);
        ~Path() override = default;

        bool operator==(const Path& other);

        void addPoint(const Point& aPoint);
        void removePoint(int32_t index);

        Point getPoint(std::size_t index) const;
        std::vector<Point> getPoints() const;

        void setPoint(const Point& aPoint, std::size_t index);
        void setPath(const std::vector<Point>& aPath);

        std::string asString() const override;
        std::string asDebugString() const override;

    private:
        std::vector<Point> points;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_PATH_H
