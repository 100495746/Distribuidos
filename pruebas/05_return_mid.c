// 05_return_mid.c
// Return en punto medio
int f(int x) {
    if (x < 0) return -1;
    x = x * 2;
    return x;
}

main() {
    printf("%d\n", f(-5));
    printf("%d\n", f(3));
}
//@ (main)
