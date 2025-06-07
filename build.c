#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

int main(int argc, char **argv)
{
    rebuild_yourself(__FILE__, argv);

    Cmd cmd = {0};
    cmd_append(&cmd, "cc");
    cmd_append(&cmd, "main.c");
    cmd_append(&cmd, "-o", "exe");
    cmd_append(&cmd, "-Wextra", "-Wall");
    cmd_append(&cmd, "-g");
    cmd_append(&cmd, "-O3");

    cmd_run_async(&cmd);

    return 0;
}
