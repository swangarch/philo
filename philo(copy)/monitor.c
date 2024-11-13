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

void 	set_death(void *args_monitor)
{
	pthread_mutex_lock(((t_monitor *)args_monitor)->mtx_death);
	*(((t_monitor *)args_monitor)->death_flag) = DEAD;
	pthread_mutex_unlock(((t_monitor *)args_monitor)->mtx_death);
}

int check_all_eat_enough(void *args_monitor)
{
	int	i;
	int	num;
	int num_eaten;

	i = 0;
	num = ((t_monitor *)args_monitor)->num;
	if (((t_monitor *)args_monitor)->num_must_eat < 0)
			return (0);
	while (i < num)
	{
		pthread_mutex_lock(((t_monitor *)args_monitor)->mtx_eaten[i]);
		num_eaten = ((t_monitor *)args_monitor)->num_eaten[i];
		pthread_mutex_unlock(((t_monitor *)args_monitor)->mtx_eaten[i]);
		if (num_eaten < ((t_monitor *)args_monitor)->num_must_eat)
			return (0);
		i++;
	}
	return (1);
}

int	monitor_check_death(void *args_monitor)
{
	int num;
	int i;
	time_t current_time;
	time_t last_eat_time;

	i = -1;
	num = ((t_monitor *)args_monitor)->num;
	current_time = now_time();
	while (++i < num)
	{
		pthread_mutex_lock(((t_monitor *)args_monitor)->mtx_lasteat[i]);
		last_eat_time = ((t_monitor *)args_monitor)->last_eat_time[i];
		pthread_mutex_unlock(((t_monitor *)args_monitor)->mtx_lasteat[i]);
		if (current_time - last_eat_time > ((t_monitor *)args_monitor)->time_to_die)
			return (print_death(args_monitor, i), DEAD);
	}
	return (ALIVE);
}

void	*monitor_func(void *args_monitor)
{
	if (((t_monitor *)args_monitor)->num_must_eat < 0)
	{
		while(1)
		{
			if (monitor_check_death(args_monitor) == DEAD)
				return (set_death(args_monitor), NULL);
			usleep(WAIT_INTERVAL_MONITOR);
		}
	}
	else 
	{
		while(1)
		{
			if (monitor_check_death(args_monitor) == DEAD)
				return (set_death(args_monitor), NULL);
			else if (check_all_eat_enough(args_monitor))
				return (set_death(args_monitor), NULL);
			usleep(WAIT_INTERVAL_MONITOR);
		}
	}
	return (NULL);
}