// 18_vector_elem_assign.c
// Asignación a elemento de vector
int arr[4];

main() {
    arr[2] = 99;
    printf("arr[2]: %d\n", arr[2]);
    arr[2] = arr[2] + 1;
    printf("arr[2]++: %d\n", arr[2]);
}
//@ (main)
