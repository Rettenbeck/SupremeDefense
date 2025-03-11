#include <ECS/include.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    struct PathNode {
        float x, y;
        float cost, heuristic;
        std::shared_ptr<PathNode> parent = nullptr;
        
        PathNode() {}
        PathNode(float x, float y, float cost, float heuristic) : x(x), y(y), cost(cost), heuristic(heuristic) {}
        PathNode(float x, float y, float cost, float heuristic, std::shared_ptr<PathNode> parent)
        : x(x), y(y), cost(cost), heuristic(heuristic), parent(parent) {}

        float totalCost() const { return cost + heuristic; }
        bool operator>(const PathNode* other) const {
            assert(other);
            return totalCost() > other->totalCost();
        }

        std::string toStr() {
            std::stringstream ss;
            ss << x << "; " << y << "; cost: " << cost;
            return ss.str();
        }
    };

    using SPathNode = std::shared_ptr<PathNode>;
    using PathNodes = std::vector<SPathNode>;
    // using PPathNodes = std::vector<PathNode*>;
    using NodeMap = std::unordered_map<int, SPathNode>;
    using FloatPair = std::pair<float, float>;
    using FloatPairs = std::vector<FloatPair>;
    
    struct ComparePathNode {
        bool operator()(const SPathNode& a, const SPathNode& b) const {
            return a->totalCost() > b->totalCost();
        }
    };

    struct PathResult {
        PathNodes path;
        bool done  = false;
        bool found = false;
        bool stuck = false;
    };

    using UPathResult = std::unique_ptr<PathResult>;
    using PathQueue = std::priority_queue<SPathNode, PathNodes, ComparePathNode>;

    class PathFinder {
        public:
            PathQueue openSet;
            NodeMap visited;
        
            UPathResult findPathForUnit(TilesComponent* tilesComponent, float startX,
                float startY, float goalX, float goalY, float unitRadius) {
                //
                openSet = PathQueue();
                visited.clear();
                auto result = std::make_unique<PathResult>();
                const float MAX_COST = 999999999;
                float tileSize = tilesComponent->tileSize;
                float subStep = tileSize / 2.0f;

                int width  = tilesComponent->tilesX * 2 + 1;
                int height = tilesComponent->tilesY * 2 + 1;
                int total  = width * height;
                
                auto getCoordKeyFromPos = [&](float c) {
                    return int((c + subStep / 2.0) / subStep);
                };

                auto getKeyFromPos = [&](float x, float y) {
                    return width * getCoordKeyFromPos(y) + getCoordKeyFromPos(x);
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
                    assert(tile);
                    return tile ? tile->movementCost : MAX_COST;
                };

                startX = getCoordKeyFromPos(startX) * subStep;
                startY = getCoordKeyFromPos(startY) * subStep;

                auto startNode = std::make_shared<PathNode>(
                    startX, startY, 0.0f, heuristic(startX, startY, goalX, goalY), nullptr
                );

                visited[getKeyFromPos(startX, startY)] = startNode;
                // std::cout << "Startnode: " << startNode.get() << "; parent: " << startNode->parent <<
                // "; key: " << getKeyFromPos(startX, startY) << "\n";
                openSet.push(startNode);
                
                FloatPairs directions = {
                    {subStep, 0}, {-subStep, 0}, {0, subStep}, {0, -subStep}, 
                    {subStep, subStep}, {subStep, -subStep}, {-subStep, subStep}, {-subStep, -subStep}
                };

                while (!openSet.empty()) {
                    auto current = openSet.top();
                    openSet.pop();
                    assert(current);
                    
                    if (heuristic(current->x, current->y, goalX, goalY) < subStep) {
                        result->found = true;
                        // std::cout << "Goal found ";
                        // std::cout << current.get() << "\n";
                        for (auto n = current; n; n = n->parent) {
                            // std::cout << "n: " << n << "\n";
                            // std::cout << "  parent: " << n->parent << "\n";
                            assert(n);
                            result->path.push_back(n);
                            if (!n->parent) break;
                            if (n.get() == startNode.get()) break;
                            if (n.get() == n->parent.get()) break;
                        }
                        // std::cout << "Return list created\n";
                        std::reverse(result->path.begin(), result->path.end());
                        return std::move(result);
                    }
                    
                    for (auto& [dx, dy] : directions) {
                        float nx = current->x + dx;
                        float ny = current->y + dy;

                        if (!isPassable(nx, ny)) continue;
                        if (!isPassable(nx + unitRadius, ny)) continue;
                        if (!isPassable(nx - unitRadius, ny)) continue;
                        if (!isPassable(nx, ny + unitRadius)) continue;
                        if (!isPassable(nx, ny - unitRadius)) continue;
                        
                        float newCost = current->cost + getTileCost(nx, ny) * heuristic(current->x, current->y, nx, ny);
                        int key = getKeyFromPos(nx, ny);
                        
                        if (visited.find(key) != visited.end() && visited[key]->cost <= newCost) continue;
                        auto newNode = std::make_shared<PathNode>(
                            nx, ny, newCost, heuristic(nx, ny, goalX, goalY), visited[getKeyFromPos(current->x, current->y)]
                        );
                        // std::cout << "newNode: " << newNode.get() << "; parent: " << newNode->parent.get()
                        // << "; key: " << getKeyFromPos(current->x, current->y) << newNode->toStr() << "\n";

                        visited[key] = newNode;

                        // std::cout << "visited:\n";
                        // for(auto& [key, node] : visited) {
                        //     std::cout << "  Key: " << key << "; node: " << node << "\n";
                        // }

                        openSet.push(std::move(newNode));
                    }
                }
                
                return std::move(result);
            }        

    };

    using UPathFinder = std::unique_ptr<PathFinder>;

}
