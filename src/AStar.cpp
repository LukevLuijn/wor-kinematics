#include "AStar.hpp"

#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace PathAlgorithm
{
    /**
	 *
	 */
    double ActualCost(const Vertex& aStart, const Vertex& aGoal)
    {
        return std::sqrt((aStart.x - aGoal.x) * (aStart.x - aGoal.x) + (aStart.y - aGoal.y) * (aStart.y - aGoal.y));
    }
    /**
	 *
	 */
    double HeuristicCost(const Vertex& aStart, const Vertex& aGoal)
    {
        return std::sqrt((aStart.x - aGoal.x) * (aStart.x - aGoal.x) + (aStart.y - aGoal.y) * (aStart.y - aGoal.y));
    }
    /**
	 *
	 */
    Path ConstructPath(VertexMap& aPredecessorMap, const Vertex& aCurrentNode)
    {
        VertexMap::iterator i = aPredecessorMap.find(aCurrentNode);
        if (i != aPredecessorMap.end())
        {
            Path path = ConstructPath(aPredecessorMap, (*i).second);
            path.push_back(aCurrentNode);
            return path;
        }
        else
        {
            Path path;
            path.push_back(aCurrentNode);
            return path;
        }
    }
    /**
	 *
	 */
    std::vector<Vertex> GetNeighbours(const Vertex& aVertex, int aFreeRadius /*= 1*/)
    {
        static const int xOffset[] = {0, 1, 1, 1, 0, -1, -1, -1};
        static const int yOffset[] = {1, 1, 0, -1, -1, -1, 0, 1};

        const std::vector<Model::WallPtr>& walls = Model::RobotWorld::getRobotWorld().getWalls();
        std::vector<Vertex> neighbours;

        for (int i = 0; i < 8; ++i)
        {
            bool addToNeigbours = true;

            Vertex vertex(aVertex.x + xOffset[i], aVertex.y + yOffset[i]);
            for (Model::WallPtr wall : walls)
            {
                if (Utils::Shape2DUtils::isOnLine(wall->getPoint1(), wall->getPoint2(), vertex.asPoint(), aFreeRadius))
                {
                    addToNeigbours = false;
                    break;
                }
            }
            if (addToNeigbours == true)
            {
                neighbours.push_back(vertex);
            }
        }
        return neighbours;
    }
    /**
	 *
	 */
    std::vector<Edge> GetNeighbourConnections(const Vertex& aVertex, int aFreeRadius /*= 1*/)
    {
        std::vector<Edge> connections;

        const std::vector<Vertex>& neighbours = GetNeighbours(aVertex, aFreeRadius);
        for (const Vertex& vertex : neighbours)
        {
            // cppcheck-suppress useStlAlgorithm
            connections.push_back(Edge(aVertex, vertex));
        }

        return connections;
    }
    /**
	 *
	 */
    Path AStar::search(const Point& aStartPoint, const Point& aGoalPoint, const Size& aRobotSize)
    {
        Vertex start(aStartPoint);
        Vertex goal(aGoalPoint);

        Path path = AStar::search(start, goal, aRobotSize);
        return path;
    }
    /**
	 *
	 */
    Path AStar::search(Vertex aStart, const Vertex& aGoal, const Size& aRobotSize)
    {
        getOS().clear();
        getCS().clear();
        getPM().clear();

        int radius = static_cast<int>(std::ceil(
                std::sqrt((aRobotSize.x / 2.0) * (aRobotSize.x / 2.0) + (aRobotSize.y / 2.0) * (aRobotSize.y / 2.0))));

        aStart.actualCost = 0.0;// Cost from aStart along the best known path.
        aStart.heuristicCost =
                aStart.actualCost + HeuristicCost(aStart, aGoal);// Estimated total cost from aStart to aGoal through y.

        addToOpenSet(aStart);

        // Keep the timing stuff, please.
        //		clock_t start = std::clock();
        while (!openSet.empty())
        {
            // The openSet should be sorted by cost, least cost must be the first
            Vertex current = *openSet.begin();

            if (current.equalPoint(aGoal))
            {
                //				clock_t end = std::clock();
                //				std::cout << "Duration: " << static_cast<double>(end - start)/CLOCKS_PER_SEC <<	", openSet: " << openSet.size() <<  ", closedSet: " << closedSet.size() << ", predecessorMap: " << predecessorMap.size() << std::endl;
                return ConstructPath(predecessorMap, current);
            }
            else
            {
                removeFirstFromOpenSet();
                addToClosedSet(current);

                // Find all the outgoing connections for the current Vertex
                const std::vector<Edge>& connections = GetNeighbourConnections(current, radius);

                for (const Edge& connection : connections)
                {
                    Vertex neighbour = connection.otherSide(current);

                    // Calculate the cost for the newly found neighbour
                    neighbour.actualCost = current.actualCost + ActualCost(current, neighbour);
                    neighbour.heuristicCost = neighbour.actualCost + HeuristicCost(neighbour, aGoal);

                    // The neighbour may already be in the openSet because of the previous current Vertex iteration
                    OpenSet::iterator openVertex = findInOpenSet(neighbour);
                    if (openVertex != openSet.end())
                    {
                        // if neighbour is in the openSet we may have found a shorter via-route
                        // than via the previous current Vertex
                        if ((*openVertex).heuristicCost <= neighbour.heuristicCost)
                        {
                            // Do nothing
                            continue;
                        }
                        else
                        {
                            // Update the cost
                            // TODO: should the actual cost be adjusted if (*openVertex).heuristicCost < neighbour.actualCost?
                            //(*openVertex).actualCost = neighbour.actualCost;
                            (*openVertex).heuristicCost = neighbour.heuristicCost;
                            continue;
                        }
                    }

                    // The neighbour may be re-opened because we found a shorter via-route
                    ClosedSet::iterator closedVertex = findInClosedSet(neighbour);
                    if (closedVertex != closedSet.end())
                    {
                        // if neighbour is in the closedSet we may have found a shorter via-route
                        if ((*closedVertex).heuristicCost <= neighbour.heuristicCost)
                        {
                            // Do nothing
                            continue;
                        }
                        else
                        {
                            std::cout << "remove" << std::endl;
                            addToOpenSet(*closedVertex);
                            removeFromClosedSet(closedVertex);
                        }
                    }

                    // Add the new found neighbour to the openSet
                    addToOpenSet(neighbour);

                    // Add or replace (assign) the route elements.
                    // cppcheck-suppress unusedVariable
                    const auto& [iterator, succes] = predecessorMap.insert_or_assign(neighbour, current);
                    if (!succes)
                    {
                        std::cerr << "**** Failed adding neighbour to the predecessorMap: " << neighbour << std::endl;
                    }

                }//for(Edge connection : connections)

                //			28-04-2014
                //
                //			Sorting after the insertion of an individual:
                //			aRobotSize = (37,29), radius = 23
                //			Duration: 17.349.490 openSet: 1507 closedSet: 76521 predecessorMap: 78027
                //
                //			After the insert of all:
                //			aRobotSize = (37,29), radius = 23
                //			Duration: 7.733.159 openSet: 1507 closedSet: 76521 predecessorMap: 78027
                //
                //			Partial sort after insert of all:
                //			aRobotSize = (37,29), radius = 23
                //			Duration: 3.297.669 openSet: 1507 closedSet: 76521 predecessorMap: 78027
                //
                //			No sorting, just iterator swap for begin() and the minimum element, which is basically
                //			the smallest partial_sort there is...
                //			aRobotSize = (37,29), radius = 23
                //			Duration: 2.626.220 openSet: 1507 closedSet: 76521 predecessorMap: 78027
                //
                //			// Sort the openSet before the next loop, use partial_sort as per Scott Meyers' Effective STL
                //			if(std::distance(openSet.begin(), openSet.end()) > 5 )
                //			{
                //				std::partial_sort( openSet.begin(), openSet.begin() + 4, openSet.end(), VertexLessCostCompare());
                //			}else
                //			{
                //				std::sort( openSet.begin(), openSet.end(), VertexLessCostCompare());
                //			}
                //
                //			 28-1-2022:
                //
                //			 Some rationalisations (see a diff for the differences) and the size of the window has changed.
                //			 Don't know if that should have changed the number of nodes though.
                //			 Timings are without enabling notyfyObservers but with checking if the are enabled. Removing the check
                //			 did not change the times significantly.
                //			 The numbers have changed but the essence stays the same: using a vector and searching/swapping
                //			 is faster than using a (always sorted) set:
                //			   With profiling information:
                //			   	Duration: 0.487936, openSet: 1252, closedSet: 83731, predecessorMap: 84982
                //			   Without profiling information:
                //			   	Duration: 0.294032, openSet: 1252, closedSet: 83731, predecessorMap: 84982
                std::iter_swap(openSet.begin(),
                               std::min_element(openSet.begin(), openSet.end(), VertexLessCostCompare()));
            }
        }

        std::cerr << "**** No route from " << aStart << " to " << aGoal << std::endl;
        return Path();
    }
    /**
	 *
	 */
    void AStar::addToOpenSet(const Vertex& aVertex)
    {
        openSet.push_back(aVertex);
        notifyObservers();
    }
    /**
	 *
	 */
    void AStar::removeFromOpenSet(const Vertex& aVertex)
    {
        OpenSet::iterator i = findInOpenSet(aVertex);
        removeFromOpenSet(i);
    }
    /**
	 *
	 */
    void AStar::removeFromOpenSet(OpenSet::iterator& i)
    {
        openSet.erase(i);
        notifyObservers();
    }
    /**
	 *
	 */
    OpenSet::iterator AStar::findInOpenSet(const Vertex& aVertex)
    {
        return std::find_if(openSet.begin(), openSet.end(), [aVertex](const Vertex& rhs) {
            return aVertex.equalPoint(rhs);
        });
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    bool AStar::findRemoveInOpenSet(const Vertex& aVertex)
    {
        OpenSet::iterator i = findInOpenSet(aVertex);
        if (i != openSet.end())
        {
            openSet.erase(i);
            return true;
        }
        return false;
    }
    /**
	 *
	 */
    void AStar::removeFirstFromOpenSet()
    {
        openSet.erase(openSet.begin());
    }
    /**
	 *
	 */
    void AStar::addToClosedSet(const Vertex& aVertex)
    {
        closedSet.insert(aVertex);
        notifyObservers();
    }
    /**
	 *
	 */
    void AStar::removeFromClosedSet(const Vertex& aVertex)
    {
        ClosedSet::iterator i = findInClosedSet(aVertex);
        removeFromClosedSet(i);
    }
    /**
	 *
	 */
    void AStar::removeFromClosedSet(ClosedSet::iterator& i)
    {
        closedSet.erase(i);
        notifyObservers();
    }
    /**
	 *
	 */
    ClosedSet::iterator AStar::findInClosedSet(const Vertex& aVertex)
    {
        return closedSet.find(aVertex);
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    ClosedSet AStar::getClosedSet() const
    {
        return closedSet;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    bool AStar::findRemoveClosedSet(const Vertex& aVertex)
    {
        ClosedSet::iterator i = findInClosedSet(aVertex);
        if (i != closedSet.end())
        {
            closedSet.erase(i);
            return true;
        }
        return false;
    }
    /**
	 *
	 */
    OpenSet AStar::getOpenSet() const
    {
        return openSet;
    }
    /**
	 *
	 */
    // cppcheck-suppress unusedFunction
    VertexMap AStar::getPredecessorMap() const
    {
        return predecessorMap;
    }

    /**
	 *
	 */
    ClosedSet& AStar::getCS()
    {
        return closedSet;
    }
    /**
	 *
	 */
    const ClosedSet& AStar::getCS() const
    {
        return closedSet;
    }
    /**
	 *
	 */
    OpenSet& AStar::getOS()
    {
        return openSet;
    }
    /**
	 *
	 */
    const OpenSet& AStar::getOS() const
    {
        return openSet;
    }
    /**
	 *
	 */
    VertexMap& AStar::getPM()
    {
        return predecessorMap;
    }
    /**
	 *
	 */
    const VertexMap& AStar::getPM() const
    {
        return predecessorMap;
    }
}// namespace PathAlgorithm
