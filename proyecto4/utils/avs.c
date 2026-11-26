#include "avs.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

static int read_uint32_be(FILE *file, uint32_t *value) {
    unsigned char bytes[4];
    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes)) return -1;
    *value = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
             ((uint32_t)bytes[2] << 8) | (uint32_t)bytes[3];
    return 0;
}

int load_texture_avs(const char *filename, int id, Texture *texture) {
    if (!filename || !texture || id <= 0) return -1;
    memset(texture, 0, sizeof(*texture));
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    uint32_t width, height;
    if (read_uint32_be(file, &width) || read_uint32_be(file, &height) ||
        width == 0 || height == 0 || width > INT_MAX || height > INT_MAX ||
        (size_t)width > SIZE_MAX / (size_t)height / sizeof(TexturePixel)) {
        fclose(file);
        return -1;
    }
    size_t count = (size_t)width * (size_t)height;
    TexturePixel *pixels = malloc(count * sizeof(*pixels));
    if (!pixels) { fclose(file); return -1; }
    for (size_t i = 0; i < count; i++) {
        unsigned char rgba[4];
        if (fread(rgba, 1, sizeof(rgba), file) != sizeof(rgba)) {
            free(pixels); fclose(file); return -1;
        }
        pixels[i] = (TexturePixel){rgba[0], rgba[1], rgba[2], rgba[3]};
    }
    if (fclose(file) != 0) { free(pixels); return -1; }
    texture->id = id;
    texture->width = (int)width;
    texture->height = (int)height;
    texture->pixels = pixels;
    return 0;
}

void free_texture(Texture *texture) {
    if (!texture) return;
    free(texture->pixels);
    memset(texture, 0, sizeof(*texture));
}

static int write_uint32_be(FILE *file, unsigned int value) {
    unsigned char bytes[4];

    bytes[0] = (unsigned char)((value >> 24) & 0xff);
    bytes[1] = (unsigned char)((value >> 16) & 0xff);
    bytes[2] = (unsigned char)((value >> 8) & 0xff);
    bytes[3] = (unsigned char)(value & 0xff);

    return fwrite(bytes, sizeof(bytes), 1, file) == 1 ? 0 : -1;
}

int save_avs(const char *filename, Color **framebuffer, int width, int height) {
    FILE *file;
    unsigned char pixel[4];
    int x;
    int y;

    if (filename == NULL || framebuffer == NULL || width <= 0 || height <= 0) {
        return -1;
    }

    file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }

    if (write_uint32_be(file, (unsigned int)width) != 0 ||
        write_uint32_be(file, (unsigned int)height) != 0) {
        fclose(file);
        return -1;
    }

    for (y = 0; y < height; y++) {
        if (framebuffer[y] == NULL) {
            fclose(file);
            return -1;
        }

        for (x = 0; x < width; x++) {
            pixel[0] = framebuffer[y][x].a;
            pixel[1] = (unsigned char)(framebuffer[y][x].r * 255.0L);
            pixel[2] = (unsigned char)(framebuffer[y][x].g * 255.0L);
            pixel[3] = (unsigned char)(framebuffer[y][x].b * 255.0L);

            if (fwrite(pixel, sizeof(pixel), 1, file) != 1) {
                fclose(file);
                return -1;
            }
        }
    }

    if (fclose(file) != 0) {
        return -1;
    }

    return 0;
}

static int run_image_viewer(const char *program, const char *first_argument,
                            const char *filename) {
    pid_t child = fork();
    int status;

    if (child < 0) {
        return -1;
    }

    if (child == 0) {
        if (first_argument == NULL) {
            execlp(program, program, "-geometry", "+0+300", filename,
                   (char *)NULL);
        } else {
            execlp(program, program, first_argument, "-geometry", "+0+300",
                   filename, (char *)NULL);
        }
        _exit(127);
    }

    if (waitpid(child, &status, 0) < 0) {
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return -1;
}

int display_avs(const char *filename) {
    int result;

    if (filename == NULL) {
        return -1;
    }

    result = run_image_viewer("display", NULL, filename);
    if (result == 127) {
        result = run_image_viewer("magick", "display", filename);
    }

    return result;
}
