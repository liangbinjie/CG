#include <stdio.h>
#include <string.h>
#include "config.h"

int load_config(Config *config) {
    FILE *file = fopen(CONFIG_FILE, "r");
    char line[100];

    if (file == NULL) {
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "width=%d", &config->width);
        sscanf(line, "height=%d", &config->height);
        sscanf(line, "title=%99[^\n]", config->title); // Read title with spaces
    }

    fclose(file);
    return 0;
}