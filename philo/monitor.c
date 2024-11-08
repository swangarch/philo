/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:42:20 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/28 16:42:22 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_all_eat_enough(void *args_monitor)
{
	int	i;
	int	num;

	i = 0;
	num = ((t_args_monitor *)args_monitor)->number_of_philosophers;
	if (((t_args_monitor *)args_monitor)->number_must_eat < 0)
			return (0);
	while (i < num)
	{
		if (((t_args_monitor *)args_monitor)->number_eaten[i] < ((t_args_monitor *)args_monitor)->number_must_eat)
			return (0);
		i++;
	}
	return (1);
}

// int check_alive(void *args_monitor)
// {
// 	int i;
// 	int num;

// 	num = ((t_args_monitor *)args_monitor)->number_of_philosophers;
// 	i = 0;
// 	while (i < num)
// 	{
// 		if (((t_args_monitor *)args_monitor)->alive[i] == _DEAD)
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

int join_philo(pthread_t *philo, int num)
{
	int i;

	i = 0;
	if (!philo || num < 1)
		return (0);
	while (i < num)
	{
		pthread_join(philo[i], NULL);  ///check
		i++;
	}
	return (1);
}

void	*monitor_func(void *args_monitor)
{
	int num;

	num = ((t_args_monitor *)args_monitor)->number_of_philosophers;
	while(1)
	{
		// if (!check_alive(args_monitor))
		// {
		// 	*(((t_args_monitor *)args_monitor)->sim_end) = 1;
		// 	break ;
		// }
		if (check_all_eat_enough(args_monitor))  /////////only if argv not -1
		{
			set_death_flag(args_monitor);
			break ;
		}
		usleep(WAIT_INTERVAL_MONITOR);
	}
	join_philo(((t_args_monitor *)args_monitor)->philo, num);
	return (NULL);
}