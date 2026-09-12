// @TODO
//
// 1. Cargar a una matriz rectangular un archivo .avs
// 2. Grabar un archivo .avs desde memoria
//
// # NOTAS
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

    
    unsigned char buffer[4];

    // read first 4 bytes of the header -> width
    fread(buffer, 1, 4, file);

    // arrange big ndian bytes to little endian
    width = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // read the follwing 4 bytes -> height
    fread(buffer, 1, 4, file);

    // arrange big ndian bytes to little endian
    height = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // this initializeas a malloc of x height of the image, with the size of the pixel
    Pixel **image = malloc(height *sizeof(Pixel *));


    // @TODO: check for errors that might occur

    for (int y=0; y < height; y++) {
        // for every y in height, create row
        image[y] = malloc(width * sizeof(Pixel));
    }

    // fill the malloc with the image pixels data
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            unsigned char pixel[4];
            fread(pixel, 1, 4, file);

            image[y][x].a = pixel[0];
            image[y][x].r = pixel[1];
            image[y][x].g = pixel[2];
            image[y][x].b = pixel[3];
        }
    }


    fclose(file);
    return image;
}


int main(int argc, char *argv[]) {

    Pixel **image = load_avs("image.avs");

    // for (int y=0; y < height; y++) {
    //     for (int x=0; x < width; x++) {
    //         printf("[%d, %d, %d, %d], ",image[y][x].a,
    //             image[y][x].r,
    //             image[y][x].g,
    //             image[y][x].b);
    //     };
    // };

    printf("Width: %d x Height: %d\n", width, height);

    free(image);

    return 0;
}  