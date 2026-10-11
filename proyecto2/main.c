#include "utils/utils.h"
#include "utils/mesa.h"
#include "utils/interface.h"
#include "config/config.h"
#include <stdio.h>

// ===== FUNCTIONS ======

void draw_scene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glutSwapBuffers();
};


int main(int argc, char **argv) {

    Config config;
    load_config(&config);
    
    init_interface(argc, argv);

    glutDisplayFunc(draw_scene);

    glutMainLoop();
    
    return 0;
}
