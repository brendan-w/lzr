
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL.h>

#include "lzr/lzr.hpp"

using namespace lzr;


#define DEFAULT_WINDOW_SIZE 1024
#define POINT_SIZE 3

static constexpr size_t play_period_ms = 70;
static constexpr size_t animate_scan_period_ms = 10;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

FrameList frames;
int frame_n = 0;

//settings
bool play = false;
bool show_blanks = false;
bool show_points = false;

// point scanning display
bool show_scan = false;
int show_scan_n_points = 0;  // -1 indicates "show all"
bool animate_scan = false;

static inline void next_frame()
{
    frame_n++;
    frame_n = (frame_n >= static_cast<int>(frames.size())) ? 0 : frame_n;
}

static inline void prev_frame()
{
    frame_n--;
    frame_n = (frame_n < 0) ? frames.size() - 1 : frame_n;
}

static inline void show_next_point()
{
    show_scan_n_points++;
    if (show_scan_n_points >= static_cast<int>(frames[frame_n].size()))
    {
        show_scan_n_points = 0;
    }
}

static inline void hide_prev_point()
{
    show_scan_n_points--;
    if (show_scan_n_points < 0)
    {
        show_scan_n_points = frames[frame_n].size() - 1;
    }
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


static void render_line(const Point& p1, const Point& p2)
{
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

static bool should_render(int i) {
    return play || (show_scan ? i < show_scan_n_points : true);
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
    for(int i = 0; (i < ((int)frame.size() - 1)) && should_render(i+1); i++)
    {
        render_line(frame[i], frame[i+1]);
    }

    //draw points overtop of the lines
    if(show_points)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for(size_t i = 0; (i < frame.size()) && should_render(i); i++)
        {
            const Point& p = frame[i];
            SDL_Rect r = lzr_point_to_sdl_rect(p);
            SDL_RenderFillRect(renderer, &r);
        }
    }

    SDL_RenderPresent(renderer);
}

static void loop()
{
    SDL_Event e;
    while(true)
    {
        if(!(play || animate_scan))
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
                    return;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            return;
                        case SDLK_SPACE:
                            play = !play;
                            show_scan = false;
                            animate_scan = false;
                            break;
                        case SDLK_b:
                            show_blanks = !show_blanks;
                            break;
                        case SDLK_p:
                            show_points = !show_points;
                            break;
                        case SDLK_s:
                            play = false;
                            show_scan = !show_scan;
                            animate_scan = !animate_scan;
                            break;
                        case SDLK_LEFT:
                            // Disable frame and point scroll when we're doing any sort of animation
                            if (animate_scan || play) break;
                            if (!show_scan) prev_frame();
                            else if (show_scan) hide_prev_point();
                            break;
                        case SDLK_RIGHT:
                            // Disable frame and point scroll when we're doing any sort of animation
                            if (animate_scan || play) break;
                            if (!show_scan) next_frame();
                            else if (show_scan) show_next_point();
                            break;
                        case SDLK_RSHIFT:
                        case SDLK_LSHIFT:
                            play = false;
                            show_scan = true;
                            animate_scan = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_RSHIFT:
                        case SDLK_LSHIFT:
                            show_scan = false;
                            animate_scan = false;
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    // Disable frame and point scroll when we're doing any sort of animation
                    if (animate_scan || play) break;
                    if(!show_scan)
                    {
                        if(e.wheel.y > 0) next_frame();
                        else if(e.wheel.y < 0) prev_frame();
                    }
                    else if(show_scan)
                    {
                        if(e.wheel.y > 0) show_next_point();
                        else if(e.wheel.y < 0) hide_prev_point();
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
            SDL_Delay(play_period_ms);
        }
        else if (animate_scan)
        {
            show_next_point();
            // SDL_Delay(animate_scan_period_ms);
        }

    }
}

void keybindings()
{
    printf("Nav Keys:\n");
    printf("    [esc]                      Quit\n");
    printf("    [space]                    Play/pause animation\n");
    printf("    [left]/[right]/[scroll]    Previous/next frame. Mouse scroll Only when animation is paused\n");
    printf("    [shift] (HOLD)             Press and hold to enter scan pattern observation mode. Will pause animation.\n");
    printf("                               Use [left]/[right]/[scroll] to hide/reveal more points.\n");
    printf("Display Keys:\n");
    printf("    s    Toggle animated scan pattern. Will pause animation.\n");
    printf("    b    Toggle visibility of blanked lines\n");
    printf("    p    Toggle point markers\n");
}

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        fprintf(stderr, "Usage: ilda-view <ilda file>\n\n");
        keybindings();
        return EXIT_SUCCESS;
    }

    // load the ILDA file
    ILDA* f = ilda_open(argv[1], "r");

    if (!f) {
        fprintf(stderr, "Failed to read ILDA file\n");        
        return EXIT_FAILURE;
    }

    printf("Found %zu projector(s)\n", ilda_projector_count(f));
    printf("Found %zu frames for projector 0\n", ilda_frame_count(f, 0));

    keybindings();

    if(ilda_read(f, 0, frames) != LZR_SUCCESS)
    {
        fprintf(stderr, "Error loading frames\n");
        return EXIT_FAILURE;
    }

    ilda_close(f);

    // Simulate any return-to-home lines within single frames by adding
    // jumps if the laser isn't already at the beginning.
    for (Frame& frame : frames)
    {
        if (!frame.front().same_position_as(frame.back()))
        {
            frame.add(frame.front());
        }
    }

    // Only consider this an animation if there are multiple frames
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
