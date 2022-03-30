#include <stdio.h>
#define N 100000
static int a[N], b[N], c[N];

void foo() {
    for(int i = 0; i < N; i++){
        a[i] = 0; b[i] = i;
    }
}
int main(){
    foo();
    for(int i = 0; i < N; i++){
        c[i] = a[i] * b[i];
    }
    return 0;
}