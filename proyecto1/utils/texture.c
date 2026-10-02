#include "texture.h"

// TEXTURE[x%TH][y%TV] = TEXEL

// Load .avs file into a matrix
// Input: filename
// Output: returns a texel struct matrix
TEXEL **load_avs(const char *filename) {
    int height, width;
    FILE *file = fopen(filename, "rb"); // read file as binaries

    if (file == NULL) {
        printf("No file found.\n");
        return NULL;
    }

    // define a 4 bytes buffer to read the header
    unsigned char buffer[4];

    // read first 4 bytes of the header -> width
    fread(buffer, 1, 4, file);

    // arrange big endian bytes to little endian
    width = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // read the follwing 4 bytes -> height
    fread(buffer, 1, 4, file);

    // arrange header big endian bytes to little endian
    height = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // this initializeas a malloc of x height of the image, with the size of the pixel
    TEXEL **image = malloc(height *sizeof(TEXEL *));

    // @TODO: check for errors that might occur with malloc

    for (int y=0; y < height; y++) {
        // for every y in height, create row
        image[y] = malloc(width * sizeof(TEXEL));
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