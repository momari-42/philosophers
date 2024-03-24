/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 01:31:11 by momari            #+#    #+#             */
/*   Updated: 2024/03/23 23:14:40 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PhiLOSOPHERS_BONUS_H
#define PhiLOSOPHERS_BONUS_H

#include <semaphore.h>
#include <sys/wait.h> // For wait
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#define TRUE 1

typedef struct s_philosophers
{
    pthread_t       philo;
    pthread_t       patrol;
    int             philo_id;
    int             *flag;
    int             nop;
    sem_t           *sem_chopstick;
    sem_t           *sem_last_meal;
    sem_t           *sem_printing;
    sem_t           *sem_check_dead;
    sem_t           *sem_check_time;
    size_t          start_date;
    size_t          last_meal;
    size_t          start_meal;
    size_t          time_to_die;
    size_t          time_to_eat;
    size_t          time_to_sleep;
    int             number_of_repeat;
    int             pid;
} t_philos;

typedef struct s_data
{
    sem_t           *sem_chopsticks;
    sem_t           *sem_last_meal;
    sem_t           *sem_printing;
    sem_t           *sem_dead;
    sem_t           *sem_time;
    int             nophilo;
    t_philos        *philos;
    size_t          time_to_die;
} t_data;


long	ft_atoi(const char *str);



#endif