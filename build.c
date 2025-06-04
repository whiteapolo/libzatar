#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(int argc, char **argv)
{
    z_rebuild_yourself(__FILE__, argv);

    z_run_async("cc", "main.c", "-o", "exe", "-Wextra", "-Wall", "-g");

    return 0;
}
