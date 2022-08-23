#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

#define SAMPLE_FREQ 44100

void init();
void close();
void handleEvent(SDL_Event *e);
void updateTime();

void renderScreen();
void readKeyStates();