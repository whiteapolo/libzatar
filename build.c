#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(int argc, char **argv)
{
    z_rebuild_yourself(__FILE__, "./build");

    z_run_async("cc", "main.c", "-o", "exe", "-Wextra", "-Wall");

    return 0;
}
