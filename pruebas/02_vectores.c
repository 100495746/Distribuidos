// 02_vectores.c
// Vectores
int numeros[5];

main() {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        numeros[i] = i * i;
    }
    for (i = 0; i < 5; i = i + 1) {
        printf("numeros[%d] = %d\n", i, numeros[i]);
    }
}
//@ (main)
