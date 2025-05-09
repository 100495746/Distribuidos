// 06_params_multi.c
// Funciones con múltiples parámetros
int mix4(int a, int b, int c, int d) {
    return a + b * c - d;
}

int use_mix(int x) {
    return mix4(x, x+1, x+2, x+3);
}

main() {
    printf("%d\n", mix4(1,2,3,4));
    printf("%d\n", use_mix(5));
}
//@ (main)
