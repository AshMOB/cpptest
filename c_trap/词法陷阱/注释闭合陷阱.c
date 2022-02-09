#include<stdio.h>
int main(){
    // printf("hello world\n");
    float y,x;
    float *p;
    float t=2.0;
    x=1;
    p=&t;
    y=x/(*p);
    printf("%f\n",y);
    return 0;
}