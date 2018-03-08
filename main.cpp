#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "initialise.h"
#include <stdio.h>

void displayCommands()
{
    printf("Commands:\n"
           "    Left-clicking and drag to move camera\n"
           "    Right-click to enable object action\n"
           "    Arrow buttons to walk around map\n"
           "    Space to jump\n"
           );
}

int main(int argc,char **argv)
{
    initGlut(argc, argv);
    return 0;
}




