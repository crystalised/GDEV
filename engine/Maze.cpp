/*==============================================
 * Crate Maze class for GDEV engine
 *
 * Written by <ZHANG XI>
 *==============================================*/
#include <fstream>  // file IO
#include <algorithm>  // for reverse
#include "Maze.h"
#include "Fail.h"

using namespace std;

CMaze::CMaze(CXMesh* pBlock)
{
	mpBlock=pBlock;
}

bool CMaze::Init(const char* name)
{
	mMaze.clear();
	// this is standard file reading code using C++
	// if you have not seen this before, you might want to study it
	ifstream in(name);	// the file
	if (!in)	FAIL(name,"Unable to load maze file");
	while(in.eof()==false)	// until the end of file
	{
		string s;
		getline(in,s);	// gets the line
		if (s.length()>1)	// skip empty lines
			mMaze.push_back(s);	// add to the list
	}
	// this chunk of code will reverse the order of the maze, making it look as the screen
	std::reverse(mMaze.begin(),mMaze.end());
	return true;
}

void CMaze::Draw()
{
	//TODO(CMaze::Draw);
//	for (....)  // this should be 2 loops, X&Z
//	for (....)  // this should be 2 loops, X&Z
//		if (....)
//			mpBlock->Draw(....)
	// PS: display should be on the XZ plane, not XY
	// you should swap the XZ around to make it look right
	for (int i = 0; i<mMaze.size(); i++)
	{
		for (int j = 0; j < mMaze[i].size(); j++)
		{
			if (mMaze[i][j] == '#')
			{
				mpBlock->Draw(D3DXVECTOR3(j, 0, i));
			}
		}
	}
}

char CMaze::GetCell(D3DXVECTOR3 pos)
{
	//TODO(CMaze::GetCell);
	// Hint: convert the x,z to integer
    // NOTE: this is floating point values, you will need to round nearest number
    // look up the function floor() as this will help you
	// check if its outside the size of the array & return '\0'
	// otherwise return the data
    // remember the XY are reversed
	int x = (int)floor(pos.x + 0.5);
	int y = (int)floor(pos.z + 0.5);
	if (x < 0 || y < 0 || y >= mMaze.size() || x >= mMaze[y].size())
		return '\0';
	return mMaze[y][x];
}
bool CMaze::IsClear(D3DXVECTOR3 pos)
{
	//TODO(CMaze::IsClear);
	// this is easy, just call GetCell
    // then see if its clear
	if (GetCell(pos) == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CMaze::IsClear(D3DXVECTOR3 pos,float radius)
{
	//TODO(CMaze::IsClear);
    // you will need to call IsClear(D3DXVECTOR3 pos)
    // several times to check each of the sides of the object
	D3DXVECTOR3 posTopRight((int)(pos.x + radius), pos.y, (int)(pos.z + radius));
	D3DXVECTOR3 posTopLeft((int)(pos.x - radius), pos.y, (int)(pos.z + radius));
	D3DXVECTOR3 posBottomLeft((int)(pos.x - radius), pos.y, (int)(pos.z - radius));
	D3DXVECTOR3 posBottomRight((int)(pos.x + radius), pos.y, (int)(pos.z - radius));

	if (IsClear(pos) && IsClear(posTopRight) && IsClear(posTopLeft) && IsClear(posBottomLeft) && IsClear(posBottomRight))
	{
		return true;
	}
	else
	{
		return false;
	}
}

D3DXVECTOR3 CMaze::WallSlide(D3DXVECTOR3 oldPos,D3DXVECTOR3 newPos,float radius)
{
    // player is trying to move from oldPos to newPos
    // figure out where they can move to and return that
	TODO(CMaze::WallSlide);
    // no hints on this one: just think it through
    return newPos;
}