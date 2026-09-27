#include "utils/utils.h"
#include "utils/mesa.h"
#include "utils/interface.h"
#include "utils/coordinates.h"

#include <stdio.h>
#include <stdlib.h>

// ===== FUNCTIONS ======

int mapX_toBuffer(double Xw) {
    return (int)(Hres * ((Xw - Xmin)/(Xmax - Xmin)));
}

int mapY_toBuffer(double Yw) {
    return (int)(Vres * ((Yw - Ymin)/(Ymax - Ymin)));
}

void draw_map() {
    setColor(1.0, 1.0, 1.0);

    glBegin(GL_POINTS);
    for (int i = 0; i < province_count; i++) {
        PROVINCE province = provinces[i];

        for (int j = 0; j < province.polygon_count; j++) {
            POLYGON polygon = province.polygons[j];
            
            for (int k = 0; k < polygon.size - 1; k++) {
                COORDINATES p0 = polygon.coordinates[k];
                COORDINATES p1 = polygon.coordinates[k + 1];
            
                // Apply rotation in universal coordinates
                p0 = rotate_point(p0);
                p1 = rotate_point(p1);
            
                int x0 = mapX_toBuffer(p0.x);
                int y0 = mapY_toBuffer(p0.y);

                int x1 = mapX_toBuffer(p1.x);
                int y1 = mapY_toBuffer(p1.y);

                draw_line(x0, y0, x1, y1);
            }
        }
    }
    glEnd();
}

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
