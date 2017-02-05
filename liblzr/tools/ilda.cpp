
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <liblzr.hpp>

using namespace lzr;


#define DEFAULT_WINDOW_SIZE 500
#define POINT_SIZE 3
#define FPS 15


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

FrameList frames;
int frame_n = 0;

//settings
bool play = false;
bool show_blanks = false;
bool show_points = false;

static inline void next_frame()
{
    frame_n++;
    frame_n = (frame_n >= (int)frames.size()) ? 0 : frame_n;
}

static inline void prev_frame()
{
    frame_n--;
    frame_n = (frame_n < 0) ? frames.size() - 1 : frame_n;
}

static inline int lzr_coord_to_screen(double v, bool invert)
{
    v = invert ? (v * -1.0) : v;
    return (int) (((v + 1.0) / 2.0) * DEFAULT_WINDOW_SIZE);
}

static inline SDL_Point lzr_point_to_sdl_point(Point p)
{
    SDL_Point sp;
    sp.x = lzr_coord_to_screen(p.x, false);
    sp.y = lzr_coord_to_screen(p.y, true); //invert
    return sp;
}

static inline SDL_Rect lzr_point_to_sdl_rect(Point p)
{
    SDL_Point sp = lzr_point_to_sdl_point(p);
    SDL_Rect r = {
        sp.x - ((POINT_SIZE - 1) / 2),
        sp.y - ((POINT_SIZE - 1) / 2),
        POINT_SIZE,
        POINT_SIZE
    };
    return r;
}

static inline SDL_Color lzr_color_to_screen(Point p)
{
    SDL_Color c;
    c.r = p.r;
    c.g = p.g;
    c.b = p.b;
    c.a = p.is_blanked() ? 0 : 255;

    if(show_blanks && p.is_blanked())
    {
        c.r = 255;
        c.g = 255;
        c.b = 255;
        c.a = 255;
    }

    return c;
}

static void render()
{
    // clear the screen to black
    SDL_SetRenderDrawColor(renderer,
                           0,
                           0,
                           0,
                           255);

    SDL_RenderClear(renderer);

    Frame& frame = frames[frame_n];

    //NOTE: using int's to avoid rollover problems with -1 (empty frames)
    for(int i = 0; i < ((int)frame.size() - 1); i++)
    {
        Point p1 = frame[i];
        Point p2 = frame[i+1];
        SDL_Point sp1 = lzr_point_to_sdl_point(p1);
        SDL_Point sp2 = lzr_point_to_sdl_point(p2);

        //TODO: double check that color is actually
        //      a property of the second point
        SDL_Color c   = lzr_color_to_screen(p2);
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

        //test whether this is a static point, or a line
        if(p1.same_position_as(p2))
        {
            SDL_Rect r = lzr_point_to_sdl_rect(p1);
            SDL_RenderFillRect(renderer, &r);
        }
        else
        {
            SDL_RenderDrawLine(renderer, sp1.x, sp1.y, sp2.x, sp2.y);
        }

    }

    //draw points overtop of the lines
    if(show_points)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for(Point p : frame)
        {
            SDL_Rect r = lzr_point_to_sdl_rect(p);
            SDL_RenderFillRect(renderer, &r);
        }
    }

    SDL_RenderPresent(renderer);
}

static void loop()
{
    SDL_Event e;
    bool running = true;

    while(running)
    {
        if(!play)
        {
            // if we're not playing, stop, and wait for an event
            SDL_WaitEvent(NULL);
        }

        //event pump
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                //dispatch SDL events to their respective handlers
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                            play = !play;
                            break;
                        case SDLK_b:
                            show_blanks = !show_blanks;
                            break;
                        case SDLK_p:
                            show_points = !show_points;
                            break;
                        case SDLK_LEFT:
                            prev_frame();
                            break;
                        case SDLK_RIGHT:
                            next_frame();
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(e.wheel.y > 0)
                    {
                        next_frame();
                    }
                    else if(e.wheel.y < 0)
                    {
                        prev_frame();
                    }
                    break;
                case SDL_WINDOWEVENT:
                        if((e.window.event == SDL_WINDOWEVENT_RESIZED) ||
                           (e.window.event == SDL_WINDOWEVENT_MAXIMIZED) ||
                           (e.window.event == SDL_WINDOWEVENT_RESTORED))
                        break;
                default:
                    break;
            }
        }

        render();

        if(play)
        {
            next_frame();
        }

        SDL_Delay(1000 / FPS);
    }
}


int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        fprintf(stderr, "Usage: ilda <ilda file>\n");
        return EXIT_SUCCESS;
    }

    // load the ILDA file
    ILDA* f = ilda_open(argv[1], "r");

    printf("Found %zu projector(s)\n", ilda_projector_count(f));
    printf("Found %zu frames for projector 0\n", ilda_frame_count(f, 0));

    if(ilda_read(f, 0, frames) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error loading frames\n");
        return EXIT_FAILURE;
    }

    ilda_close(f);

    play = frames.size() > 1;

    //start SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error\n%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("LZR Visualizer",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              DEFAULT_WINDOW_SIZE,
                              DEFAULT_WINDOW_SIZE,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error\n%s\n", SDL_GetError());
        goto err_window;
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error\n%s\n", SDL_GetError());
        goto err_render;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // clear the screen to black
    render();

    //start the main loop
    loop();

    //cleanup
    SDL_DestroyRenderer(renderer);
err_render:
    SDL_DestroyWindow(window);
err_window:
    SDL_Quit();


    return 0;
}
