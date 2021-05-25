#include "../libs/types.h"
#include "../libs/stat.h"
#include "../libs/sysinfo.h"
#include "user.h"

int main()
{
    struct sysinfo info;
    if (sysinfo(&info) < 0) {
        printf("sysinfo fail!\n");
    } else {
        printf("memory left: %d KB\n", info.freemem >> 10);
        printf("process amount: %d\n", info.nproc);
    }
    exit(0);
}
