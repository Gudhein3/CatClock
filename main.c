#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static double getSystemTime() {
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

enum {
    FTRANS = 1<<0,
    FDECOR = 1<<1,
    FRESIZ = 1<<2,
    FSTR2X = 1<<3
};

int main(int argc, char **argv) { // @![GREEN]@![SHELL,cc -o main main.c -lraylib -lm && ./main,Build&Run]@![NOC]
                 //                          !
    if (argc != 2) {
        fprintf(stderr, "Usage: %s flags\n"
                        "flag:\n"
                        "\t't' - transparent\n"
                        "\t'd' - decorated\n"
                        "\t'r' - resizeable\n"
                        "\t'2' - start in 2x resolution\n", argv[0]);
        return 2;
    }
    int flags = 0;
    for (int i = 0; argv[1][i]; i++) {
        switch (argv[1][i]) {
        case 't':
            flags |= FTRANS;
            break;
        case 'd':
            flags |= FDECOR;
            break;
        case 'r':
            flags |= FRESIZ;
            break;
        case '2':
            flags |= FSTR2X;
            break;
        }
    }

    if (flags & FTRANS) {
	    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    }
    if (!(flags & FDECOR)) {
        SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    }
    if (flags & FRESIZ) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }
    int resiz = (flags&FSTR2X) ? 2 : 1;
    InitWindow(150*resiz, 300*resiz, "Cat Clock");
    Texture2D cattie = LoadTexture("kitty.png");
    SetTargetFPS(50);
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (flags & FTRANS)
            ClearBackground((Color) {0});
        else
            ClearBackground(WHITE);

        float width = GetScreenWidth();
        float height = GetScreenHeight();
        float rsx = width/150.;
        float rsy = height/300.;

        DrawTexturePro(cattie, (Rectangle) {0, 0, 150, 300}, (Rectangle) {0, 0, width, height}, (Vector2) {0, 0}, 0, WHITE);
        time_t ct = time(NULL);
        struct tm *ctm = localtime(&ct);
        float rt = getSystemTime();
        float t = rt*2.;
        float r = sin(t)*.5;
        for (int iy = 34; iy <= 34+15; ++iy) {
            float y = ((float)iy-30-23/2.)/(23/2.);
            float x0 = cos(asin(y))*r;
            DrawRectangle((49+23/2.+(x0-.5)*23/2.+3)*rsx,iy*rsy,.5*23/2.*rsx,floorf(rsy*2),BLACK);
            DrawRectangle((80+23/2.+(x0-.5)*23/2.+3)*rsx,iy*rsy,.5*23/2.*rsx,floorf(rsy*2),BLACK);
        }
        float tx = cos((M_PI-2)*(sin(t)+1)*.5+1);
        float ty = sin((M_PI-2)*(sin(t)+1)*.5+1);
        DrawLineEx((Vector2) {75*rsx, 207*rsy}, (Vector2) {(75+tx*88)*rsx, (207+ty*88)*rsy}, 3, BLACK);

        float rr = ctm->tm_sec+60*(ctm->tm_min+12*ctm->tm_hour);

        float ss = fmodf(rr, 60)/60;
        float ms = fmodf(rr/60, 60)/60;
        float hs = fmodf(rr/3600, 12)/12;
        float x,y;
        x = sin(ss*M_PI*2);
        y = -cos(ss*M_PI*2);
        DrawLineEx((Vector2) {73*rsx,149*rsy}, (Vector2) {(73+x*20)*rsx, (149+y*20)*rsy}, 2, RED);
        x = sin(ms*M_PI*2);
        y = -cos(ms*M_PI*2);
        DrawLineEx((Vector2) {73*rsx,149*rsy}, (Vector2) {(73+x*20)*rsx, (149+y*20)*rsy}, 3, GREEN);
        x = sin(hs*M_PI*2);
        y = -cos(hs*M_PI*2);
        DrawLineEx((Vector2) {73*rsx,149*rsy}, (Vector2) {(73+x*12)*rsx, (149+y*12)*rsy}, 3, BLUE);

        for (int a = 0; a < 30; ++a) {
            x = sin((float)a/30.*M_PI*2);
            y = -cos((float)a/30.*M_PI*2);
            DrawLine((75+x*17)*rsx,(151+y*24)*rsy,(75+x*20)*rsx,(151+y*27)*rsy, BLACK);
        }
        EndDrawing();
    }
    return 0;
}
