#include "GridComponent.h"

#include <cmath>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

TypeBits GridComponent::Type;
std::vector<sf::Texture*> GridComponent::TileSheets;

GridComponent::GridComponent(int sizeX, int sizeY, Tile** tiles, int tickCount) : mSizeX(sizeX), mSizeY(sizeY),
    mTiles(tiles), mTickCount(tickCount)
{
	/*for (int y = 0; y < mSizeY; y++)
		for (int x = 0; x < mSizeX; x++)
			calcNeighborState(x, y);*/

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
	int top = std::max<float>(centerT.y-1, 0);
	int right = centerT.x + ssXT + 1;
	int bot = std::min<float>(centerT.y+ssYT+1, mSizeY);

    sf::VertexArray verts(sf::Quads, 4);
	for (int _y = top; _y <= bot; _y++)
	{
		for (int _x = left; _x <= right; _x++)
        {
			int x = _x;
			int y = _y;

			if (x < 0)
            {
				x = (x % mSizeX);
				if (x < 0)
					x += mSizeX;
			}
			else if (x >= mSizeX)
				x %= mSizeX;

			if (mTiles[y][x].mMat == 0)
				continue;

			// Grab tile sheet info
			sf::Texture* sheet = TileSheets[mTiles[y][x].mMat];
			if (!sheet)
                continue;

			int sheetSizeX = sheet->getSize().x / TILE_SIZE;
			int sheetSizeY = sheet->getSize().y / TILE_SIZE;


			auto start = sf::Vector2f(tsize * static_cast<float>(_x), tsize * static_cast<float>(_y)); // Tile start draw
			verts[0] = sf::Vertex(start,
				sf::Color{255, 255, 255, 255},
				sf::Vector2f());
			verts[1] = sf::Vertex(start+sf::Vector2f(0, tsize),
				sf::Color{255, 255, 255, 255},
				sf::Vector2f(0, tsize));
			verts[2] = sf::Vertex(start+sf::Vector2f(tsize, tsize),
				sf::Color{255, 255, 255, 255},
				sf::Vector2f(tsize, tsize));
			verts[3] = sf::Vertex(start+sf::Vector2f(tsize, 0),
				sf::Color{255, 255, 255, 255},
				sf::Vector2f(tsize, 0));

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

			/*if joelMode {
				for i, c := range mTiles[y][x].comp {
					bheight := (float32(c) / 255 * 16)
					bwidth := float32(TILE_SIZE / MaxComps)
					bstart := start.Add(sf::Vector2f{float32(i) * bwidth, 16 - bheight})

					color := sf::Color{}
					color.A = 255

					switch i + int(mTiles[y][x].mat) {
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
	sf::Vector2f offset = cOffset-sf::Vector2f(dim.x / 2, dim.y / 2);
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
		else if (offset.x <= 0 && (dir == RIGHT || dir == RIGHT))
			fix -= float(TILE_SIZE);
	}

	if (cOffset.x-(dim.x/2) < 0 || cOffset.x+(dim.x/2) > mSizeX*TILE_SIZE)
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
			int x = _x % mSizeX;
			int y = _y;

			if (mTiles[y][x].mMat != 0)
            {
				switch (dir)
				{
				case RIGHT:
					fix = -(right - _x);
				case DOWN:
					fix = -(bot - _y);
				}
				return true;
			}
		}
	}
	return false;
}

sf::Vector2f getTilePos(sf::Transformable* myTrans, sf::Vector2f pos)
{
	sf::Transform myInv = myTrans->getTransform().getInverse();
	pos = myInv.transformPoint(pos);
	pos = pos/float(TILE_SIZE);
	pos.x = floor(pos.x);
	pos.y = floor(pos.y);
	return pos;
}

/*func (g *GridComponent) SetTile(x, y int, tile Tile, tick int) {
	if y < 0 || y >= mSizeY {
		return
	}

	x = g.WrapX(x)

	if mTiles[y][x].mat == tile.mat &&
		mTiles[y][x].force == tile.force && mTiles[y][x].heat == tile.heat {
		return
	}

	mTiles[y][x] = tile

	left := g.WrapX(x - 1)
	right := g.WrapX(x + 1)
	var top, bot int = y, y

	if y > 0 {
		top = y - 1
	} else if y < mSizeY-1 {
		top = y + 1
	}

	// If tick is -1, make it interesting for all ticks
	var tickMin, tickMax = 0, g.mTickCount
	if tick != -1 {
		tickMin, tickMax = tick, tick+1
	}

	for t := tickMin; t < tickMax; t++ {
		for i := top; i <= bot; i++ {
			if mTiles[i][left].mat != 0 {
				g.calcNeighborState(left, i)
				mCTiles[t] = append(mCTiles[t], [2]int{left, i})
			}
			if mTiles[i][x].mat != 0 {
				g.calcNeighborState(x, i)
				mCTiles[t] = append(mCTiles[t], [2]int{x, i})
			}
			if mTiles[i][right].mat != 0 {
				g.calcNeighborState(right, i)
				mCTiles[t] = append(mCTiles[t], [2]int{right, i})
			}
		}
	}
}

func (g *GridComponent) calcNeighborState(x, y int) {
	left := g.WrapX(x - 1)
	right := g.WrapX(x + 1)

	var a [3][3]Tile // Area

	if y > 0 {
		a[0][0] = mTiles[y-1][left]
		a[0][1] = mTiles[y-1][x]
		a[0][2] = mTiles[y-1][right]
	}

	a[1][0] = mTiles[y][left]
	a[1][1] = mTiles[y][x]
	a[1][2] = mTiles[y][right]

	if y < mSizeY-1 {
		a[2][0] = mTiles[y+1][left]
		a[2][1] = mTiles[y+1][x]
		a[2][2] = mTiles[y+1][right]
	}

	mTiles[y][x].state = earthState(a)
}*/
