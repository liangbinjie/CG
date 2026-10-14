#include "utils/utils.h"
#include "utils/interface.h"
#include "config/config.h"
#include <stdio.h>

int main(int argc, char **argv) {

    Config config;
    load_config(&config);

    return 0;
}
