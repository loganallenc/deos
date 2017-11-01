#include "atdlib.h"

int main(int argc, char const *argv[])
{   deostx *tx;
    tx = newdeostx();
    printf("%d\n", tx->i);
    deldeostx(tx);
    AES128_init();
    return EXIT_SUCCESS;
}
