#pragma once

#include <ECS/component_registry.hpp>
#include <ECS/Tiles/tile.hpp>


namespace SupDef {

    DEFINE_COMPONENT_NOJSON_BEGIN(TilesComponent, SC_TILES)
        int tileSize = 16;
        int tilesX = 0, tilesY = 0;
        Tiles tiles;

        void buildTiles(int width, int height) {
            tiles.clear();
            tilesX = width  / tileSize;
            tilesY = height / tileSize;
            if(width  % tileSize > 0) tilesX++;
            if(height % tileSize > 0) tilesY++;

            for(int y = 0; y < tilesY; y++) {
                for(int x = 0; x < tilesX; x++) {
                    tiles.push_back(std::make_unique<Tile>());
                }
            }
        }

        TilesComponent(int tileSize_, int width, int height) : tileSize(tileSize_) {
            addToRegistry();
            buildTiles(width, height);
        }

        TilesComponent(int tileSize_) : tileSize(tileSize_) { addToRegistry(); }

        void setTilesToDefault(Ints tileIndeces) {
            for(auto t : tileIndeces) {
                assert(t >= 0 && t < tiles.size());
                tiles[t]->setToDefault();
            }
        }

        void setTilesToDefault() {
            for(auto& tile : tiles) {
                tile->setToDefault();
            }
        }

        inline int getIndexFromXY(int x, int y) {
            return tilesX * y + x;
        }

        inline int getIndexFromPos(float x, float y) {
            if(x < 0.0 || y < 0.0) return -1;
            int indexX = x / tileSize;
            int indexY = y / tileSize;
            return getIndexFromXY(indexX, indexY);
        }

        inline V2 getXYFromIndex(int index) {
            return V2(index % tilesX, index / tilesX);
        }

        inline VF2 getPosFromIndex(int index) {
            auto xy = getXYFromIndex(index);
            return VF2(xy.x * tileSize, xy.y * tileSize);
        }

        inline Tile* getFromXY(int x, int y) {
            int index = getIndexFromXY(x, y);
            if(index < 0 || index >= tiles.size()) return nullptr;
            return tiles[index].get();
        }

        inline Tile* getFromPos(float x, float y) {
            int index = getIndexFromPos(x, y);
            if(index < 0 || index >= tiles.size()) return nullptr;
            return tiles[index].get();
        }
        
        void rebuildFromString(std::string input) {
            tiles.clear();
            std::stringstream ss(input);
            std::string segment;
        
            while (std::getline(ss, segment, TILE_SEPARATOR[0])) {
                if (segment.empty()) continue;
                assert(segment.size() >= 2);
        
                char first = segment[0];
                char second = segment[1];
                std::string rest = segment.substr(2);
                float cost = TILE_INITIAL_MOVEMENT_COST;
                if (rest.size() > 1) {
                    char third = rest[0];
                    std::string value = rest.substr(1);
                    if (third == TILE_ATTRIBUTE_COST[0]) {
                        cost = std::stof(value);
                    }
                }
                tiles.push_back(std::make_unique<Tile>(STR_TO_BOOL(first), STR_TO_BOOL(second), cost));
            }
        }

        void to_json(json& j) const override {
            j[S_TILE_SIZE] = tileSize;
            j[S_TILES_X] = tilesX;
            j[S_TILES_Y] = tilesY;
            std::stringstream ss;
            for (auto& tile : tiles) {
                ss << BOOL_TO_INT(tile->isImpassable);
                ss << BOOL_TO_INT(tile->isOccupied);
                if (tile->movementCost != TILE_INITIAL_MOVEMENT_COST) {
                    ss << TILE_ATTRIBUTE_COST << tile->movementCost << TILE_ATTRIBUTE_SEPARATOR;
                }
                ss << TILE_SEPARATOR;
            }
            j[S_TILES] = ss.str();
        }
        
        void from_json(const json& j) override {
            tileSize = j.at(S_TILE_SIZE).get<int>();
            tilesX = j.at(S_TILES_X).get<int>();
            tilesY = j.at(S_TILES_Y).get<int>();
            rebuildFromString(j.at(S_TILES).get<std::string>());
        }

        REFLECT_BEGIN
            REFLECT_FIELD(tileSize)
            REFLECT_FIELD(tilesX)
            REFLECT_FIELD(tilesY)
            REFLECT_LIST_UNIQUE(tiles, Tile)
        REFLECT_END
    DEFINE_COMPONENT_END

}
