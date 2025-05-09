// 07_local_vars.c
// Variables locales y prefijado
int a = 100;

foo(int a, int b) {
    int c = a + b;
    a = c * 2;
    return a;
}

main() {
    printf("%d\n", foo(3,4)); // 14
    printf("%d\n", a);        // 100
}
//@ (main)
