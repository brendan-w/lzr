
#include <string>
#include <iostream>
#include <SDL.h>
#include <lzr.h>

#define DEFAULT_WINDOW_SIZE 500


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void* zmq_ctx = NULL;
void* rx = NULL;

lzr_frame* frame;



static inline int lzr_coord_to_screen(double v)
{
    return (int) (((v + 1.0) / 2.0) * DEFAULT_WINDOW_SIZE);
}

static void draw_frame()
{
    // clear the screen to black
    SDL_SetRenderDrawColor(renderer,
                           0,
                           0,
                           0,
                           255);
    SDL_RenderClear(renderer);

    for(size_t i = 0; i < (frame->n_points - 1); i++)
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
                           lzr_coord_to_screen(p1.x),
                           lzr_coord_to_screen(p1.y),
                           lzr_coord_to_screen(p2.x),
                           lzr_coord_to_screen(p2.y));
    }
}

static void loop()
{
    SDL_Event e;
    bool running = true;

    while(running)
    {
        // SDL_WaitEvent(NULL);

        //event pump
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                //dispatch SDL events to their respective handlers
                case SDL_QUIT:        running = false; break;
                case SDL_WINDOWEVENT: break;
                case SDL_KEYDOWN:     break;
            }
        }

        //TODO: this sucks, and is temporary
        int r = lzr_recv_frame_no_block(rx, frame);
        // std::cout << r << std::endl;


        if(r > 0)
        {
            draw_frame();
        }

        //render changes
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
}


int main(int argc, char * argv[])
{
    frame   = new lzr_frame;
    zmq_ctx = lzr_create_zmq_ctx();
    rx      = lzr_create_frame_rx(zmq_ctx, LZR_ZMQ_ENDPOINT);

    //start SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error" << std::endl;
        std::cout << SDL_GetError() << std::endl;
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
        std::cout << "SDL_CreateWindow Error" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        goto err_window;
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        std::cout << "SDL_CreateRenderer Error" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        goto err_render;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // clear the screen to black
    SDL_SetRenderDrawColor(renderer,
                           0,
                           0,
                           0,
                           255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);



    /*
        start the main loop
    */
    loop();


    //cleanup
    SDL_DestroyRenderer(renderer);
err_render:
    SDL_DestroyWindow(window);
err_window:
    SDL_Quit();

    lzr_destroy_frame_rx(rx);
    lzr_destroy_zmq_ctx(zmq_ctx);
    delete frame;

    return 0;
}
