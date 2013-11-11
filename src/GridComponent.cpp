#include "GridComponent.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

int randStateCovered();
int randStateTop();
int randStateBot();
int randStateRight();
int randStateLeft();

TypeBits GridComponent::Type;
std::vector<sf::Texture*> GridComponent::TileSheets;

GridComponent::GridComponent(int sizeX, int sizeY, Tile** tiles, int tickCount) : mSizeX(sizeX), mSizeY(sizeY),
    mTiles(tiles), mTickCount(tickCount)
{
	for (int y = 0; y < mSizeY; y++)
		for (int x = 0; x < mSizeX; x++)
			calcNeighborState(x, y);

    mCTiles.resize(tickCount);
}

GridComponent::~GridComponent()
{
    //dtor
}

void GridComponent::serialize(sf::Packet &packet)
{
}

void GridComponent::deserialize(sf::Packet &packet)
{
}

void GridComponent::render(sf::RenderTarget& target, sf::RenderStates states)
{
    float tsize = static_cast<float>(TILE_SIZE);

	sf::Vector2f center = states.transform.transformPoint(sf::Vector2f());
	center -= target.getView().getCenter();
	center += sf::Vector2f(target.getSize().x, target.getSize().y)/2.f;
	center.x *= -1;
	center.y *= -1;

	sf::Vector2f centerT = center/tsize;                // Center tiled
	float ssXT = ceil(target.getSize().x / tsize); // Screen size X in tiles
	float ssYT = ceil(target.getSize().y / tsize); // Screen size Y in tiles

	int left = centerT.x - 1;
	int top = std::max<int>(centerT.y-1, 0);
	int right = centerT.x + ssXT + 1;
	int bot = std::min<int>(centerT.y+ssYT+1, mSizeY);

    sf::VertexArray verts(sf::Quads, 4);
	for (int _y = top; _y <= bot; _y++)
	{
		for (int _x = left; _x <= right; _x++)
        {
			int x = wrapX(_x);
			int y = _y;

			auto start = sf::Vector2f(tsize * static_cast<float>(_x), tsize * static_cast<float>(_y)); // Tile start draw
			verts[0] = sf::Vertex(start,
				sf::Color(255, 255, 255, 255),
				sf::Vector2f());
			verts[1] = sf::Vertex(start+sf::Vector2f(0, tsize),
				sf::Color(255, 255, 255, 255),
				sf::Vector2f(0, tsize));
			verts[2] = sf::Vertex(start+sf::Vector2f(tsize, tsize),
				sf::Color(255, 255, 255, 255),
				sf::Vector2f(tsize, tsize));
			verts[3] = sf::Vertex(start+sf::Vector2f(tsize, 0),
				sf::Color(255, 255, 255, 255),
				sf::Vector2f(tsize, 0));

            if (mTiles[y][x].mMat == 4)
            {
                states.texture = NULL;
                verts[0].color = sf::Color(255, 0, 0, mTiles[y][x].mFluid);
                verts[1].color = sf::Color(255, 0, 0, mTiles[y][x].mFluid);
                verts[2].color = sf::Color(255, 0, 0, mTiles[y][x].mFluid);
                verts[3].color = sf::Color(255, 0, 0, mTiles[y][x].mFluid);
                target.draw(verts, states);
            }
            else if (mTiles[y][x].mWire == 1)
            {
                states.texture = NULL;
                verts[0].color = sf::Color(0, 0, 255, 255);
                verts[1].color = sf::Color(0, 0, 255, 255);
                verts[2].color = sf::Color(0, 0, 255, 255);
                verts[3].color = sf::Color(0, 0, 255, 255);
                target.draw(verts, states);
            }
            else if (mTiles[y][x].mWire == 2)
            {
                states.texture = NULL;
                verts[0].color = sf::Color(0, 255, 0, 255);
                verts[1].color = sf::Color(0, 255, 0, 255);
                verts[2].color = sf::Color(0, 255, 0, 255);
                verts[3].color = sf::Color(0, 255, 0, 255);
                target.draw(verts, states);
            }
            else
            {
                if (mTiles[y][x].mMat == 0 || mTiles[y][x].mMat >= TileSheets.size())
                    continue;

                // Grab tile sheet info
                sf::Texture* sheet = TileSheets[mTiles[y][x].mMat];
                if (!sheet)
                    continue;

                int sheetSizeX = sheet->getSize().x / TILE_SIZE;
                int sheetSizeY = sheet->getSize().y / TILE_SIZE;

                float texStartX = float(int(mTiles[y][x].mState)%sheetSizeX) * tsize;
                float texStartY = float(int(mTiles[y][x].mState)/sheetSizeY) * tsize;

                verts[0].texCoords.x += texStartX;
                verts[0].texCoords.y += texStartY;
                verts[1].texCoords.x += texStartX;
                verts[1].texCoords.y += texStartY;
                verts[2].texCoords.x += texStartX;
                verts[2].texCoords.y += texStartY;
                verts[3].texCoords.x += texStartX;
                verts[3].texCoords.y += texStartY;

                states.texture = sheet;
                target.draw(verts, states);
            }

			/*if joelMode {
				for i, c := range mTiles[y][x].comp {
					bheight := (float32(c) / 255 * 16)
					bwidth := float32(TILE_SIZE / MaxComps)
					bstart := start.Add(sf::Vector2f{float32(i) * bwidth, 16 - bheight})

					color := sf::Color{}
					color.A = 255

					switch i + int(mTiles[y][x].mMat) {
					case 0:
						color.R = 255
					case 1:
						color.G = 255
					case 2:
						color.B = 255
					case 3:
						color.R = 255
						color.G = 255
					case 4:
						color.R = 255
						color.B = 255
					case 5:
						color.R = 255
						color.G = 255
						color.B = 255
					case 6:
						color.G = 255
						color.B = 255
					case 7:
						color.R = 255
						color.G = 255
						color.B = 255
					}

					verts[0] = sf::Vertex{bstart,
						color,
						sf::Vector2f{}}
					verts[1] = sf::Vertex{bstart.Add(sf::Vector2f{0, bheight}),
						color,
						sf::Vector2f{}}
					verts[2] = sf::Vertex{bstart.Add(sf::Vector2f{bwidth, bheight}),
						color,
						sf::Vector2f{}}
					verts[3] = sf::Vertex{bstart.Add(sf::Vector2f{bwidth, 0}),
						color,
						sf::Vector2f{}}

					states.Texture = nil
					rt.Render(verts[:], sf::Quads, states)
				}
			}*/
		}
	}
}

bool GridComponent::checkCollision(sf::Transformable* myTrans, sf::Transformable* trans, sf::Vector2f dim, int dir, float& fix)
{
	sf::Transform myInv = myTrans->getTransform().getInverse(); // Inverse of grid's transform
	sf::Vector2f cOffset = myInv.transformPoint(trans->getPosition());
	sf::Vector2f offset = cOffset-(dim/2.f);
	sf::Vector2f tOffset = offset/float(TILE_SIZE);

	int x = floor(tOffset.x);
	int y = floor(tOffset.y);
	int right = ceil((offset.x+dim.x)/float(TILE_SIZE)) - 1;
	int bot = ceil((offset.y+dim.y)/float(TILE_SIZE)) - 1;

	// Let's be safe about this
	if (y >= int(mSizeY) || bot < 0)
        return 0;

	if (y < 0)
		y = 0;
	if (bot >= int(mSizeY))
		bot = int(mSizeY) - 1;

	// Test the collision
	int tfix = 0;
	bool col = dirCollision(x, y, right, bot, dir, tfix);

	if (col)
	{
		// Calculate remainder pixels depending on direction
		switch (dir)
        {
		case LEFT:
			fix = float(TILE_SIZE) - fmod(offset.x, TILE_SIZE);
			break;
		case RIGHT:
			fix = float(tfix*TILE_SIZE) - fmod(offset.x+dim.x, TILE_SIZE);
			break;
		case UP:
			fix = float(TILE_SIZE) - fmod(offset.y, TILE_SIZE);
			break;
		case DOWN:
			fix = float(tfix*TILE_SIZE) - fmod(offset.y+dim.y, TILE_SIZE);
			break;
		}
		if (offset.x <= 0 && offset.x > -dim.x && dir == RIGHT)
        {
		}
		else if (offset.x <= 0 && (dir == LEFT || dir == RIGHT))
			fix -= float(TILE_SIZE);
	}

	if (cOffset.x-(dim.x/2) <= 0 || cOffset.x+(dim.x/2) >= mSizeX*TILE_SIZE)
    {
		cOffset.x = fmod(cOffset.x, mSizeX*TILE_SIZE);
		sf::Transform t = myTrans->getTransform();
		trans->setPosition(t.transformPoint(cOffset));
	}

	return col;
}

bool GridComponent::dirCollision(int left, int top, int right, int bot, int dir, int& fix)
{
	for (int _y = top; _y <= bot; _y++)
	{
		for (int _x = left; _x <= right; _x++)
        {
			int x = wrapX(_x);
			int y = _y;

			if (mTiles[y][x].mMat != 0 && mTiles[y][x].mMat != 4)
            {
				switch (dir)
				{
				case RIGHT:
					fix = -(right - _x);
					break;
				case DOWN:
					fix = -(bot - _y);
					break;
				}
				return true;
			}
		}
	}
	return false;
}

sf::Vector2f GridComponent::getTilePos(sf::Transformable* myTrans, sf::Vector2f pos)
{
	sf::Transform myInv = myTrans->getTransform().getInverse();
	pos = myInv.transformPoint(pos);
	pos = pos/float(TILE_SIZE);
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);
	return pos;
}

void GridComponent::setTile(int x, int y, Tile tile, int tick)
{
	if (y < 0 || y >= mSizeY)
	{
		return;
	}

	x = wrapX(x);

	if (mTiles[y][x].mMat == tile.mMat && mTiles[y][x].mFluid == tile.mFluid &&
        mTiles[y][x].mWire == tile.mWire && mTiles[y][x].mSignal == tile.mSignal)
	{
		return;
	}

	//std::cout << int(mTiles[y][x].mMat) << std::endl;

	mTiles[y][x] = tile;

	int left = wrapX(x - 1);
	int right = wrapX(x + 1);
	int top = y-1;
	int bot = y+1;

	if (top < 0)
		top = 0;
	else if (bot > mSizeY-1)
		bot = mSizeY-1;


	// If tick is -1, make it interesting for all ticks
	int tickMin = 0;
	int tickMax = mTickCount;
	if (tick != -1)
    {
		tickMin = tick;
		tickMax = tick+1;
	}

	if (y > 0)
    {
        calcNeighborState(left, y-1);
        calcNeighborState(x, y-1);
        calcNeighborState(right, y-1);
    }

    calcNeighborState(left, y);
	calcNeighborState(x, y);
	calcNeighborState(right, y);

	if (y < mSizeY-1)
    {
        calcNeighborState(left, y+1);
        calcNeighborState(x, y+1);
        calcNeighborState(right, y+1);
    }

	for (int t = tickMin; t < tickMax; t++)
    {
		//for (int i = top; i <= bot; i++)
		//{
		    if (y > 0)
                addInterestingTile(x, y-1, t);
            addInterestingTile(left, y, t);
            addInterestingTile(x, y, t);
            addInterestingTile(right, y, t);
            if (y < mSizeY-1)
                addInterestingTile(x, y+1, t);

		    /*if (std::find(mCTiles[t].begin(), mCTiles[t].end(), sf::Vector2i(left, i)) == mCTiles[t].end())
                mCTiles[t].push_back(sf::Vector2i(left, i));
            if (std::find(mCTiles[t].begin(), mCTiles[t].end(), sf::Vector2i(x, i)) == mCTiles[t].end())
                mCTiles[t].push_back(sf::Vector2i(x, i));
            if (std::find(mCTiles[t].begin(), mCTiles[t].end(), sf::Vector2i(right, i)) == mCTiles[t].end())
                mCTiles[t].push_back(sf::Vector2i(right, i));*/
		//}
	}
}

void GridComponent::calcNeighborState(int x, int y)
{
    x = wrapX(x);
	int left = wrapX(x - 1);
	int right = wrapX(x + 1);

    if (mTiles[y][x].mMat == 0 || mTiles[y][x].mMat == 4)
        return;

	Tile a[3][3]; // Area

	if (y > 0)
    {
		a[0][0] = mTiles[y-1][left];
		a[0][1] = mTiles[y-1][x];
		a[0][2] = mTiles[y-1][right];
	}

	a[1][0] = mTiles[y][left];
	a[1][1] = mTiles[y][x];
	a[1][2] = mTiles[y][right];

	if (y < mSizeY-1)
    {
		a[2][0] = mTiles[y+1][left];
		a[2][1] = mTiles[y+1][x];
		a[2][2] = mTiles[y+1][right];
	}

	if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = randStateCovered();
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 21;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = randStateTop();
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = randStateBot();
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = randStateLeft();
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = randStateRight();
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 0;
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = 1;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = 17;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = 18;
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 19;
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 20;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat == a[1][1].mMat)
		mTiles[y][x].mState = 2;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat == a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 3;
	else if (a[0][1].mMat == a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat != a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 4;
	else if (a[0][1].mMat != a[1][1].mMat &&
		a[1][0].mMat != a[1][1].mMat && a[1][2].mMat == a[1][1].mMat &&
		a[2][1].mMat != a[1][1].mMat)
		mTiles[y][x].mState = 5;
}

int GridComponent::wrapX(int x)
{
    if (x < 0)
    {
		x = x % mSizeX;
		if (x < 0)
			x += mSizeX;
	}
	else if (x >= mSizeX)
		x = x % mSizeX;

	return x;
}

Area GridComponent::getArea(int x, int y)
{
    int left = wrapX(x - 1);
    int right = wrapX(x + 1);

    // Tiles
    Area a;
    a.mX = x;
    a.mY = y;
    if (y > 0)
    {
        a.mTiles[0][0] = mTiles[y-1][left];
        a.mTiles[0][1] = mTiles[y-1][x];
        a.mTiles[0][2] = mTiles[y-1][right];
    }

    a.mTiles[1][0] = mTiles[y][left];
    a.mTiles[1][1] = mTiles[y][x];
    a.mTiles[1][2] = mTiles[y][right];

    if (y < mSizeY-1)
    {
        a.mTiles[2][0] = mTiles[y+1][left];
        a.mTiles[2][1] = mTiles[y+1][x];
        a.mTiles[2][2] = mTiles[y+1][right];
    }

    return a;
}

// ****************************************************************************

int random(int min, int max)
{
    return (rand()%(max-min))+min;
}

int randStateCovered()
{
	return random(6, 9);
}

int randStateTop()
{
	return random(10, 11);
}

int randStateBot()
{
	return random(12, 13);
}

int randStateRight()
{
	return random(14, 15);
}

int randStateLeft()
{
	return 16;
}
