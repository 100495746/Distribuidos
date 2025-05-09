// 17_array_expr.c
// Elemento de vector en expresión
int v[3];

main() {
    v[0]=2; v[1]=3; v[2]=5;
    int sum = v[0] + v[1] * v[2];
    printf("sum = %d\n", sum);
}
//@ (main)
