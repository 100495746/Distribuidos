// 20_directive_embed.c
// Prueba de //@ embed
int f1(int x) { return x*x; }

//@ (print (f1 4))
//@ (print (f1 7))

main() {
    printf("f1(4)=%d\n", f1(4));
    printf("f1(7)=%d\n", f1(7));
}
//@ (main)
