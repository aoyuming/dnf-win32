#ifndef _DEFINE_H
#define _DEFINE_H

#include <string>
#include <map>
#include <vector>
struct File_W_H
{
	int w;
	int h;
};
enum MyEnum : char
{
	_MOVE_UP,
	_MOVE_DOWN,
	_MOVE_LEFT,
	_MOVE_RIGHT,
	PTGJ,
};



extern std::string m_Nick_Name;
extern std::map<std::string,File_W_H> g_FileName;
extern int g_mapX,g_mapY;
//extern std::vector<std::string> edit_string; 
#endif