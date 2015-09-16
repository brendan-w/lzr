
#include <string>
#include <iostream>
#include <SDL.h>

#define DEFAULT_WINDOW_SIZE 500

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


static void loop()
{
    SDL_Event e;
    bool running = true;

    while(running)
    {
        SDL_WaitEvent(NULL);

        //event pump
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                //dispatch SDL events to their respective handlers
                case SDL_QUIT:            running = false; break;
                case SDL_WINDOWEVENT:     break;
                case SDL_KEYDOWN:         break;
            }
        }

        //render changes
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
}


int main(int argc, char * argv[])
{
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

    return 0;
}
