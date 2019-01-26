int put_char(char c) {
    c;
    interrupt 0;
}
int put_string(char *text) {
    while (put_char(*text++));
}
int put_int(int number) {
    number;
    interrupt 1;
}
int put_hex(int number) {
    number;
    interrupt 2;
}
char *malloc(int size) {
    size;
    interrupt 30;
}
char *malloc_log(int size) {
    put_string("[x] MALLOC: ");
    char *ptr = malloc(size);
    put_hex((int) ptr);
    put_string(", SIZE: ");
    put_int(size);
    put_string("\n");
    return ptr;
}
int free(int addr) {
    addr;
    interrupt 31;
}
int free_log(int addr) {
    put_string("[x] FREE:   ");
    put_hex(addr);
    int size = free(addr);
    put_string(", SIZE: ");
    put_int(size);
    put_string("\n");
    return size;
}
int case_1() {
    put_string("-- CASE #1 --\n");
    int i, n = 10;
    int *ptrs = (int *) malloc_log(sizeof(int *) * n);
    for (i = 0; i < n; ++i) ptrs[i] = malloc_log(i << 4);
    for (i = 0; i < n; ++i) free_log(ptrs[i]);
    for (i = 0; i < n; ++i) ptrs[i] = malloc_log(i << 4);
    for (i = 0; i < n; ++i) free_log(ptrs[i]);
    free_log((int) ptrs);
}
int case_2() {
    put_string("-- CASE #2 --\n");
    int i, n = 10;
    int *ptrs = (int *) malloc_log(sizeof(int *) * 10);
    for (i = 0; i < n; ++i) ptrs[i] = malloc_log(i << 8);
    for (i = n - 1; i >= 0; --i) free_log(ptrs[i]);
    for (i = 0; i < n; ++i) ptrs[i] = malloc_log(i << 8);
    for (i = 0; i < n; ++i) free_log(ptrs[i]);
    free_log((int) ptrs);
}
int main(int argc, char **argv) {
    int i;
    put_string("========== [#5 TEST MALLOC] ==========\n");
    case_1();
    case_2();
    put_string("========== [#5 TEST MALLOC] ==========\n");
    return 0;
}