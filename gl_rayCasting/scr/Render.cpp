#include "Render.h"

Render::Render(int width, int height) {
	distances.resize(width / 2);
}

Render::~Render() {

}

void Render::DrawPlayer(int player_X, int player_Y) {

	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(player_X, player_Y);
	glEnd();
}

void Render::DrawMap(const int* map, const int map_X, const int map_Y, int tile_size) {
	for (int i = 0; i < map_Y; ++i) {
		for (int j = 0; j < map_X; ++j) {
			if (map[i*map_X+j]) {
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			glBegin(GL_QUADS);
			glVertex2i(j * 64, i * 64);
			glVertex2i(j * 64, i * 64 + 64);
			glVertex2i(j * 64 + 64, i * 64 + 64);
			glVertex2i(j * 64 + 64, i * 64);
			glEnd();
		}
	}
}
bool Render::CheckWall(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size) {
	int i = player_Y / tile_size;
	int j = player_X / tile_size;
	if (map[i * map_X + j] == 1) {
		return false;
	}
	return true;
}

void Render::CastRays(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double pov, double fov, int width, int height) {
	double angle = pov - fov / 2.0;
	double del_angle = fov / ((double)width / 2.0);
	double pangle = -fov / 2.0;
	for (int i = 0; i < width / 2.0; ++i) {
		Pointd py = RayY(map, map_X, map_Y, player_X, player_Y, tile_size, angle);
		Pointd px = RayX(map, map_X, map_Y, player_X, player_Y, tile_size, angle);
		glColor3f(1, 0, 0);
		if (py.dis <= px.dis) {
			glBegin(GL_LINES);
			glVertex2i(player_X, player_Y);
			glVertex2i(py.aX, py.aY);
			glEnd();
			distances[i].first = py.dis * cos(pangle * M_PI / 180.0);
			distances[i].second = 'y';
		}
		else {
			glBegin(GL_LINES);
			glVertex2i(player_X, player_Y);
			glVertex2i(px.aX, px.aY);
			glEnd();
			distances[i].first = px.dis * cos(pangle * M_PI / 180.0);
			distances[i].second = 'x';
		}
		angle += del_angle;
		pangle += del_angle;
	}
}

Pointd Render::RayY(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double angle) {
	int del_y = 0;
	int dY = 0;
	int aY = 0;
	double aX = 0.0;
	double dX = 0.0;
	bool b = false;
	if (angle < 0.0) {
		angle += 360.0;
	}
	else if (angle > 360.0) {
		angle -= 360.0;
	}
	double t = 1 / tan(angle * M_PI / 180.0);
	if (angle == 90.0 || angle == 270.0) {
		t = 0.0;
	}
	if (angle > 0.0 && angle < 180.0) {
		del_y = 64;
		dY = tile_size;
	}
	else if (angle > 180.0 && angle < 360.0) {
		del_y = -1;
		dY = -tile_size;
	}
	if (angle == 0.0 || angle == 360.0) {
		b = true;
		aX = 100000.0;
		aY = 0;
	}
	else if(angle == 180.0) {
		b = true;
		aX = -100000.0;
		aY = 0;
	}
	if (!b) {
		dX = (double)dY * t;
		aY = (player_Y / tile_size) * tile_size + del_y;
		aX = player_X + (((double)aY - (double)player_Y) * t);
		int i = aY / tile_size;
		int j = (int)aX / tile_size;
		while (i >= 0 && j >= 0 && i < map_Y && j < map_X && map[i * map_X + j] != 1) {
			aX += dX;
			aY += dY;
			i = aY / tile_size;
			j = (int)aX / tile_size;
		}
	}
	Pointd output;
	output.aX = aX;
	output.aY = aY;
	output.dis = Distance((double)player_X, (double)player_Y, (double)aX, (double)aY);
	return output;
}

double Render::Distance(double player_X, double player_Y, double AX, double AY) {
	return sqrt(((AX - player_X) * (AX - player_X)) + ((AY - player_Y) * (AY - player_Y)));
}

Pointd Render::RayX(const int* map, const int map_X, const int map_Y, int player_X, int player_Y, int tile_size, double angle) {
	int del_X = 0;
	int dX = 0;
	int aX = 0;
	double aY = 0.0;
	double dY = 0.0;
	bool b = false;
	if (angle < 0.0) {
		angle += 360.0;
	}
	else if (angle > 360.0) {
		angle -= 360.0;
	}
	double t = tan(angle * M_PI / 180.0);
	if (angle > 270.0 || angle < 90.0) {
		del_X = 64;
		dX = tile_size;
	}
	else if (angle > 90.0 || angle < 270.0) {
		del_X = -1;
		dX = -tile_size;
	}
	if (angle == 90.0) {
		b = true;
		aX = 0.0;
		aY = 100000.0;
	}
	else if (angle == 270.0) {
		b = true;
		aX = 0;
		aY = -100000.0;
	}
	if (!b) {
		dY = (double)dX * t;
		aX = (player_X / tile_size) * tile_size + del_X;
		aY = player_Y + (((double)aX - (double)player_X) * t);
		int i = (int)aY / tile_size;
		int j = aX / tile_size;
		while (i >= 0 && j >= 0 && i < map_Y && j < map_X && map[i * map_X + j] != 1) {
			aX += dX;
			aY += dY;
			i = (int)aY / tile_size;
			j = aX / tile_size;
		}
	}
	Pointd output;
	output.aX = aX;
	output.aY = aY;
	output.dis = Distance((double)player_X, (double)player_Y, (double)aX, (double)aY);
	return output;
}

void Render::Draw(int width, int height) {
	for (int i = 0; i < distances.size(); ++i) {
		int h = (double)height*64 / distances[i].first;
		h = std::min(h, height);
		int start = height / 2 - h / 2;
		int end = start + h - 1;
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex2i(width / 2 + i, start);
		glVertex2i(width / 2 + i, end);
		glEnd();
		if (end < height - 1) {
			glColor3f(0, 0, 1);
			glBegin(GL_LINES);
			glVertex2i(width / 2 + i, end+1);
			glVertex2i(width / 2 + i, height-1);
			glEnd();
		}
	}
}