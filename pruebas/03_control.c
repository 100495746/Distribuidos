// 03_control.c
// Estructuras de control
main() {
    int i = 0;
    if (i == 0) puts("Inicio correcto");
    while (i < 3) {
        printf("while: i = %d\n", i);
        i = i + 1;
    }
    for (i = 0; i < 3; i = i + 1) {
        printf("for: i = %d\n", i);
    }
}
//@ (main)
