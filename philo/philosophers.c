/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:03:56 by momari            #+#    #+#             */
/*   Updated: 2024/03/07 18:11:47 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// void *sayhello()
// {
//     printf("mohammed is here\n");
//     return (NULL);
// }
    // pthread_t th[5];
    // int i = 0;
    // pthread_mutex_init(&mutex, NULL);
    // while (i < 5)
    // {
    //     printf("thread %d is created\n", i);
    //     pthread_create(th + i, NULL, sayhello, NULL);
    //     i++;
    // }
    // i = 0;
    // while (i < 5)
    // {
    //     pthread_join(th[i], NULL);
    //     printf("thread %d is finished\n", i);
    //     i++;
    // } 
    // // pthread_create(&t2, NULL, sayhello, NULL);
    // // pthread_join(t2, NULL);
    // printf("the number is %d\n", a);
    // pthread_mutex_destroy(&mutex);



void ft_fill_chopsticks(t_data *data, int nop)
{
    data->chopsticks = malloc(nop * sizeof(pthread_mutex_t));
    if (!data->chopsticks)
        return ;
    while (nop--)
        pthread_mutex_init(&data->chopsticks[nop], NULL);
}

size_t getsommeoftime()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void ft_usleep(size_t time)
{
    size_t start_time = getsommeoftime();
    size_t end_time = start_time + time;

    while (getsommeoftime() < end_time)
        ;
}

void ft_generate_thinking(t_philos *current)
{
    pthread_mutex_lock(&current->mutex);
    printf("%ld %d is thinking\n", getsommeoftime() - current->start_date, current->philo_id);
    pthread_mutex_unlock(&current->mutex);
}

void ft_generate_eating(t_philos *current)
{
    pthread_mutex_lock(current->left_chopstick);
    pthread_mutex_lock(current->right_chopstick);
    if (current->time_to_die < (getsommeoftime() - current->last_meal))
    {
        pthread_mutex_lock(&current->mutex3);
        printf("%ld %d has dead\n",getsommeoftime(), current->philo_id);
        pthread_mutex_unlock(&current->mutex3);
        exit(0);
    }
    current->last_meal = getsommeoftime();
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    printf("%ld %d is eating\n", getsommeoftime() - current->start_date, current->philo_id);
    ft_usleep(current->time_to_eat * 1000);
    pthread_mutex_unlock(current->left_chopstick);
    pthread_mutex_unlock(current->right_chopstick);
}

void ft_generate_sleeping(t_philos *current)
{
    pthread_mutex_lock(&current->mutex2);
    printf("%ld %d is sleeping\n", getsommeoftime() - current->start_date, current->philo_id);
    ft_usleep(current->time_to_sleep * 1000);
    pthread_mutex_unlock(&current->mutex2);
}

void *routine_func(void *data)
{
    t_philos *current;

    current = (t_philos *)data;
    current->start_date = getsommeoftime();
    while (TRUE)
    {
        ft_generate_thinking(current);
        ft_generate_eating(current);
        ft_generate_sleeping(current);
    }
    return (NULL);
}

void ft_fill_philos(t_data *data, int ttd, int tte, int tts)
{
    int i;

    data->philos = (t_philos *)malloc(sizeof(t_philos) * data->nophilo);
    if (!data->philos)
        return ;
    i = 0;
    while (i < data->nophilo)
    {
        
        data->philos[i].philo_id = i + 1;
        data->philos[i].flag = &data->flag;
        data->philos[i].left_chopstick = &data->chopsticks[i];
        if (i == 0)
            data->philos[i].right_chopstick = &data->chopsticks[data->nophilo - 1];
        else
            data->philos[i].right_chopstick = &data->chopsticks[i - 1];
        data->philos[i].time_to_die = ttd;
        data->philos[i] .time_to_eat = tte;
        data->philos[i].time_to_sleep = tts;
        pthread_mutex_init(&data->philos[i].mutex, NULL);
        pthread_mutex_init(&data->philos[i].mutex2, NULL);
        pthread_mutex_init(&data->philos[i].mutex3, NULL);
        data->philos[i].last_meal = getsommeoftime();
        pthread_create(&data->philos[i].philo, NULL, &routine_func, &data->philos[i]);
        pthread_detach(data->philos[i].philo);
        i++;
    }
}

void ft_fill_structers(t_data *data, char **av)
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
    ft_fill_chopsticks(data, nop);
    data->flag    = 1;
    data->nophilo = nop;
    ft_fill_philos(data, ttd, tte, tts);
}

void ft_check_dead(t_data *data)
{
    int i;

    while (TRUE)
    {
        i = 0;
        while (i < data->nophilo)
        {
            if (data->philos[i].time_to_die < (getsommeoftime() - data->philos[i].last_meal) )
            {
                printf("%ld %d has dead\n",getsommeoftime(), data->philos[i].philo_id);
                exit(0);
            }
            i++;
        }
    }
}

int main(int ac, char **av)
{
    t_data data;

    if (ac == 5 || ac == 6)
    {
        ft_fill_structers(&data, av);
        // ft_check_dead(&data);
        while (TRUE)
            ;
    }
}