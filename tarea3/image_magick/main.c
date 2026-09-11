// @TODO
//
// 1. Cargar a una matriz rectangular un archivo .avs
// 2. Grabar un archivo .avs desde memoria
//
// # NOTAs
// Los archivos .AVS se leen en formato de Big Endian
// para arquietcturas Little Endian hay que cambiarlo

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int width;
int height;

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    int width;
    int height;
    Pixel **pixels;
} Image;

// Load .avs file into a matrix
// InputL: filename, width, height
// Output: Pixel struct matrix
Pixel **load_avs(const char *filename) {
    FILE *file = fopen(filename, "rb"); // read file as binaries

    if (file == NULL) {
        printf("No file found.\n");
        return NULL;
    }

    // // Leer width y height
    // fread(&width, sizeof(int), 1, file);
    // fread(&height, sizeof(int), 1, file);

    // Pixel **image = malloc(height * sizeof(Pixel *));

    // for (int y = 0; y < height; y++) {
    //     image[y] = malloc(width * sizeof(Pixel));

    //     for (int x = 0; x < width; x++) {
    //         fread(&image[y][x], sizeof(Pixel), 1, file);
    //     }
    // }

    fclose(file);
    return image;
}


int main(int argc, char *argv[]) {

    Pixel **image = load_avs("image.avs");

    return 0;
}  