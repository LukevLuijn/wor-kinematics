#include "WayPointShape.hpp"

#include "Logger.hpp"
#include "WayPoint.hpp"

#include <sstream>

namespace View
{
    /**
	 *
	 */
    WayPointShape::WayPointShape(Model::WayPointPtr aWayPoint)
        : RectangleShape(std::dynamic_pointer_cast<Model::ModelObject>(aWayPoint), aWayPoint->getPosition(),
                         aWayPoint->getName())
    {
    }
    /**
	 *
	 */
    WayPointShape::~WayPointShape()
    {
    }
    /**
	 *
	 */
    Model::WayPointPtr WayPointShape::getWayPoint() const
    {
        return std::dynamic_pointer_cast<Model::WayPoint>(getModelObject());
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    void WayPointShape::setWayPoint(Model::WayPointPtr aWayPoint)
    {
        setModelObject(std::dynamic_pointer_cast<Model::ModelObject>(aWayPoint));
    }

    /**
	 *
	 */
    void WayPointShape::handleNotification()
    {
        Application::Logger::log(__PRETTY_FUNCTION__);
    }
    /**
	 *
	 */
    void WayPointShape::draw(wxDC& dc)
    {
        // The minimum size of the RectangleShape is the size of the title
        titleSize = dc.GetTextExtent(WXSTRING(title));
        if (size.x < (titleSize.x + 2 * spacing + 2 * borderWidth))
        {
            size.x = titleSize.x + 2 * spacing + 2 * borderWidth;
        }
        if (size.y < (titleSize.y + 2 * spacing + 2 * borderWidth))
        {
            size.y = titleSize.y + 2 * spacing + 2 * borderWidth;
        }

        if (getWayPoint()->getSize() != size)
        {
            getWayPoint()->setSize(size, false);
        }

        // Draws a rectangle with the given top left corner, and with the given size.
        dc.SetBrush(*wxWHITE_BRUSH);
        if (isSelected())
        {
            dc.SetPen(wxPen(WXSTRING(getSelectionColour()), borderWidth, wxPENSTYLE_SOLID));
        }
        else
        {
            dc.SetPen(wxPen(WXSTRING(getNormalColour()), borderWidth, wxPENSTYLE_SOLID));
        }

        int x = centre.x - (size.x / 2);
        int y = centre.y - (size.y / 2);
        dc.DrawRectangle(x, y, size.x, size.y);

        dc.SetPen(wxPen(WXSTRING("BLACK"), borderWidth, wxPENSTYLE_SOLID));
        dc.DrawText(WXSTRING(title), centre.x - titleSize.x / 2, y + spacing + borderWidth);
    }
    /**
	 *
	 */
    void WayPointShape::setCentre(const Point& aPoint)
    {
        getWayPoint()->setPosition(aPoint, false);
        RectangleShape::setCentre(getWayPoint()->getPosition());
    }
    /**
	 *
	 */
    std::string WayPointShape::asString() const
    {
        std::ostringstream os;

        os << "WayPointShape " << RectangleShape::asString();

        return os.str();
    }
    /**
	 *
	 */
    std::string WayPointShape::asDebugString() const
    {
        std::ostringstream os;

        os << "WayPointShape:\n";
        os << RectangleShape::asDebugString() << "\n";
        if (getWayPoint())
        {
            os << getWayPoint()->asDebugString();
        }

        return os.str();
    }
}// namespace View
