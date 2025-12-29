#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

int* list;
int size;

void* greater(void* arg)
{
    int gcount = 0;

    for(int i = 0; i < size; i++)
    {
        if(list[i] >= 80)
            gcount++;
    }

    int* result = malloc(sizeof(int));
    *result = gcount;
    return result;
}

void* lesser(void* arg)
{
    int lcount = 0;

    for(int i = 0; i < size; i++)
    {
        if(list[i] < 80)
            lcount++;
    }

    int* result = malloc(sizeof(int));
    *result = lcount;
    return result;
}

void* counting(void* arg)
{
    int* result = malloc(sizeof(int));
    *result = size;
    return result;
}

int main(int argc, char* argv[])
{
    size = argc - 1;
    list = malloc(sizeof(int) * size);

    for(int i = 0; i < size; i++)
        list[i] = atoi(argv[i + 1]);

    int outfile = open("result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(outfile, 1);
    close(outfile);

    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, greater, NULL);
    pthread_create(&t2, NULL, lesser, NULL);
    pthread_create(&t3, NULL, counting, NULL);

    void* grcount;
    void* lesscount;
    void* totalcount;

    pthread_join(t1, &grcount);
    pthread_join(t2, &lesscount);
    pthread_join(t3, &totalcount);

    int* gc = (int*)grcount;
    int* lc = (int*)lesscount;
    int* tc = (int*)totalcount;

    printf("Numbers >= 80: %d\n", *gc);
    printf("Numbers < 80: %d\n", *lc);
    printf("Total numbers: %d\n", *tc);

    free(gc);
    free(lc);
    free(tc);
    free(list);

    return 0;
}
