#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include "Rendering/RenderComponent.h"

#define MAX_COMPS 4
#define TILE_SIZE 16

enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NO_DIR
};

struct Tile
{
    Tile(sf::Uint8 mat = 0, sf::Uint8 state = 0, sf::Uint8 heat = 0, sf::Uint8 force = 0) :
        mMat(mat), mState(state), mHeat(heat), mForce(force) {}

	sf::Uint8 mMat;
	sf::Uint8 mState;

	// status
	sf::Uint8 mComp[MAX_COMPS]; // composit id, quantity
	sf::Uint8 mHeat;
	sf::Uint8 mForce;
};


class GridComponent : public RenderComponent
{
    public:
        GridComponent(int sizeX, int sizeY, Tile** tiles, int tickCount);
        virtual ~GridComponent();

        // Serialization stuff
        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        // Renderable components gotto render...
        void render(sf::RenderTarget& target, sf::RenderStates states);

        sf::Vector2f getTilePos(sf::Transformable* myTrans, sf::Vector2f pos);
        bool checkCollision(sf::Transformable* myTrans, sf::Transformable* trans, sf::Vector2f dim, int dir, float& fix);
        bool dirCollision(int left, int top, int right, int bot, int dir, int& fix);
        void setTile(int x, int y, Tile tile, int tick);
        void calcNeighborState(int x, int y);
        int wrapX(int x);

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new GridComponent(0, 0, NULL, 0);}

        static void addTileSheet(int mat, sf::Texture* sheet)
        {
            if (mat >= static_cast<int>(TileSheets.size()))
                TileSheets.resize(mat+1);
            TileSheets[mat] = sheet;
        }

    private:
        int mID;
        int mSizeX;
        int mSizeY;
        Tile** mTiles; // 2D array of tiles
        int mTickCount; // The number of tick types
        std::vector<std::vector<sf::Vector2i>> mCTiles; // Cached interesting tile coordinates
        std::vector<std::vector<sf::Vector2i>> mBuses; // Array of buses

        static std::vector<sf::Texture*> TileSheets;
};

#endif // GRIDCOMPONENT_H
