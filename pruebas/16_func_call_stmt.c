// 16_func_call_stmt.c
// Llamada como sentencia
int side_effect(int x) {
    printf("side_effect: %d\n", x);
    return x*2;
}

main() {
    side_effect(9);
    printf("Fin\n");
}
//@ (main)
