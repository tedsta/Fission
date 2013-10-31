#include "GridOps.h"

#include <iostream>

Area veggyGridOp(Area a)
{
    if (a.mTiles[1][1].mMat == 1 && (a.mTiles[0][1].mMat == 3 || a.mTiles[1][0].mMat == 3 || a.mTiles[1][2].mMat == 3 || a.mTiles[2][1].mMat == 3) &&
		(a.mTiles[0][1].mMat == 0 || a.mTiles[1][0].mMat == 0 || a.mTiles[1][2].mMat == 0 || a.mTiles[2][1].mMat == 0))
    {
        std::cout << "veggy me up\n";
        a.mTiles[1][1].mMat = 3;
    }

	return a;
}
