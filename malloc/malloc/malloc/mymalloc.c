//
//  mymalloc.c
//  malloc
//  author: Yongqi Jia

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mymalloc.h"

struct node {
    int chunk_size;

    // 1 is in use, 0 is free
    int in_use;
    
    struct node *next;
    struct node *prev;
};

struct node *_head = NULL;
struct node *_tail = NULL;

void *add(struct node *head, int size){
    while(_head->next != NULL){
        if (_head->chunk_size >= size && _head->in_use == 0) {
            struct node *free_node;
            int remain_size = _head->chunk_size - size;
            if (remain_size >= sizeof(struct node)) {
                free_node = (void *)_head + sizeof(struct node) + size;
                free_node->chunk_size = remain_size - sizeof(struct node);
                free_node->chunk_size = size;
                free_node->next = _head->next;
                free_node->prev = _head;
                free_node->in_use = 0;
                _head->next = free_node;
            }
            _head->in_use = 1;
            return (void *)_head + sizeof(struct node);
            
        }else {
            _head = _head->next;
        }
    }
    return sbrk(0);
}

void *my_firstfit_malloc(int size) {
    if (_head == NULL) {
        _head = sbrk(sizeof(struct node));
        _head->chunk_size = size;
        _head->next = NULL;
        _head->prev = NULL;
        _tail = _head;
        _head->in_use = 1;
        return sbrk(size);
    }
    else {
        add(_head, size);
        struct node *new_node = sbrk(sizeof(struct node));
        new_node->in_use = 1;
        new_node->chunk_size = size;
        new_node->prev = _tail;
        new_node->next = NULL;
        _tail = new_node;
        return sbrk(size);
    }
}

void my_free(void *ptr) {
    struct node *freed_node = ptr - sizeof(struct node);
    freed_node->in_use = 0;
    if (freed_node->next != NULL && freed_node->next->in_use == 0) {
        int size = freed_node->chunk_size + freed_node->next->chunk_size + sizeof(struct node);
        freed_node->chunk_size = size;
        freed_node->next = freed_node->next->next;
        freed_node->next->prev = freed_node;
        
    }
    if (freed_node->prev != NULL && freed_node->prev->in_use == 0) {
        int size = freed_node->chunk_size + freed_node->prev->chunk_size + sizeof(struct node);
        freed_node->prev->chunk_size = size;
        freed_node->prev->next = freed_node->next;
        if (freed_node->next != NULL) {
            freed_node->next->prev = freed_node->prev;
        }else{
            _tail = freed_node->prev;
        }
        freed_node = freed_node->prev;
    }
    if (freed_node == _tail) {
        sbrk((void *)freed_node - sbrk(0));
        if (freed_node == _head) {
            _head = NULL;
            _tail = NULL;
        }else {
            struct node *temp = freed_node->prev;
            _tail = temp;
            temp->next = NULL;
        }
        
    }
}
