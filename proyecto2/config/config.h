#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE "config.cfg"

typedef struct {
    long width;
    long height;
    char title[100];
} Config;

int load_config(Config *config);

#endif // CONFIG_H