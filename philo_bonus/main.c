#include "philosophers_bonus.h"

sem_t *test;
void ft_child(int  i)
{
    sem_wait(test);
    printf("test %d\n", i);
    sem_post(test);
}


int main(int argc, char const *argv[])
{

    sem_close(test);
    sem_unlink("test");
    sem_open("test", O_CREAT, 0644, 1);
    int i = 0;
    while (i < 5)
    {
        int pid = fork();
        if (pid == 0)
            ft_child(i);
        i++;
    }
    
    return 0;
}
