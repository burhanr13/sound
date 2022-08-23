#include "basicSounds.h"
#include "waveforms.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_AudioSpec audioSpec;
SDL_AudioDeviceID audioID;

Uint64 lastTime = 0;
int frames;
float elapsedTime;
float fps;

int audioCalls;

int quit = 0;

float freq1;
float freq2;
float startT1;
float startT2;

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

        renderScreen();
    }

    close();

    return 0;
}

float soundWave(float t)
{
    return saw(fabsf(t-startT1), freq1) + noise(fabsf(t-startT2), freq2);
}

void getSample(void *userData, Uint8 *stream, int len)
{
    float t = (float)(audioCalls++) / SAMPLE_FREQ;
    *(float *)stream = 0.1 * soundWave(t);
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
        .callback = getSample,
        .userdata = &audioCalls};

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
            case SDLK_a:
                freq1 = 200;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_s:
                freq1 = 225;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_d:
                freq1 = 250;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_f:
                freq1 = 266.66;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_j:
                freq1 = 300;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_k:
                freq1 = 333.33;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_l:
                freq1 = 375;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_SEMICOLON:
                freq1 = 400;
                startT1 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_q:
                freq2 = 200;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_w:
                freq2 = 225;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_e:
                freq2 = 250;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_r:
                freq2 = 266.66;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_u:
                freq2 = 300;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_i:
                freq2 = 333.33;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_o:
                freq2 = 375;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            case SDLK_p:
                freq2 = 400;
                startT2 = (float)audioCalls / SAMPLE_FREQ;
                break;
            }
        }
        break;
    case SDL_KEYUP:
        switch (e->key.keysym.sym)
        {
        case SDLK_a:
        case SDLK_s:
        case SDLK_d:
        case SDLK_f:
        case SDLK_j:
        case SDLK_k:
        case SDLK_l:
        case SDLK_SEMICOLON:
            freq1 = 0;
            break;
        case SDLK_q:
        case SDLK_w:
        case SDLK_e:
        case SDLK_r:
        case SDLK_u:
        case SDLK_i:
        case SDLK_o:
        case SDLK_p:
            freq2 = 0;
            break;
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

    float startT = ((startT1 < startT2) ? startT1 : startT2);

    float prev = soundWave(startT);
    float cur;

    for (int i = 0; i < 200; i++)
    {
        float t = (float)(i + 1) / 200 * 0.01;
        cur = soundWave(t+startT);
        SDL_RenderDrawLine(renderer, 50 + 2 * i, 250 - prev * 100,
                           50 + 2 * (i + 1), 250 - cur * 100);
        prev = cur;
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

    SDL_RenderPresent(renderer);
}
