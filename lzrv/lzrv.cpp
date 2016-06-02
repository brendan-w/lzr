
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <liblzr.h>

using namespace lzr;


#define DEFAULT_WINDOW_SIZE 500
#define POINT_SIZE 3


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Uint32 NEW_FRAME; //SDL event for new lzr frames

void* zmq_ctx;
pthread_t zmq_thread;

pthread_mutex_t frame_lock = PTHREAD_MUTEX_INITIALIZER;
Frame frame;

//settings
bool show_blanks = false;
bool show_points = false;


static void trigger_new_frame()
{
    SDL_Event e;
    SDL_zero(e);
    e.type = NEW_FRAME;
    e.user.data1 = NULL; //don't bother with data
    e.user.data2 = NULL;

    //flush any existing frame events, so they don't pile up
    SDL_FlushEvent(NEW_FRAME);

    //try to push onto SDL event queue
    int r = SDL_PushEvent(&e);

    if(r < 0)
    {
        fprintf(stderr, "SDL_PushEvent() failed\n%s\n", SDL_GetError());
    }
    else if(r == 0)
    {
        fprintf(stderr, "SDL_PushEvent() was filtered\n");
    }
}

//ZMQ recv thread
static void* zmq_loop(void* data)
{
    (void)data; //param is required, but not used

    void* zmq_sub = frame_sub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);
    Frame temp_frame;

    while(1)
    {
        int r = recv_frame(zmq_sub, temp_frame);
        std::cout << "recv frame: " << temp_frame.size() << std::endl;

        if(r > 0)
        {
            pthread_mutex_lock(&frame_lock);
            frame = temp_frame;
            pthread_mutex_unlock(&frame_lock);
            trigger_new_frame();
        }
        else if((r == -1) && (errno == ETERM))
        {
            //zmq_ctx has been terminated, stop looping,
            //close the socket, and prepare for the join()
            break;
        }
    }

    zmq_close(zmq_sub);

    return NULL;
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

    //begin processing the current frame
    pthread_mutex_lock(&frame_lock);

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

    pthread_mutex_unlock(&frame_lock);

    SDL_RenderPresent(renderer);
}

static void loop()
{
    SDL_Event e;
    bool running = true;

    while(running)
    {
        SDL_WaitEvent(NULL);

        //event pump

        bool do_render = false;

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
                        case SDLK_b:
                            show_blanks = !show_blanks;
                            do_render = true;
                            break;
                        case SDLK_p:
                            show_points = !show_points;
                            do_render = true;
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                        if((e.window.event == SDL_WINDOWEVENT_RESIZED) ||
                           (e.window.event == SDL_WINDOWEVENT_MAXIMIZED) ||
                           (e.window.event == SDL_WINDOWEVENT_RESTORED))
                            do_render = true;
                        break;
                default:
                    if(e.type == NEW_FRAME)
                        do_render = true;
            }
        }

        if(do_render)
            render();

        //cap at ~30 fps
        SDL_Delay(33);
    }
}


int main()
{
    zmq_ctx = zmq_ctx_new();


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

    NEW_FRAME = SDL_RegisterEvents(1);
    if(NEW_FRAME == ((Uint32) -1 ))
    {
        fprintf(stderr, "Failed to register custom events\n");
        goto err_events;
    }


    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // clear the screen to black
    render();


    //start a ZMQ worker thread
    if(pthread_create(&zmq_thread, NULL, zmq_loop, NULL))
    {
        fprintf(stderr, "Failed to start ZMQ thread\n");
        goto err_thread;
    }

    //start the main loop
    loop();

    //shut off all zmq activites. This will free up any blocking recv() calls
    zmq_ctx_term(zmq_ctx);

    //join the ZMQ reading thread
    pthread_join(zmq_thread, NULL);


    //cleanup
err_events:
err_thread:
    SDL_DestroyRenderer(renderer);
err_render:
    SDL_DestroyWindow(window);
err_window:
    SDL_Quit();


    return 0;
}
