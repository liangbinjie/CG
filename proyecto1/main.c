#include "utils/utils.h"
#include "utils/mesa.h"
#include "utils/interface.h"
#include "utils/coordinates.h"

// ===== FUNCTIONS ======

void draw_scene(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    draw_map();
    
    glutSwapBuffers();
};


int main(int argc, char **argv)
{
    init_interface(argc, argv);

    glutDisplayFunc(draw_scene);

    glutMainLoop();
    
    return 0;
}
