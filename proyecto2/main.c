#include "utils/utils.h"
#include "utils/interface.h"
#include "config/config.h"
#include <stdio.h>

// ===== FUNCTIONS ======

int main(int argc, char **argv) {

    Config config;
    load_config(&config);

    printf("Width: %d, Height: %d\n", config.width, config.height);
    
    return 0;
}
