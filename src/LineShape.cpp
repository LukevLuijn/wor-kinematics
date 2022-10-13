#include "LineShape.hpp"

#include "Logger.hpp"
#include "Shape2DUtils.hpp"
#include "Size.hpp"

#include <algorithm>
#include <sstream>


namespace View
{
    /**
	 *
	 */
    LineShape::LineShape(RectangleShapePtr aNode1, RectangleShapePtr aNode2, const std::string& aTitle /*= ""*/,
                         int aLineWidth /*= 1*/, int anArrowHeadSize /*=10*/)
        : node1(aNode1), node2(aNode2), title(aTitle), lineWidth(aLineWidth), arrowHeadSize(anArrowHeadSize), top(),
          right(), left()
    {
    }
    /**
	 *
	 */
    LineShape::LineShape(Model::ModelObjectPtr aModelObject, RectangleShapePtr aNode1, RectangleShapePtr aNode2,
                         const std::string& aTitle /*= ""*/, int aLineWidth /*= 1*/, int anArrowHeadSize /*= 10*/)
        : Shape(aModelObject), node1(aNode1), node2(aNode2), title(aTitle), lineWidth(aLineWidth),
          arrowHeadSize(anArrowHeadSize), top(), right(), left()
    {
    }
    /**
	 *
	 */
    LineShape::~LineShape()
    {
    }
    /**
	 *
	 */
    void LineShape::draw(wxDC& dc)
    {
        if (isSelected())
        {
            dc.SetPen(wxPen(WXSTRING("RED"), lineWidth, wxPENSTYLE_SOLID));
        }
        else
        {
            dc.SetPen(wxPen(WXSTRING("BLACK"), lineWidth, wxPENSTYLE_SOLID));
        }

        dc.DrawLine(node1->getCentre().x, node1->getCentre().y, node2->getCentre().x, node2->getCentre().y);

        if (arrowHeadSize > 0)
        {
            drawHead(dc);
        }

        if (title == "" && node1->getTitle() != "" && node2->getTitle() != "")
        {
            title = node1->getTitle() + " to " + node2->getTitle();
        }
        Size textSize = dc.GetTextExtent(WXSTRING(title));

        Point textPoint = getBegin();
        double angle = Utils::Shape2DUtils::getAngle(node1->getCentre(), node2->getCentre());
        double dX = (getLength() / 2 - textSize.x / 2) * sin(angle);
        double dY = (getLength() / 2 - textSize.x / 2) * cos(angle);

        textPoint.x += static_cast<int>(dX);
        textPoint.y -= static_cast<int>(dY);

        double degreeAngle = angle * (180.0 / Utils::PI);
        double rotationAngle = 90 - degreeAngle;

        dc.DrawRotatedText(WXSTRING(title), textPoint, rotationAngle);
    }
    /**
	 *
	 */
    void LineShape::drawHead(wxDC& dc)
    {
        using Utils::PI;

        int arrowHeadSize = 10;

        // First we draw a triangle at (0.0)
        // Second we rotate the triangle with the angle the line makes with the Y-axis around it's centre
        // Than we move the centre of the triangle to the end of the line, but outside the node

        top = Point(0, -arrowHeadSize);
        right = Point(static_cast<int>(arrowHeadSize * std::sin(PI / 3)),
                      static_cast<int>(arrowHeadSize * std::cos(PI / 3)));
        left = Point(static_cast<int>(-arrowHeadSize * std::sin(PI / 3)),
                     static_cast<int>(arrowHeadSize * std::cos(PI / 3)));

        //double angle = getAngle();
        double angle = Utils::Shape2DUtils::getAngle(node1->getCentre(), node2->getCentre()) + 0.5 * PI;

        top = Utils::Shape2DUtils::rotate(top, angle);
        right = Utils::Shape2DUtils::rotate(right, angle);
        left = Utils::Shape2DUtils::rotate(left, angle);

        double shortenLine = 0.0;

        double dxRect = node2->getSize().x / 2;
        double dyRect = node2->getSize().y / 2;
        double boundaryAngle = std::atan(dxRect / dyRect);

        if (0.0 < angle && angle <= PI / 2)
        {
            if (angle < boundaryAngle)
            {
                shortenLine = dyRect / std::cos(angle);
            }
            else
            {
                shortenLine = dxRect / std::cos(PI / 2 - angle);
            }
        }
        else if (PI / 2 < angle && angle <= 2 * PI / 2)
        {
            if (angle < PI - boundaryAngle)
            {
                shortenLine = dxRect / std::cos(angle - PI / 2);
            }
            else
            {
                shortenLine = dyRect / std::cos(PI - angle);
            }
        }
        else if (2 * PI / 2 < angle && angle <= 3 * PI / 2)
        {
            if (angle < boundaryAngle + PI)
            {
                shortenLine = dyRect / std::cos(angle - PI);
            }
            else
            {
                shortenLine = dxRect / std::cos(3 * PI / 2 - angle);
            }
        }
        else if (3 * PI / 2 < angle && angle <= 4 * PI / 2)
        {
            if (angle < 2 * PI - boundaryAngle)
            {
                shortenLine = dxRect / std::cos(angle - PI / 2);
            }
            else
            {
                shortenLine = dyRect / std::cos(PI / 2 - angle);
            }
        }
        else
        {
            if (angle < 2 * PI - boundaryAngle)
            {
                shortenLine = dxRect / std::cos(angle - 3 * PI / 2);
            }
            else
            {
                shortenLine = dyRect / std::cos(2 * PI - angle);
            }
        }

        shortenLine += arrowHeadSize;

        double dX = (getLength() - shortenLine) * sin(angle);
        double dY = (getLength() - shortenLine) * cos(angle);

        Point triangleCentre(static_cast<int>(getBegin().x + dX), static_cast<int>(getBegin().y - dY));

        top += triangleCentre;
        right += triangleCentre;
        left += triangleCentre;

        Point triangle[] = {top, right, left};

        if (isSelected())
        {
            dc.SetPen(wxPen(WXSTRING("RED"), lineWidth, wxPENSTYLE_SOLID));
            dc.SetBrush(wxBrush(wxColour(WXSTRING("RED"))));
        }
        else
        {
            dc.SetPen(wxPen(WXSTRING("BLACK"), lineWidth, wxPENSTYLE_SOLID));
            dc.SetBrush(wxBrush(wxColour(WXSTRING("BLACK"))));
        }
        dc.DrawPolygon(3, triangle);

        // For debugging purposes
        dc.SetPen(wxPen(WXSTRING("ORANGE"), 2, wxPENSTYLE_SOLID));
        dc.SetBrush(wxBrush(wxColour(WXSTRING("ORANGE"))));
        dc.DrawCircle(top, 2);
        dc.SetPen(wxPen(WXSTRING("GREEN"), 2, wxPENSTYLE_SOLID));// stuuRRRRRRboord RRRRRRechts gRRRRRRoen
        dc.SetBrush(wxBrush(wxColour(WXSTRING("GREEN"))));
        dc.DrawCircle(right, 2);
        dc.SetPen(wxPen(WXSTRING("RED"), 2, wxPENSTYLE_SOLID));
        dc.SetBrush(wxBrush(wxColour(WXSTRING("RED"))));
        dc.DrawCircle(left, 2);
    }
    /**
	 *
	 */
    bool LineShape::occupies(const Point& aPoint) const
    {

        Point triangle[] = {top, right, left};

        if (Utils::Shape2DUtils::isInsidePolygon(triangle, 3, aPoint))
        {
            return true;
        }

        bool result = Utils::Shape2DUtils::isOnLine(getBegin(), getEnd(), aPoint, 2);
        if (result == true)
        {
        }
        return result;
    }
    /**
	 *
	 */
    Point LineShape::getCentre() const
    {
        int x = node1->getCentre().x + node2->getCentre().x;
        int y = node1->getCentre().y + node2->getCentre().y;
        return Point(x / 2, y / 2);
    }

    void LineShape::setCentre(const Point& UNUSEDPARAM(aPoint))
    {
    }

    /**
	 *
	 */
    std::string LineShape::getTitle() const
    {
        return title;
    }
    /**
	 *
	 */
    void LineShape::setTitle(const std::string& aTitle)
    {
        title = aTitle;
    }
    /**
	 *
	 */
    int LineShape::getLineWidth() const
    {
        return lineWidth;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    void LineShape::setLineWidth(int aLineWidth)
    {
        lineWidth = aLineWidth;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    int LineShape::getArrowHeadSize() const
    {
        return arrowHeadSize;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    void LineShape::setArrowHeadSize(int anArrowHeadSize)
    {
        arrowHeadSize = anArrowHeadSize;
    }

    /**
	 *
	 */
    Point LineShape::getBegin() const
    {
        return node1->getCentre();
    }

    /**
	 *
	 */
    Point LineShape::getEnd() const
    {
        return node2->getCentre();
    }

    /**
	 *
	 */
    double LineShape::getLength() const
    {
        double dX = node1->getCentre().x - node2->getCentre().x;
        double dY = node1->getCentre().y - node2->getCentre().y;

        return sqrt((dX * dX) + (dY * dY));
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    bool LineShape::connects(RectangleShapePtr aNode) const
    {
        return node1 == aNode || node2 == aNode;
    }
    /**
	 *
	 */
    void LineShape::handleActivated()
    {
    }
    /**
	 *
	 */
    void LineShape::handleSelection()
    {
    }
    /**
	 *
	 */
    std::string LineShape::asString() const
    {
        std::ostringstream os;
        os << Shape::asString() << " LineShape: name = " << title << " " << Utils::Shape2DUtils::asString(getBegin())
           << "->" << Utils::Shape2DUtils::asString(getEnd());
        return os.str();
    }
    /**
	 *
	 */
    std::string LineShape::asDebugString() const
    {
        std::ostringstream os;
        os << "LineShape:\n";
        os << Shape::asDebugString() << "\n";
        os << "name = " << (title.size() == 0 ? "\"\"" : title) << ", " << Utils::Shape2DUtils::asString(getBegin())
           << "->" << Utils::Shape2DUtils::asString(getEnd());
        return os.str();
    }
}// namespace View
