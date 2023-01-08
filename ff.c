#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define POOL_SIZE 20000
#define META_SIZE 32

struct block {
    size_t size; // 8 bytes
    int free; // 4 bytes
    struct block *prev; // 8 bytes
    struct block *next; // 8 bytes
};

void *pool = NULL;
struct block *head;

void init() {
    pool = mmap(NULL, POOL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    head = (struct block *) pool;
    head->size = POOL_SIZE - META_SIZE;
    head->free = 1;
    head->prev = NULL;
    head->next = NULL;
}

void *deinit() {
    size_t max_size = 0;
    struct block *b = head;
    while (b) {
        if (b->free && b->size > max_size) max_size = b->size;
        b = b->next;
    }
    char buf[128];
    snprintf(buf, 32, "Max Free Chunk Size = %ld\n", max_size);
    write(1, buf, strlen(buf));
    munmap(pool, POOL_SIZE);
    return NULL;
}

void add(struct block *b, size_t size) {
    b->free = 0;
    if (b->size == size) return;

    struct block *newBlock = b + 1 + size / 32; // 1 is 32 bytes
    newBlock->size = b->size - size - META_SIZE;
    newBlock->free = 1;
    newBlock->next = b->next;
    newBlock->prev = b;
    if (newBlock->next) newBlock->next->prev = newBlock;
    if (newBlock->prev) newBlock->prev->next = newBlock;

    b->size = size;
}

void *ff_allocate(size_t size) {
    if (size % 32 != 0) size = size - size % 32 + 32;
    struct block *block = head;
    while (block) {
        if (block->free && block->size >= size) {
            add(block, size);
            return block + 1;
        }
        block = block->next;
    }
    return NULL;
}

void *malloc(size_t size) {
    if (!pool) init();
    if (!size) return deinit();
    return ff_allocate(size);
}

void free(void *ptr) {
    struct block *b = ptr;
    b = b - 1; // 1 is 32 bytes
    b->free = 1;
    if (b->next && b->next->free) {
        b->size += META_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next) b->next->prev = b;
    }
    if (b->prev && b->prev->free) {
        b->prev->size += META_SIZE + b->size;
        b->prev->next = b->next;
        if (b->next) b->next->prev = b->prev;
    }
}