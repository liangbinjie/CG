#include "utils/utils.h"
#include "utils/mesa.h"

#include <stdio.h>
#include <stdlib.h>

// ====== GLOBAL VARIABLES ======
//
//
int resolution;
//
//
// ====== END GLOBAL VARIABLES ======


// ===== FUNCTIONS ======
void draw_scene(void);


int main(int argc, char **argv)
{
    resolution = 1920;

    init_interface(argc, argv);

    glutDisplayFunc(draw_scene);

    glutMainLoop();

    return 0;
}