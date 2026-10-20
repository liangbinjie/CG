#include "avs.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
            pixel[1] = framebuffer[y][x].r;
            pixel[2] = framebuffer[y][x].g;
            pixel[3] = framebuffer[y][x].b;

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
