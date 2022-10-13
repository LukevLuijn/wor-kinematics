#ifndef ASTAR_HPP_
#define ASTAR_HPP_

#include "Config.hpp"

#include "Notifier.hpp"
#include "Point.hpp"
#include "Size.hpp"

#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <vector>


namespace PathAlgorithm
{
    struct Vertex {
        /**
			 *
			 */
        Vertex(int anX, int anY) : x(anX), y(anY), actualCost(0.0), heuristicCost(0.0)
        {
        }
        /**
			 *
			 */
        explicit Vertex(const Point& aPoint) : x(aPoint.x), y(aPoint.y), actualCost(0.0), heuristicCost(0.0)
        {
        }
        /**
			 *
			 */
        Vertex(const Vertex&) = default;
        /**
			 *
			 */
        Vertex(Vertex&&) = default;
        /**
			 *
			 */
        Vertex& operator=(const Vertex&) = default;
        /**
			 *
			 */
        Point asPoint() const
        {
            return Point(x, y);
        }
        /**
			 *
			 */
        bool lessCost(const Vertex& aVertex) const
        {
            if (heuristicCost < aVertex.heuristicCost)
                return true;
            // less uncertainty if the actual cost is smaller
            if (heuristicCost == aVertex.heuristicCost)
                return actualCost > aVertex.actualCost;
            return false;
        }
        /**
			 *
			 */
        bool lessId(const Vertex& aVertex) const
        {
            if (x < aVertex.x)
                return true;
            if (x == aVertex.x)
                return y < aVertex.y;
            return false;
        }
        /**
			 *
			 */
        bool equalPoint(const Vertex& aVertex) const
        {
            return x == aVertex.x && y == aVertex.y;
        }

        int x;
        int y;

        double actualCost;
        double heuristicCost;
    };
    // struct Vertex

    /**
	 *
	 */
    struct VertexLessCostCompare {
        bool operator()(const Vertex& lhs, const Vertex& rhs) const
        {
            return lhs.lessCost(rhs);
        }
    };
    // struct VertexCostCompare
    /**
	 *
	 */
    struct VertexLessIdCompare {
        bool operator()(const Vertex& lhs, const Vertex& rhs) const
        {
            return lhs.lessId(rhs);
        }
    };
    // struct VertexIdCompare
    /**
	 *
	 */
    struct VertexEqualPointCompare {
        bool operator()(const Vertex& lhs, const Vertex& rhs) const
        {
            return lhs.equalPoint(rhs);
        }
    };
    // struct VertexPointCompare
    struct Edge {
        Edge(const Vertex& aVertex1, const Vertex& aVertex2) : vertex1(aVertex1), vertex2(aVertex2)
        {
        }
        Edge(const Edge& anEdge) : vertex1(anEdge.vertex1), vertex2(anEdge.vertex2)
        {
        }

        const Vertex& thisSide(const Vertex& aVertex) const
        {
            if (vertex1.equalPoint(aVertex))
                return vertex1;
            if (vertex2.equalPoint(aVertex))
                return vertex2;
            throw std::logic_error("thisSide: huh???");
        }

        const Vertex& otherSide(const Vertex& aVertex) const
        {
            if (vertex1.equalPoint(aVertex))
                return vertex2;
            if (vertex2.equalPoint(aVertex))
                return vertex1;
            throw std::logic_error("otherSide: huh???");
        }

        Vertex vertex1;
        Vertex vertex2;
    };// struct Edge
    /**
	 *
	 * @param os
	 * @param aVertex
	 * @return
	 */
    inline std::ostream& operator<<(std::ostream& os, const Vertex& aVertex)
    {
        return os << "(" << aVertex.x << "," << aVertex.y << "), " << aVertex.actualCost << " "
                  << aVertex.heuristicCost;
    }
    /**
	 *
	 * @param os
	 * @param anEdge
	 * @return
	 */
    inline std::ostream& operator<<(std::ostream& os, const Edge& anEdge)
    {
        return os << anEdge.vertex1 << " -> " << anEdge.vertex2;
    }
    /*
	 *
	 */
    typedef std::vector<Vertex> Path;
    typedef std::vector<Vertex> OpenSet;
    typedef std::set<Vertex, VertexLessIdCompare> ClosedSet;
    typedef std::map<Vertex, Vertex, VertexLessIdCompare> VertexMap;
    /**
	 *
	 */
    class AStar : public Base::Notifier
    {
    public:
        /**
			 *
			 */
        Path search(const Point& aStartPoint, const Point& aGoalPoint, const Size& aRobotSize);
        /**
			 *
			 */
        Path search(Vertex aStart, const Vertex& aGoal, const Size& aRobotSize);
        /**
			 *
			 */
        void addToOpenSet(const Vertex& aVertex);
        /**
			 *
			 */
        void removeFromOpenSet(const Vertex& aVertex);
        /**
			 *
			 */
        void removeFromOpenSet(OpenSet::iterator& i);
        /**
			 *
			 */
        OpenSet::iterator findInOpenSet(const Vertex& aVertex);
        /**
			 *
			 */
        bool findRemoveInOpenSet(const Vertex& aVertex);
        /**
			 *
			 */
        void removeFirstFromOpenSet();
        /**
			 *
			 */
        void addToClosedSet(const Vertex& aVertex);
        /**
			 *
			 */
        void removeFromClosedSet(const Vertex& aVertex);
        /**
			 *
			 */
        void removeFromClosedSet(ClosedSet::iterator& i);
        /**
			 *
			 */
        ClosedSet::iterator findInClosedSet(const Vertex& aVertex);
        /**
			 *
			 */
        bool findRemoveClosedSet(const Vertex& aVertex);
        /**
			 *
			 */
        ClosedSet getClosedSet() const;
        /**
			 *
			 */
        OpenSet getOpenSet() const;
        /**
			 *
			 */
        VertexMap getPredecessorMap() const;

    protected:
        /**
			 *
			 */
        ClosedSet& getCS();
        /**
			 *
			 */
        const ClosedSet& getCS() const;
        /**
			 *
			 */
        OpenSet& getOS();
        /**
			 *
			 */
        const OpenSet& getOS() const;
        /**
			 *
			 */
        VertexMap& getPM();
        /**
			 *
			 */
        const VertexMap& getPM() const;

    private:
        /**
			 *
			 */
        ClosedSet closedSet;
        /**
			 *
			 */
        OpenSet openSet;
        /**
			 *
			 */
        VertexMap predecessorMap;
    };// class AStar
}// namespace PathAlgorithm
#endif// ASTAR_HPP_
