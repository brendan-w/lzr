
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "lzr/lzr.hpp"

using namespace lzr;

int main(int argc, char* argv[])
{
    init_dacs();

    sleep(2);

    if (argc != 3)
    {
        // list help and DAC list only
        printf("Usage: %s <dac-name> <ilda-file>\n\n", argv[0]);
        printf("Available DACs:\n");

        DACNames dacs = list_dacs();

        for(std::string dac : dacs)
        {
            printf("    %s\n", dac.c_str());
        }

        if (dacs.size() == 0)
        {
            printf("no DACs found\n");
            return EXIT_FAILURE;
        }
    }
    else if (argc == 3)
    {
        // connect to the requested DAC
        DAC* dac = dac_connect(argv[1]);
        dac->pps(30000);

        // load the ILDA file
        ILDA* f = ilda_open(argv[2], "r");

        FrameList frames;
        if(ilda_read(f, 0, frames) != LZR_SUCCESS)
        {
            fprintf(stderr, "Error loading frames\n");
            return EXIT_FAILURE;
        }

        ilda_close(f);

        for (Frame& frame : frames)
        {
            dac->send(frame);
            sleep(100);
            // usleep(30*1000);
        }
    }

    return EXIT_SUCCESS;
}
