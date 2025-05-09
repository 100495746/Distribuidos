// 09_recursive_funcs.c
// Recursión mutua even/odd
int is_even(int n);

int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n-1);
}

int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n-1);
}

main() {
    printf("%d %d %d %d\n",
        is_even(4), is_odd(4),
        is_even(7), is_odd(7)
    );
}
//@ (main)
