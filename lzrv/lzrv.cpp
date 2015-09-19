
#include <pthread.h>
#include <string>
#include <iostream>
#include <SDL.h>
#include <lzr.h>

#define DEFAULT_WINDOW_SIZE 500


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Uint32 NEW_FRAME; //SDL event for new lzr frames

void* zmq_ctx;
pthread_t zmq_thread;

pthread_mutex_t frame_lock = PTHREAD_MUTEX_INITIALIZER;
lzr_frame* frame;


static void trigger_new_frame()
{
    SDL_Event e;
    SDL_zero(e);

    e.type = NEW_FRAME;
    e.user.data1 = NULL; //don't bother with data
    e.user.data2 = NULL;

    //try to push onto SDL event queue
    int r = SDL_PushEvent(&e);

    if(r < 0)
    {
        std::cerr << "SDL_PushEvent() failed" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
    }
    else if(r == 0)
    {
        std::cerr << "SDL_PushEvent() was filtered" << std::endl;
    }
}

//ZMQ recv thread
static void* loop_recv(void*)
{
    void* rx = lzr_create_frame_rx(zmq_ctx, LZR_ZMQ_ENDPOINT);
    lzr_frame* temp_frame = new lzr_frame;

    while(1)
    {
        int r = lzr_recv_frame(rx, temp_frame);

        if(r > 0)
        {
            pthread_mutex_lock(&frame_lock);
            *frame = *temp_frame;
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

    zmq_close(rx);
    delete temp_frame;

    return NULL;
}

static inline int lzr_coord_to_screen(double v, bool invert)
{
    v = invert ? (v * -1.0) : v;
    return (int) (((v + 1.0) / 2.0) * DEFAULT_WINDOW_SIZE);
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

    //begin drawing the current frame
    pthread_mutex_lock(&frame_lock);

    //NOTE: cast to int to avoid rollover problems with -1
    for(int i = 0; i < (frame->n_points - 1); i++)
    {
        lzr_point p1 = frame->points[i];
        lzr_point p2 = frame->points[i+1];

        //TODO: double check that color is actually
        //      a property of the first point
        SDL_SetRenderDrawColor(renderer,
                               p1.r,
                               p1.g,
                               p1.b,
                               !LZR_POINT_IS_BLANKED(p1) * 255);

        SDL_RenderDrawLine(renderer,
                           lzr_coord_to_screen(p1.x, false),
                           lzr_coord_to_screen(p1.y, true),
                           lzr_coord_to_screen(p2.x, false),
                           lzr_coord_to_screen(p2.y, true));
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
        std::cout << "loop" << std::endl;
        SDL_WaitEvent(NULL);

        //event pump
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                //dispatch SDL events to their respective handlers
                case SDL_QUIT:        running = false; break;
                case SDL_WINDOWEVENT: break;
                case SDL_KEYDOWN:     break;
                default:
                    if(e.type == NEW_FRAME)
                    {
                        //draw the new lzr_frame
                        render();
                    }
            }
        }

        //cap at ~30 fps
        SDL_Delay(33);
    }
}


int main()
{
    zmq_ctx = zmq_ctx_new();
    frame = new lzr_frame;

    //start SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
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
        std::cerr << "SDL_CreateWindow Error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        goto err_window;
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        std::cerr << "SDL_CreateRenderer Error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        goto err_render;
    }

    NEW_FRAME = SDL_RegisterEvents(1);
    if(NEW_FRAME == ((Uint32) -1 ))
    {
        std::cerr << "Failed to register custom events" << std::endl;
        goto err_events;
    }


    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // clear the screen to black
    frame->n_points = 0;
    render();


    //start a ZMQ worker thread
    if(pthread_create(&zmq_thread, NULL, loop_recv, NULL))
    {
        std::cerr << "Failed to start ZMQ thread" << std::endl;
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

    delete frame;

    return 0;
}
