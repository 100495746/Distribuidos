// 01_funciones.c
// Funciones y retorno
int suma(int x, int y) {
    return x + y;
}

int factorial(int n) {
    if (n <= 1) return 1;
    else return n * factorial(n - 1);
}

main() {
    int rs = suma(3,7);
    int rf = factorial(5);
    printf("suma(3,7): %d\n", rs);
    printf("factorial(5): %d\n", rf);
}
//@ (main)
