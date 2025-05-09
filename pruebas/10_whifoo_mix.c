// 10_whifoo_mix.c
// WHILE/IF/FOR mezclados
main() {
    int i = 0;
    while (i < 2) {
        for (j = 0; j < 2; j = j + 1) {
            if (j == 1) {
                printf("WIF mix: %d,%d\n", i, j);
            }
        }
        i = i + 1;
    }
}
//@ (main)
