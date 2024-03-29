/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:57:28 by momari            #+#    #+#             */
/*   Updated: 2024/03/21 01:59:29 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#define TRUE 1

typedef struct s_philosophers
{
    pthread_t       philo;
    int             philo_id;
    int             *flag;
    int             nop;
    pthread_mutex_t *right_chopstick;
    pthread_mutex_t *left_chopstick;
    pthread_mutex_t *mutex_last_meal;
    pthread_mutex_t *mutex_printing;
    pthread_mutex_t *check_dead;
    size_t          start_date;
    size_t          last_meal;
    size_t          start_meal;
    size_t          time_to_die;
    size_t          time_to_eat;
    size_t          time_to_sleep;
    int             number_of_repeat;
} t_philos;

typedef struct s_data
{
    pthread_mutex_t *chopsticks;
    pthread_mutex_t mutex_last_meal;
    pthread_mutex_t mutex_printing;
    pthread_mutex_t check_dead;
    int             nophilo;
    t_philos        *philos;
    size_t          time_to_die;
} t_data;


long	ft_atoi(const char *str);
void	ft_lstadd_back(t_data **lst, t_data *new);
t_data	*ft_lstnew(int index, int ttd, int tte, int tts);


#endif