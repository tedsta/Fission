#include "GridOps.h"

#include <iostream>

#include "GridComponent.h"

Area veggyGridOp(Area a)
{
    if (a.mTiles[1][1].mMat == 1 && (a.mTiles[0][1].mMat == 3 || a.mTiles[1][0].mMat == 3 || a.mTiles[1][2].mMat == 3 || a.mTiles[2][1].mMat == 3) &&
		(a.mTiles[0][0].mMat == 0 || a.mTiles[0][1].mMat == 0 || a.mTiles[0][2].mMat == 0 ||
        a.mTiles[1][0].mMat == 0 || a.mTiles[1][2].mMat == 0 ||
        a.mTiles[2][0].mMat == 0 || a.mTiles[2][1].mMat == 0 || a.mTiles[2][2].mMat == 0))
    {
        a.mTiles[1][1].mMat = 3;
        a.mChanged = true;
    }

	return a;
}

Area wireGridOp(Area a)
{
    if (a.mTiles[1][1].mWire == 0)
        return a;

    if (a.mTiles[1][1].mWire == 3)
    {
        a.mTiles[1][1].mWire = 1;
        a.mChanged = true;
    }
    else if (a.mTiles[1][1].mWire == 2 && (a.mTiles[0][1].mWire == 3 || a.mTiles[1][0].mWire == 3 ||
                                      a.mTiles[1][2].mWire == 3 || a.mTiles[2][1].mWire == 3))
    {
        a.mTiles[1][1].mWire = 3;
        a.mChanged = true;
    }
    else if (a.mTiles[1][1].mWire == 1 && (a.mTiles[0][1].mWire == 2 || a.mTiles[1][0].mWire == 2 ||
                                      a.mTiles[1][2].mWire == 2 || a.mTiles[2][1].mWire == 2))
    {
        a.mTiles[1][1].mWire = 2;
        a.mChanged = true;
    }

	return a;
}

// ******************************************************************************************************

const float MaxMass = 1.f;
const float MaxCompress = 0.02f; //How much excess water a cell can store, compared to the cell above it
const float MinMass = 0.001f;
const float MinFlow = 0.01f;
const float MaxSpeed = 1.f;

bool isFluid(int mat){return mat==0||mat==4;}

//Returns the amount of water that should be in the bottom cell.
float fluidStableState(float mass)
{
    if (mass <= 1)
        return 1;
    else if (mass < 2*MaxMass + MaxCompress)
        return (MaxMass*MaxMass + mass*MaxCompress)/(MaxMass + MaxCompress);
    else
        return (mass + MaxCompress)/2;
}

float constrain(float v, float min, float max)
{
    if (v < min)
        v = min;
    else if (v > max)
        v = max;
    return v;
}

/*void fluidGridOp2(GridComponent* grid, int tick)
{
    for (int i = 0; i < grid->getInterestingTiles(tick).size(); i++)
    {
        int x = grid->getInterestingTiles(tick)[i].x;
        int y = grid->getInterestingTiles(tick)[i].y;

        float m01, m10, m11, m12, m21;
        m01 = float(a.mTiles[0][1].mState);
        m10 = float(a.mTiles[1][0].mState);
        m11 = float(a.mTiles[1][1].mState);
        m12 = float(a.mTiles[1][2].mState);
        m21 = float(a.mTiles[2][1].mState);

        //Custom push-only flow
        float flow = 0;
        float remaining = m11;
        if (remaining <= 0) return r;

        //The block below this one
        if (remaining > 0 && isFluid(a.mTiles[2][1].mMat))
        {
            flow = fluidStableState(remaining + m21) - m21;
            if (flow > MinFlow)
                flow *= 0.5; //leads to smoother flow

            flow = constrain( flow, 0, std::min(MaxSpeed, remaining) );

            r.mTiles[1][1].mState -= flow;
            r.mTiles[2][1].mState += flow;
            remaining -= flow;
            r.mChanged = true;
        }

        //Left
        if (remaining > 0 && isFluid(a.mTiles[1][0].mMat))
        {
            //Equalize the amount of water in this block and it's neighbour
            flow = (m11 - m10)/4;
            if (flow > MinFlow) flow *= 0.5;
            flow = constrain(flow, 0, remaining);

            r.mTiles[1][1].mState -= flow;
            r.mTiles[1][0].mState += flow;
            remaining -= flow;
            r.mChanged = true;
        }

        //Right
        if (remaining > 0 && isFluid(a.mTiles[1][2].mMat))
        {
            //Equalize the amount of water in this block and it's neighbour
            flow = (m11 - m12)/4;
            if (flow > MinFlow) flow *= 0.5;
            flow = constrain(flow, 0, remaining);

            r.mTiles[1][1].mState -= flow;
            r.mTiles[1][2].mState += flow;
            remaining -= flow;
            r.mChanged = true;
        }

        //Up. Only compressed water flows upwards.
        if (remaining > 0 && isFluid(a.mTiles[0][1].mMat))
        {
            flow = remaining - fluidStableState(remaining + m01);
            if (flow > MinFlow) flow *= 0.5;
            flow = constrain( flow, 0, std::min(MaxSpeed, remaining) );

            r.mTiles[1][1].mState -= flow;
            r.mTiles[0][1].mState += flow;
            remaining -= flow;
            r.mChanged = true;
        }
    }

	return r;
}*/

// ******************************************************************************************************
// ******************************************************************************************************
// ******************************************************************************************************
// ******************************************************************************************************
// ******************************************************************************************************

#define MAX_FLUID 127
#define compress 3

int vert(int top, int bot, int offset)
{
    int total = top+bot;
    int flow = 0;

    int topMaxFill = std::min(MAX_FLUID+offset*compress, 255);
    int botMaxFill = std::min(MAX_FLUID+(offset+1)*compress, 255);
    if (top > 0 && top <= topMaxFill && bot < botMaxFill)
        flow = std::min<int>(top, botMaxFill-bot);

    else if (bot > botMaxFill && top < 255)
    {
        //std::cout << "Top: " << top << std::endl << "Bot: " << bot << std::endl << "Max: " << maxFill << std::endl;
        flow = -std::min(bot-botMaxFill, 255-top);
    }

    return flow;
}

int horiz(int p, int s, int po, int so)
{
    int pMaxFill = std::min(MAX_FLUID+po*compress, 255);
    int sMaxFill = std::min(MAX_FLUID+so*compress, 255);
    int flow = 0;

    if (p > s && po > so)
    {
        flow = -compress;
    }
    else if (s > p && so > po)
        flow = compress;

    /*if (top > 0 && top <= topMaxFill && bot < botMaxFill)
        flow = std::min<int>(top, botMaxFill-bot);

    else if (bot > botMaxFill && top < 255)
    {
        //std::cout << "Top: " << top << std::endl << "Bot: " << bot << std::endl << "Max: " << maxFill << std::endl;
        flow = -std::min(bot-botMaxFill, 255-top);
    }*/

    return flow;
}

Area fluidGridOp(Area a)
{
    if (!isFluid(a.mTiles[1][1].mMat))
        return a;

    if (a.mTiles[1][1].mMat == 4)
    {
        if (a.mTiles[0][1].mMat != 4)
        {
            a.mTiles[1][1].mSignal = 0;
            a.mChanged = true;
        }
        else if (a.mTiles[1][1].mSignal != a.mTiles[0][1].mSignal+1)
        {
            a.mTiles[1][1].mSignal = a.mTiles[0][1].mSignal+1;
            a.mChanged = true;
        }
    }

    int mat00 = a.mTiles[0][0].mMat;
    int mat01 = a.mTiles[0][1].mMat;
    int mat02 = a.mTiles[0][2].mMat;
    int mat10 = a.mTiles[1][0].mMat;
    int mat11 = a.mTiles[1][1].mMat;
    int mat12 = a.mTiles[1][2].mMat;
    int mat20 = a.mTiles[2][0].mMat;
    int mat21 = a.mTiles[2][1].mMat;
    int mat22 = a.mTiles[2][2].mMat;

    int m00 = a.mTiles[0][0].mFluid;
    int m01 = a.mTiles[0][1].mFluid;
    int m02 = a.mTiles[0][2].mFluid;
    int m10 = a.mTiles[1][0].mFluid;
    int m11 = a.mTiles[1][1].mFluid;
    int m12 = a.mTiles[1][2].mFluid;
    int m20 = a.mTiles[2][0].mFluid;
    int m21 = a.mTiles[2][1].mFluid;
    int m22 = a.mTiles[2][2].mFluid;

    int o00 = a.mTiles[0][0].mSignal;
    int o01 = a.mTiles[0][1].mSignal;
    int o02 = a.mTiles[0][2].mSignal;
    int o10 = a.mTiles[1][0].mSignal;
    int o11 = a.mTiles[1][1].mSignal;
    int o12 = a.mTiles[1][2].mSignal;
    int o20 = a.mTiles[2][0].mSignal;
    int o21 = a.mTiles[2][1].mSignal;
    int o22 = a.mTiles[2][2].mSignal;

    int flow = 0;
    if (isFluid(mat21))
        flow -= vert(m11, m21, o11);

    if (isFluid(mat01))
        flow += vert(m01, m11, o01);

    int tmpFlow = flow;

    if (isFluid(mat10))
    {
        flow += horiz(mat11+tmpFlow, mat10, o11, o10);
        /*int leftVertFlow = 0;
        if (isFluid(mat20))
            leftVertFlow -= vert(m10, m20, o10);
        if (isFluid(mat00))
            leftVertFlow += vert(m00, m10, o00);

        if (m10+leftVertFlow >= m11+tmpFlow+compress)
            flow += compress;
        else if (m11+tmpFlow >= m10+leftVertFlow+compress)
            flow -= compress;*/
        //flow += -horiz(m11+tmpFlow, m10 - (isFluid(mat20) ? vert(m10, m20, o10) : 0));
    }

    /*if (isFluid(mat12))
    {
        int rightVertFlow = 0;
        if (isFluid(mat22))
            rightVertFlow -= vert(m12, m22, o12);
        if (isFluid(mat02))
            rightVertFlow += vert(m02, m12, o02);

        if (m12+rightVertFlow >= m11+tmpFlow+compress)
            flow += compress;
        else if (m11+tmpFlow >= m12+rightVertFlow+compress)
            flow -= compress;
        //flow += -horiz(m11+tmpFlow, m12 - (isFluid(mat22) ? vert(m12, m22, o12) : 0));
    }*/

    a.mTiles[1][1].mFluid += flow;

    if (a.mTiles[1][1].mFluid > 0)
        a.mTiles[1][1].mMat = 4;
    else
        a.mTiles[1][1].mMat = 0;

    if (a.mTiles[1][1].mMat == 4)
    {
        if (a.mTiles[0][1].mMat != 4)
        {
            a.mTiles[1][1].mSignal = 0;
            a.mChanged = true;
        }
        else if (a.mTiles[1][1].mSignal != a.mTiles[0][1].mSignal+1)
        {
            a.mTiles[1][1].mSignal = a.mTiles[0][1].mSignal+1;
            a.mChanged = true;
        }
    }

    a.mChanged = a.mChanged || flow!=0;

    return a;

    // Flow down
    if (a.mTiles[0][1].mMat == 4 && a.mTiles[0][1].mFluid > 0 && a.mTiles[1][1].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[0][1].mFluid, MAX_FLUID-a.mTiles[1][1].mFluid);
        int change = tofill/3 + (tofill%3);
        m11 += change;
        m01 -= change;
        a.mChanged = true;
    }
    if (a.mTiles[1][1].mMat == 4 && (a.mTiles[2][1].mMat == 4 || a.mTiles[2][1].mMat == 0) &&
        a.mTiles[1][1].mFluid > 0 && a.mTiles[2][1].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[1][1].mFluid, MAX_FLUID-a.mTiles[2][1].mFluid);
        int change = tofill/3 + (tofill%3);
        m11 -= change;
        m21 += change;
        a.mChanged = true;
    }

    // Flow right to left
    if (a.mTiles[1][2].mMat == 4 && a.mTiles[1][2].mFluid > 0 && a.mTiles[1][1].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[1][2].mFluid, MAX_FLUID-a.mTiles[1][1].mFluid);
        int change = tofill/3;
        m11 += change;
        a.mChanged = true;
    }
    if (a.mTiles[1][1].mMat == 4 && (a.mTiles[1][0].mMat == 4 || a.mTiles[1][0].mMat == 0) &&
        a.mTiles[1][1].mFluid > 0 && a.mTiles[1][0].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[1][1].mFluid, MAX_FLUID-a.mTiles[1][0].mFluid);
        int change = tofill/3;
        m11 -= change;
        a.mChanged = true;
    }

    // Flow left to right
    if (a.mTiles[1][0].mMat == 4 && a.mTiles[1][0].mFluid > 0 && a.mTiles[1][1].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[1][0].mFluid, MAX_FLUID-a.mTiles[1][1].mFluid);
        int change = tofill/3;
        m11 += change;
        a.mChanged = true;
    }
    if (a.mTiles[1][1].mMat == 4 && (a.mTiles[1][2].mMat == 4 || a.mTiles[1][2].mMat == 0) &&
        a.mTiles[1][1].mFluid > 0 && a.mTiles[1][2].mFluid < MAX_FLUID)
    {
        int tofill = std::min<int>(a.mTiles[1][1].mFluid, MAX_FLUID-a.mTiles[1][2].mFluid);
        int change = tofill/3;
        m11 -= change;
        a.mChanged = true;
    }

    a.mTiles[1][1].mFluid = m11;

    if (a.mTiles[1][1].mFluid > 0 && a.mTiles[1][1].mMat == 0)
        a.mTiles[1][1].mMat = 4;
    else if (a.mTiles[1][1].mFluid == 0 && a.mTiles[1][1].mMat == 4)
        a.mTiles[1][1].mMat = 0;

    return a;
}

void flowVert(Area& a)
{
    /*for (int x = 0; x < 3; x++)
    {
        int upMat = a.mTiles[0][x].mMat;
        int midMat = a.mTiles[1][x].mMat;
        int downMat = a.mTiles[2][x].mMat;

        int up = a.mTiles[0][x].mFluid;
        int mid = a.mTiles[1][x].mFluid;
        int down = a.mTiles[2][x].mFluid;

        int upO = a.mTiles[0][x].mSignal;
        int midO = a.mTiles[1][x].mSignal;
        int downO = a.mTiles[2][x].mSignal;

        if (a.mTiles[1][1].mMat == 4)
        {
            if (a.mTiles[0][1].mMat != 4)
            {
                a.mTiles[1][1].mSignal = 0;
                a.mChanged = true;
            }
            else if (a.mTiles[1][1].mSignal != a.mTiles[0][1].mSignal+1)
            {
                a.mTiles[1][1].mSignal = a.mTiles[0][1].mSignal+1;
                a.mChanged = true;
            }
        }

        int o01 = a.mTiles[0][1].mSignal;
        int o02 = a.mTiles[0][2].mSignal;
        int o10 = a.mTiles[1][0].mSignal;
        int o11 = a.mTiles[1][1].mSignal;
        int o12 = a.mTiles[1][2].mSignal;
        int o20 = a.mTiles[2][0].mSignal;
        int o21 = a.mTiles[2][1].mSignal;
        int o22 = a.mTiles[2][2].mSignal;

        int flow = 0;
        if (isFluid(mat21))
            flow -= vert(m11, m21, o11);

        if (isFluid(mat01))
            flow += vert(m01, m11, o01);

        int tmpFlow = flow;

        if (isFluid(mat10))
            flow += -horiz(m11+tmpFlow, m10 - (isFluid(mat20) ? vert(m10, m20, o10) : 0));
        //std::cout << "Flow step 2: " << (isFluid(mat20) ? vert(m10, m20) : 0) << std::endl;

        if (isFluid(mat12))
            flow += -horiz(m11+tmpFlow, m12 - (isFluid(mat22) ? vert(m12, m22, o12) : 0));
        //std::cout << "Flow step 2: " << (isFluid(mat22) ? vert(m12, m22) : 0) << std::endl;

        a.mTiles[1][1].mFluid += flow;

        a.mTiles[0][x].mFluid = up;
        a.mTiles[1][x].mState = mid;
        a.mTiles[2][x].mState = down;
    }*/
}
