# Memory Allocation

This repository implements basic memory allocation methods, including first-fit and best-fit.

## Description

### Memory Pool

The data structure of the memory pool is like a doubly linked list. A memory chunck includes metadata and the actual data. Metadata save information of the data block, including size, status, and also the pointers to the next and previous memory chunck.

![image](https://user-images.githubusercontent.com/80531783/212006599-f4e92d7a-613a-4762-beed-316f06a7b9c1.png)

### Allocation

The are several techniques for memory allocation, here we introduce two simple algorithms:

#### First-fit

First-fit find the first memory chunck that fits

![image](https://user-images.githubusercontent.com/80531783/212007305-e754e78a-b81c-40e0-a5cb-4f42bdf62dd6.png)

#### Best-fit

Best-fit find the smallest memory chunck that fit the requested memory size.

![image](https://user-images.githubusercontent.com/80531783/212007395-62c599d7-6a0c-44aa-86d1-7241b2848d47.png)

### Free

Assume we want to free the allocated memory surrounded by the red block, the memory pool before freeing is like:

![image](https://user-images.githubusercontent.com/80531783/212007784-cb567e92-f969-4209-886f-df8010066087.png)

After we free the memory, it was merged with the neighboring free memory chunck.

![image](https://user-images.githubusercontent.com/80531783/212007870-f672275f-7cc0-45cc-94a4-e030e21e311c.png)

## Getting started

The main program simulate a series of allocation and free operations. `malloc()` and `free()` of the GNU glibc implementation is replaced with the self-implemented memory allocation algorithms.

### Executing program

```shell
$ make
$ LD_PRELOAD=./ff.so ./main # first fit
# result: Max Free Chunk Size = 960
$ LD_PRELOAD=./bf.so ./main # best fit
# result: Max Free Chunk Size = 416

```
