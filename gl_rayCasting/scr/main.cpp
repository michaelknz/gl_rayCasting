#include "Display.h"
#include "Render.h"

const int width = 1024;
const int height = 512;
const int mapYSize = 8;
const int mapXSize = 8;
const int tileSize = 64;
int speed = 10;
double pov = 180.0;
double fov = 60.0f;
int PlayerPosX = tileSize + 10;
int PlayerPosY = tileSize + 10;

const int map[] = { 1,1,1,1,1,1,1,1,
				    1,0,0,0,0,0,0,1,
				    1,0,0,0,0,0,0,1,
				    1,0,0,1,1,0,0,1,
				    1,0,0,1,1,0,0,1,
				    1,0,0,0,0,0,0,1,
				    1,0,0,0,0,0,0,1,
				    1,1,1,1,1,1,1,1 };

int main(int argc, char** argv) {

	Display display(width, height, "RayCasting");
	Render render(width, height);
	SDL_Event event;

	while (true) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_w) {
					PlayerPosY -= speed;
					if (!render.CheckWall(map, mapXSize, mapYSize, PlayerPosX, PlayerPosY, tileSize)) {
						PlayerPosY += speed;
					}
				}
				if (event.key.keysym.sym == SDLK_s) {
					PlayerPosY += speed;
					if (!render.CheckWall(map, mapXSize, mapYSize, PlayerPosX, PlayerPosY, tileSize)) {
						PlayerPosY -= speed;
					}
				}
				if (event.key.keysym.sym == SDLK_a) {
					PlayerPosX -= speed;
					if (!render.CheckWall(map, mapXSize, mapYSize, PlayerPosX, PlayerPosY, tileSize)) {
						PlayerPosX += speed;
					}
				}
				if (event.key.keysym.sym == SDLK_d) {
					PlayerPosX += speed;
					if (!render.CheckWall(map, mapXSize, mapYSize, PlayerPosX, PlayerPosY, tileSize)) {
						PlayerPosX -= speed;
					}
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					pov += 1;
					if (pov < 0.0) {
						pov += 360.0;
					}
					else if (pov > 360.0) {
						pov -= 360.0;
					}
				}
				if (event.key.keysym.sym == SDLK_LEFT) {
					pov -= 1;
				}
			}
		}
		display.Clear(0.5f, 0.5f, 0.5f, 1.0f);
		render.DrawMap(map, mapXSize, mapYSize, tileSize);
		render.DrawPlayer(PlayerPosX, PlayerPosY);
		render.CastRays(map, mapXSize, mapYSize, PlayerPosX, PlayerPosY, tileSize, pov, fov, width, height);
		render.Draw(width, height);
		display.Swap();
	}

	return 0;
}