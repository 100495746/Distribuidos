// 11_nested_ifelse.c
// If anidado
classify(int x) {
    if (x < 0) {
        if (x % 2 == 0){ return -2;}
        else {return -1;}
    } else {
        if (x % 2 == 0) {return 2;}
        else {return 1;}
    }
}

main() {
    printf("%d %d %d %d\n",
        classify(-3), classify(-4),
        classify(5), classify(6)
    );
}
//@ (main)
