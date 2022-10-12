//
// Created by luke on 11-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_PATHSHAPE_H
#define WOR_WORLD_KINEMATICA_PATHSHAPE_H

#include "Shape.hpp"
#include "Path.h"

namespace View
{
    class PathShape : public Shape
    {
    public:
        explicit PathShape(const Model::PathPtr& aPath);
        ~PathShape() override = default;

        void draw(wxDC& dc) override;

        [[nodiscard]] bool occupies(const Point& aPoint) const override;
        [[nodiscard]] Point getCentre() const override;

        void setCentre(const Point& aPoint) override;

        [[nodiscard]] Model::PathPtr getPath() const;

        void handleNotification() override;
    };
}// namespace View

#endif//WOR_WORLD_KINEMATICA_PATHSHAPE_H
