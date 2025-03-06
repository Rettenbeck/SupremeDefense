#include <ECS/include.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    struct PathNode {
        float x, y;
        float cost, heuristic;
        PathNode* parent = nullptr;
        
        float totalCost() const { return cost + heuristic; }
        
        bool operator>(const PathNode& other) const {
            return totalCost() > other.totalCost();
        }

        std::string toStr() {
            std::stringstream ss;
            ss << x << "; " << y << "; cost: " << cost;
            return ss.str();
        }
    };

    using PathNodes = std::vector<PathNode>;
    using NodeMap = std::unordered_map<int, PathNode*>;
    using FloatPair = std::pair<float, float>;
    using FloatPairs = std::vector<FloatPair>;
    
    class PathFinder {
        public:

            std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet;
            NodeMap visited;
        
            PathNodes findPathForUnit(TilesComponent* tilesComponent, float startX, float startY, float goalX, float goalY, float unitRadius) {
                const float MAX_COST = 999999999;
                float tileSize = tilesComponent->tileSize;
                float subStep = tileSize / 2.0f;

                int width  = tilesComponent->tilesX + 1;
                int height = tilesComponent->tilesY + 1;
                int total  = width * height;
                
                auto getKeyFromPos = [&](float x, float y) {
                    int indexX = (x + subStep / 2.0) / subStep;
                    int indexY = (y + subStep / 2.0) / subStep;
                    return width * indexY + indexX;
                };
                
                auto heuristic = [](float x1, float y1, float x2, float y2) {
                    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
                };
                
                auto isPassable = [&](float x, float y) -> bool {
                    Tile* tile = tilesComponent->getFromPos(x, y);
                    return tile && !tile->isImpassable;
                };
                
                auto getTileCost = [&](float x, float y) -> float {
                    Tile* tile = tilesComponent->getFromPos(x, y);
                    return tile ? tile->movementCost : MAX_COST;
                };

                PathNode startNode = {startX, startY, 0.0f, heuristic(startX, startY, goalX, goalY), nullptr};
                openSet.push(startNode);
                visited[getKeyFromPos(startX, startY)] = &startNode;
                
                FloatPairs directions = {
                    {subStep, 0}, {-subStep, 0}, {0, subStep}, {0, -subStep}, 
                    {subStep, subStep}, {subStep, -subStep}, {-subStep, subStep}, {-subStep, -subStep}
                };

                while (!openSet.empty()) {
                    PathNode current = openSet.top();
                    openSet.pop();
                    
                    if (heuristic(current.x, current.y, goalX, goalY) < subStep) {
                        PathNodes path;
                        for (PathNode* n = &current; n; n = n->parent) {
                            path.push_back(*n);
                            if(n == n->parent) break;
                        }
                        std::reverse(path.begin(), path.end());
                        return path;
                    }
                    
                    for (auto& [dx, dy] : directions) {
                        float nx = current.x + dx;
                        float ny = current.y + dy;

                        if (!isPassable(nx, ny)) continue;
                        if (!isPassable(nx + unitRadius, ny)) continue;
                        if (!isPassable(nx - unitRadius, ny)) continue;
                        if (!isPassable(nx, ny + unitRadius)) continue;
                        if (!isPassable(nx, ny - unitRadius)) continue;
                        
                        float newCost = current.cost + getTileCost(nx, ny) * heuristic(current.x, current.y, nx, ny);
                        int key = getKeyFromPos(nx, ny);
                        
                        if (visited.find(key) != visited.end() && visited[key]->cost <= newCost) continue;
                        
                        PathNode* newNode = new PathNode{nx, ny, newCost, heuristic(nx, ny, goalX, goalY), visited[getKeyFromPos(current.x, current.y)]};
                        visited[key] = newNode;
                        openSet.push(*newNode);
                    }
                }
                
                return {};
            }        

    };

    using UPathFinder = std::unique_ptr<PathFinder>;

}
