/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 01:31:07 by momari            #+#    #+#             */
/*   Updated: 2024/03/24 00:04:03 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"


// void ft_free_chopsticks(t_data *data, int nop)
// {
//     while (nop++ < data->nophilo)
//         pthread_mutex_destroy(&data->chopsticks[nop]);
//     free(data->chopsticks);
// }

size_t getsommeoftime()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void ft_usleep(size_t time)
{
    size_t end_time = getsommeoftime() + time;

    while (getsommeoftime() < end_time)
        usleep(100);
}

void ft_generate_thinking(t_philos *current)
{
    sem_wait(current->sem_printing);
    printf("%ld %d is thinking\n", getsommeoftime() - current->start_date, current->philo_id);
    sem_post(current->sem_printing);
}

void ft_generate_eating(t_philos *current)
{

    sem_wait(current->sem_chopstick);
    sem_wait(current->sem_printing);
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    sem_post(current->sem_printing);
    sem_wait(current->sem_chopstick);
    sem_wait(current->sem_last_meal);
    current->last_meal = getsommeoftime();
    sem_post(current->sem_last_meal);
    sem_wait(current->sem_printing);
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    printf("%ld %d is eating\n", getsommeoftime() - current->start_date, current->philo_id);
    sem_post(current->sem_printing);
    
    // sem_wait(*(current->sem_check_dead));
    if (current->number_of_repeat > 0)
        current->number_of_repeat--;
    // sem_post(*(current->sem_check_dead));
    
    ft_usleep(current->time_to_eat);
    sem_post(current->sem_chopstick);
    sem_post(current->sem_chopstick);
}

void ft_generate_sleeping(t_philos *current)
{
    sem_wait(current->sem_printing);
    printf("%ld %d is sleeping\n", getsommeoftime() - current->start_date, current->philo_id);
    sem_post(current->sem_printing);
    ft_usleep(current->time_to_sleep);
}

void *test(void *data)
{
    t_philos *current;

    current = (t_philos *)data;
    while (TRUE)
    {
        sem_wait(current->sem_last_meal);
        if (!current->number_of_repeat)
        {
            sem_post(current->sem_last_meal);
            break;
        }
        else if (current->time_to_die < (getsommeoftime() - current->last_meal))
        {
            sem_wait(current->sem_printing);
            printf("%ld %d is dead\n",getsommeoftime() - current->last_meal, current->philo_id);
            sem_post(current->sem_check_dead);
            sem_post(current->sem_last_meal);
            break;
        }
        sem_post(current->sem_last_meal);
    }
    return (NULL);
}

void *routine_func(void *data)
{
    int pid;
    t_philos *current;

    current = (t_philos *)data;
    pid = fork();
    if (pid == 0)
    {
        current->start_date = getsommeoftime();
        pthread_create(&current->patrol, NULL, &test, data);
        pthread_detach(current->patrol);
        while (current->number_of_repeat != 0)
        {
            ft_generate_thinking(current);
            ft_generate_eating(current);
            ft_generate_sleeping(current);
        }
        exit(122);
    }
    else
        current->pid = pid;
    waitpid(pid, 0, 0);
    return (NULL);
}

void ft_free_philos(t_data *data, int n)
{
    (void)n;
    free(data->philos);
}

int ft_fill_philos(t_data *data, int ttd, int tte, int tts, int nor, int nop)
{
    int i;

    data->philos = (t_philos *)malloc(sizeof(t_philos) * data->nophilo);
    if (!data->philos)
        return (-1);
    i = 0;
    while (i < data->nophilo)
    {
        // if (nor != -1)
            
        data->philos[i].philo_id = i + 1;
        data->philos[i].sem_chopstick = data->sem_chopsticks;
        data->philos[i].time_to_die = ttd;
        data->philos[i].nop = nop;
        data->philos[i] .time_to_eat = tte;
        data->philos[i].time_to_sleep = tts;
        data->philos[i].number_of_repeat = nor;
        data->philos[i].sem_last_meal = data->sem_last_meal;
        data->philos[i].sem_printing = data->sem_printing;
        data->philos[i].sem_check_dead = data->sem_dead;
        data->philos[i].sem_check_time = data->sem_time;
        i++;
    }
    i = 0;
    while (i < data->nophilo)
    {
        data->philos[i].last_meal = getsommeoftime();
        if (pthread_create(&data->philos[i].philo, NULL, &routine_func, &data->philos[i]) != 0)
            return (ft_free_philos(data, i), -1);
        if (pthread_detach(data->philos[i].philo) != 0)
            return (ft_free_philos(data, i), -1);
        i++;
    }
    return (0);
}

void destroy_semaphore(sem_t *sem, const char *str)
{
    sem_close(sem);
    sem_unlink(str);
}

int ft_fill_structers(t_data *data, char **av, int nor)
{
    int ttd;
    int tte;
    int tts;
    int nop;
    int i;

    i = 0;
    nop = ft_atoi(av[1]);
    ttd = ft_atoi(av[2]);
    tte = ft_atoi(av[3]);
    tts = ft_atoi(av[4]);
    destroy_semaphore(data->sem_printing, "printing");
    destroy_semaphore(data->sem_last_meal, "last_meal");
    destroy_semaphore(data->sem_dead, "dead");
    destroy_semaphore(data->sem_chopsticks, "chopsticks");
    destroy_semaphore(data->sem_time, "time");
    data->sem_chopsticks = sem_open("chopsticks", O_CREAT, 0644, nop);
    data->sem_printing = sem_open("printing", O_CREAT, 0644, 1);
    data->sem_last_meal = sem_open("last_meal", O_CREAT, 0644, 1);
    data->sem_dead = sem_open("dead", O_CREAT, 0644, 0);
    data->sem_time = sem_open("time", O_CREAT, 0644, 0);
    data->nophilo = nop;
    data->time_to_die = ttd;
    if (ft_fill_philos(data, ttd, tte, tts, nor, nop) == -1)
        return (-1);
    return (0);
}


int ft_check_dead(t_data *data, int nor)
{
    sem_wait(data->sem_dead);
    if (nor == -1)
    {
        while (data->nophilo--)
            kill(data->philos[data->nophilo].pid, SIGTERM);    
    }
    destroy_semaphore(data->sem_printing, "printing");
    destroy_semaphore(data->sem_last_meal, "last_meal");
    destroy_semaphore(data->sem_chopsticks, "chopsticks");
    destroy_semaphore(data->sem_dead, "dead");
    // int i;
    // int n;

    // (void)data;
    // while (TRUE)
    // {
    //     i = 0;
    //     n = 0;
    //     while (n  < data->nophilo)
    //     {
    //         sem_wait(data->sem_dead);
    //         if (data->philos[n].number_of_repeat != 0)
    //         {
    //             sem_post(data->sem_dead);
    //             break ;
    //         }
    //         sem_post(data->sem_dead);
    //         n++;
    //     }
    //     if (n == data->nophilo)
    //     {
    //         destroy_semaphore(data->sem_printing, "printing");
    //         destroy_semaphore(data->sem_last_meal, "last_meal");
    //         destroy_semaphore(data->sem_chopsticks, "chopsticks");
    //         destroy_semaphore(data->sem_dead, "dead");
    //         return (1);
    //     }
    //     while (i < data->nophilo)
    //     {
    //         sem_wait(data->sem_last_meal);
    //         if (data->time_to_die < (getsommeoftime() - data->philos[i].last_meal))
    //         {
    //             sem_wait(data->sem_printing);
    //             printf("%ld %d is dead\n",getsommeoftime() - data->philos[i].last_meal, data->philos[i].philo_id);
    //             while (data->nophilo--)
    //                 kill(data->philos[data->nophilo].pid, SIGABRT);
    //             sem_post(data->sem_last_meal);
    //             destroy_semaphore(data->sem_printing, "printing");
    //             destroy_semaphore(data->sem_last_meal, "last_meal");
    //             destroy_semaphore(data->sem_chopsticks, "chopsticks");
    //             destroy_semaphore(data->sem_dead, "dead");
    //             return (1);
    //         }
    //         sem_post(data->sem_last_meal);
    //         i++;
    //     }
    // }
    return (0);
}

int ft_check_param(int ac, char **av)
{
    if (ft_atoi(av[1]) == -1 || ft_atoi(av[2]) == -1
        || ft_atoi(av[3]) == -1 || ft_atoi(av[4]) == -1)
        return (-1);
    if (ac == 6 && ft_atoi(av[5]) == -1)
        return (-1);
    return (0);
}

void ft_check_nor(t_data *data, int nor) {
    usleep(20);
    if (nor != -1) {
        int status;
        pid_t wpid;

        // Wait for all child processes to terminate
        while ((wpid = waitpid(-1, &status, 0)) > 0)
            ;
            // if (WIFEXITED(status)) {
            //     printf("Child process %d exited with status %d\n", wpid, WEXITSTATUS(status));
            // } else if (WIFSIGNALED(status)) {
            //     printf("Child process %d terminated by signal %d\n", wpid, WTERMSIG(status));
            // }

        // if (wpid == -1) {
        //     perror("waitpid");
        //     exit(EXIT_FAILURE);
        // }

        // Post the semaphore after all child processes have exited
        sem_post(data->sem_dead);
    }
}

int main(int ac, char **av)
{
    t_data *data;

    if (ac == 5 || ac == 6)
    {
        if (ft_check_param(ac, av) == -1)
            return (1);
        data = malloc(sizeof(t_data));
        if (ac == 5)
        {
            if (ft_fill_structers(data, av, -1) == -1)
                return (1);
        }
        else
        {
            if (ft_fill_structers(data, av, ft_atoi(av[5])) == -1)
                return (1);
        }
                //         destroy_semaphore(data->sem_printing, "printing");
                // destroy_semaphore(data->sem_last_meal, "last_meal");
                // destroy_semaphore(data->sem_chopsticks, "chopsticks");
                // destroy_semaphore(data->sem_dead, "dead");
        if (ac == 5)
        {
            ft_check_nor(data, -1);
            ft_check_dead(data, -1);
        }
        else
        {
            ft_check_nor(data, ft_atoi(av[5]));
            ft_check_dead(data, ft_atoi(av[5]));
        }
            
    }
    return (0);
}
