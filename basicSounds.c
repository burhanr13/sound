#include "basicSounds.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_AudioSpec audioSpec;
SDL_AudioDeviceID audioID;

Uint64 lastTime = 0;
int frames;
float elapsedTime;
float fps;

int audioCalls = 0;

int quit = 0;

float freq = 400;
int harmonics = 1;
int saw = 0;

int main(int argc, char *argv[])
{
    init();

    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            handleEvent(&e);
        }

        updateTime();

        readKeyStates();

        renderScreen();
    }

    close();

    return 0;
}

float soundWave(float t)
{
    float wave = 0;
    for (int i = 1; i <= harmonics; i++)
    {
        wave += sinf(2 * M_PI * t * freq * i) / i;
        if (!saw)
            wave += sinf(2 * M_PI * (0.5 / freq - t) * freq * i) / i;
    }
    return 0.1 * wave;
}

void getSample(void *userData, Uint8 *stream, int len)
{
    *(float *)stream = soundWave((float)(audioCalls++) / SAMPLE_FREQ);
}

void init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Sound", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

    audioSpec = (SDL_AudioSpec){
        .freq = SAMPLE_FREQ,
        .format = AUDIO_F32,
        .channels = 1,
        .samples = 1,
        .callback = getSample};
    audioID = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
    SDL_PauseAudioDevice(audioID, 0);
}

void close()
{
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_Quit();
    IMG_Quit();
}

void handleEvent(SDL_Event *e)
{
    switch (e->type)
    {
    case SDL_QUIT:
        quit = 1;
        break;
    case SDL_KEYDOWN:
        if (!e->key.repeat)
        {
            switch (e->key.keysym.sym)
            {
            case SDLK_RIGHT:
                harmonics++;
                if (harmonics > 100)
                    harmonics = 100;
                break;
            case SDLK_LEFT:
                harmonics--;
                if (harmonics < 1)
                    harmonics = 1;
                break;
            case SDLK_SPACE:
                saw = !saw;
            }
        }
    }
}

void updateTime()
{
    elapsedTime = (float)(SDL_GetTicks64() - lastTime) / 1000;
    lastTime = SDL_GetTicks64();
    frames++;
    fps = (float)frames * 1000 / SDL_GetTicks64();
}

void renderScreen()
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    float prev = soundWave(0);
    float cur;

    for (int i = 0; i < 200; i++)
    {
        float t = (float)(i + 1) / 200 * 0.005;
        cur = soundWave(t);
        SDL_RenderDrawLine(renderer, 50 + 2 * i, 250 - prev * 1000, 50 + 2 * (i + 1), 250 - cur * 1000);
        prev = cur;
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

    SDL_RenderPresent(renderer);
}

void readKeyStates()
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    freq += 0.1 * (keys[SDL_SCANCODE_UP] - keys[SDL_SCANCODE_DOWN]);
    if (freq < 0)
        freq = 0;
}