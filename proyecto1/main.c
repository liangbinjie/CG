#include "utils/utils.h"
#include "utils/mesa.h"
#include "utils/interface.h"
#include "utils/coordinates.h"

#include <stdio.h>
#include <stdlib.h>


// ===== FUNCTIONS ======
void draw_scene(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
};


int main(int argc, char **argv)
{

    init_interface(argc, argv);

    glutDisplayFunc(draw_scene);

    for (int i=0; i < province_count; i++) {
        PROVINCE province = provinces[i];

        printf("%s\n", province.name);

    }

    glutMainLoop();



    return 0;
}