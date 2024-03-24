/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:38:25 by momari            #+#    #+#             */
/*   Updated: 2024/03/15 01:39:33 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers_bonus.h"

static void	ft_print_error(void)
{
	printf("Error\n");
	exit(1);
}

long	ft_atoi(const char *str)
{
	long			temp;
	long			pos;
	long			i;

	i = 0;
	temp = 0;
	pos = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			pos *= -1;
		i++;
	}
	if (!str[i])
		ft_print_error();
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			ft_print_error();
		temp = (10 * temp) + (str[i] - '0');
		if (temp * pos > INT_MAX || temp * pos < INT_MIN)
			ft_print_error();
		i++;
	}
	return (temp * pos);
}
