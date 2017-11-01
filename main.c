#include "atdlib.h"

int main(int argc, char const *argv[])
{   deostx *tx;
    tx = newdeostx();
    printf("%d\n", tx->i);
    deldeostx(tx);
    return EXIT_SUCCESS;
}
