#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static double getSystemTime() { // TODO: Make it public.
#if defined(_WIN32)
    FILETIME system_time;
    ULARGE_INTEGER large;

    GetSystemTimePreciseAsFileTime(&system_time);
    large.u.LowPart = system_time.dwLowDateTime;
    large.u.HighPart = system_time.dwHighDateTime;
    const uint64_t scale_factor = 1000000;
    uint64_t q = large.QuadPart/10;
    return (double)q/(double)scale_factor;
#elif defined(__APPLE__)
    zetodo("Apple MacOSX");
#else
    // Stolen from RGFW and rewritten with floats.
    struct timespec ts;
    const uint64_t scale_factor = 1000000000;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / (double)scale_factor;
#endif
}

int main(void) { // @![GREEN]@![SHELL,cc -o main main.c -lraylib -lm && ./main,Build&Run]@![NOC]
                 //         !
    InitWindow(150, 300, "Cat Clock");
    Texture2D cattie = LoadTexture("kitty.png");
    SetTargetFPS(50);
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawTexture(cattie, 0, 0, WHITE);
        time_t ct = time(NULL);
        struct tm *ctm = localtime(&ct);
        float rt = getSystemTime();
        float t = rt*2.;
        float r = sin(t)*.5;
        for (int iy = 34; iy <= 34+15; ++iy) {
            float y = ((float)iy-30-23/2.)/(23/2.);
            float x0 = cos(asin(y))*r;
            float x1 = cos(asin(y))*r-.5;
            DrawLine(49+23/2.+x0*23/2.+3,iy,49+23/2.+x1*23/2.+3,iy,BLACK);
            DrawLine(80+23/2.+x0*23/2.+3,iy,80+23/2.+x1*23/2.+3,iy,BLACK);
        }
        float tx = cos((M_PI-2)*(sin(t)+1)*.5+1);
        float ty = sin((M_PI-2)*(sin(t)+1)*.5+1);
        DrawLineEx((Vector2) {75, 207}, (Vector2) {75+tx*88, 207+ty*88}, 3, BLACK);

        float rr = ctm->tm_sec+60*(ctm->tm_min+12*ctm->tm_hour);

        float ss = fmodf(rr, 60)/60;
        float ms = fmodf(rr/60, 60)/60;
        float hs = fmodf(rr/3600, 12)/12;
        float x,y;
        x = sin(ss*M_PI*2);
        y = -cos(ss*M_PI*2);
        DrawLineEx((Vector2) {73,149}, (Vector2) {73+x*20, 149+y*20}, 2, RED);
        x = sin(ms*M_PI*2);
        y = -cos(ms*M_PI*2);
        DrawLineEx((Vector2) {73,149}, (Vector2) {73+x*20, 149+y*20}, 3, GREEN);
        x = sin(hs*M_PI*2);
        y = -cos(hs*M_PI*2);
        DrawLineEx((Vector2) {73,149}, (Vector2) {73+x*12, 149+y*12}, 3, BLUE);

        for (int a = 0; a < 30; ++a) {
            x = sin((float)a/30.*M_PI*2);
            y = -cos((float)a/30.*M_PI*2);
            DrawLine(75+x*17,151+y*24,75+x*20,151+y*27, BLACK);
        }
        EndDrawing();
    }
    return 0;
}
