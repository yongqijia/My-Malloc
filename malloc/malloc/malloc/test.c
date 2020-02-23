//
//  test.c
//  malloc
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

#define MALLOC my_firstfit_malloc

#define FREE my_free

void test1(){
    void* a = MALLOC(10);
    void* b = MALLOC(70);
    FREE(a);
    FREE(b);
    void* c = MALLOC(40);
    FREE(c);
}

void test2(){
    void* a = MALLOC(150);
    FREE(a);
    void* b = MALLOC(200);
    void* c = MALLOC(75);
    void* d = MALLOC(25);
    void* e = MALLOC(100);
    FREE(b);
    FREE(c);
    FREE(d);
    FREE(e);
}

int main(){
    printf("brk before  test1: %p\n", sbrk(0));
    test1();
    printf("brk after   test1: %p\n", sbrk(0));
    printf("\n");
    printf("brk before  test2: %p\n", sbrk(0));
    test2();
    printf("brk after   test2: %p\n", sbrk(0));
    return 0;

}
