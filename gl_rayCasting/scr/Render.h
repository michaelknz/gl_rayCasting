#pragma once
#define _USE_MATH_DEFINES
#include <glew.h>
#include <algorithm>
#include <cmath>
#include <vector>

struct Pointd {
	double aX;
	double aY;
	double dis;
};

class Render
{
public:
	Render(int width, int height);
	~Render();
	void DrawPlayer(int player_X, int player_Y);
	void DrawMap(const int* map, const int map_X, const int map_Y, int tile_size);
	bool CheckWall(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size);
	void CastRays(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double pov, double fov, int width, int height);
	Pointd RayY(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double angle);
	Pointd RayX(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double angle);
	double Distance(double player_X, double player_Y, double AX, double AY);
	void Draw(int width, int height);
private:
	std::vector<std::pair<double,char>> distances;
};

