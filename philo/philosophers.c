/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:03:56 by momari            #+#    #+#             */
/*   Updated: 2024/03/23 21:46:52 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void ft_free_chopsticks(t_data *data, int nop)
{
    while (nop++ < data->nophilo)
        pthread_mutex_destroy(&data->chopsticks[nop]);
    free(data->chopsticks);
}

int ft_fill_chopsticks(t_data *data, int nop)
{
    data->chopsticks = malloc(nop * sizeof(pthread_mutex_t));
    if (!data->chopsticks)
        return (-1);
    while (nop--)
    {
        if (pthread_mutex_init(&data->chopsticks[nop], NULL) != 0)
        {
            ft_free_chopsticks(data, nop);
            return (-1);
        }
    }
    return (0);
}

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
        // ;
}

void ft_generate_thinking(t_philos *current)
{
    pthread_mutex_lock(current->mutex_printing);
    printf("%ld %d is thinking\n", getsommeoftime() - current->start_date, current->philo_id);
    pthread_mutex_unlock(current->mutex_printing);
}

void ft_generate_eating(t_philos *current)
{
    pthread_mutex_lock(current->left_chopstick);
    pthread_mutex_lock(current->mutex_printing);
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    pthread_mutex_unlock(current->mutex_printing);
    pthread_mutex_lock(current->right_chopstick);
    pthread_mutex_lock(current->mutex_last_meal);
    current->last_meal = getsommeoftime();
    pthread_mutex_unlock(current->mutex_last_meal);
    pthread_mutex_lock(current->mutex_printing);
    printf("%ld %d has taken a fork\n", getsommeoftime() - current->start_date, current->philo_id);
    printf("%ld %d is eating\n", getsommeoftime() - current->start_date, current->philo_id);
    pthread_mutex_unlock(current->mutex_printing);
    
    pthread_mutex_lock(current->check_dead);
    current->number_of_repeat--;
    pthread_mutex_unlock(current->check_dead);
    
    ft_usleep(current->time_to_eat);
    pthread_mutex_unlock(current->left_chopstick);
    pthread_mutex_unlock(current->right_chopstick);
}

void ft_generate_sleeping(t_philos *current)
{
    pthread_mutex_lock(current->mutex_printing);
    printf("%ld %d is sleeping\n", getsommeoftime() - current->start_date, current->philo_id);
    pthread_mutex_unlock(current->mutex_printing);
    ft_usleep(current->time_to_sleep);
}

void *routine_func(void *data)
{
    t_philos *current;

    current = (t_philos *)data;
    current->start_date = getsommeoftime();
    if (current->philo_id % 2 == 1 && current->philo_id == current->nop)
    {
        ft_generate_thinking(current);
        ft_usleep(current->time_to_eat * 2);
    }
    else if (current->philo_id % 2 == 0)
    {
        ft_generate_thinking(current);
        ft_usleep(current->time_to_eat);
    }
    while (current->number_of_repeat != 0)
    {
        ft_generate_thinking(current);
        ft_generate_eating(current);
        ft_generate_sleeping(current);
    }
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
        data->philos[i].philo_id = i + 1;
        data->philos[i].left_chopstick = &data->chopsticks[i];
        if (i == 0)
            data->philos[i].right_chopstick = &data->chopsticks[data->nophilo - 1];
        else
            data->philos[i].right_chopstick = &data->chopsticks[i - 1];
        data->philos[i].time_to_die = ttd;
        data->philos[i].nop = nop;
        data->philos[i] .time_to_eat = tte;
        data->philos[i].time_to_sleep = tts;
        data->philos[i].number_of_repeat = nor;
        data->philos[i].mutex_last_meal = &data->mutex_last_meal;
        data->philos[i].mutex_printing = &data->mutex_printing;
        data->philos[i].check_dead = &data->check_dead;
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
    if (ft_fill_chopsticks(data, nop) == -1)
        return (-1);
    data->nophilo = nop;
    data->time_to_die = ttd;
    if (pthread_mutex_init(&data->mutex_last_meal, NULL) != 0)
        return (ft_free_chopsticks(data, -1), -1);
    if (pthread_mutex_init(&data->mutex_printing, NULL) != 0)
        return (ft_free_chopsticks(data, -1), -1);
    if (pthread_mutex_init(&data->check_dead, NULL) != 0)
        return (ft_free_chopsticks(data, -1), -1);
    if (ft_fill_philos(data, ttd, tte, tts, nor, nop) == -1)
    {
        ft_free_chopsticks(data, -1);
        pthread_mutex_destroy(&data->mutex_last_meal);
        return (-1);
    }
    return (0);
}

int ft_check_dead(t_data *data)
{
    int i;
    int n;

    while (TRUE)
    {
        i = 0;
        n = 0;
        while (n  < data->nophilo)
        {
            pthread_mutex_lock(&data->check_dead);
            if (data->philos[n].number_of_repeat != 0)
            {
                pthread_mutex_unlock(&data->check_dead);
                break ;
            }
            pthread_mutex_unlock(&data->check_dead);
            n++;
        }
        if (n == data->nophilo)
            return (1);
        while (i < data->nophilo)
        {
            pthread_mutex_lock(&data->mutex_last_meal);
            if (data->time_to_die < (getsommeoftime() - data->philos[i].last_meal))
            {
                pthread_mutex_lock(&data->mutex_printing);
                printf("%ld %d is dead\n",getsommeoftime() - data->philos[i].last_meal, data->philos[i].philo_id);
                // pthread_mutex_unlock(&data->mutex_printing);
                // pthread_mutex_unlock(&data->mutex_last_meal);
                return (1);
            }
            pthread_mutex_unlock(&data->mutex_last_meal);
            i++;
        }
    }
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
        ft_check_dead(data);
    }
    return (0);
}
