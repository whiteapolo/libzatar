#define LIBZATAR_IMPLEMENTATION
#include "zatar.h"

int main(int argc, char **argv)
{
    z_rebuild_yourself(__FILE__, "./build");

    if (z_should_rebuild("main.o", "main.c")) {
        Z_Cmd cmd;
        z_cmd_init(&cmd);

        z_cmd_append(&cmd, "cc");
        // z_cmd_append(&cmd, "-o", "main.o");
        z_cmd_append(&cmd, "-c");
        z_cmd_append(&cmd, "main.c");
        z_cmd_append(&cmd, "-Wall", "-Wextra");

        z_cmd_run_async(&cmd);
    }


    z_run_async("cc", "main.o", "-o", "exe");


    return 0;
}
