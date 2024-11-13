/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:41:04 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/28 16:41:06 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(void *args)
{
	int death;

	pthread_mutex_lock(((t_args *)args)->mtx_death);
	death = *(((t_args *)args)->death_flag);
	pthread_mutex_unlock(((t_args *)args)->mtx_death);
	return (death);
}

int philo_eat(void *ag)
{
	int fork;

	fork = lock_mutex_fork(ag);
	if (!fork)
		return (0);
	else if (fork == 1)
		return (unlock_mutex_fork(ag), 0);
	print_msg(EAT, ag);
	pthread_mutex_lock(((t_args *)ag)->mtx_lasteat[((t_args *)ag)->idx]);
	((t_args *)ag)->last_eat_time[((t_args *)ag)->idx] = now_time();
	pthread_mutex_unlock(((t_args *)ag)->mtx_lasteat[((t_args *)ag)->idx]);
	usleep(((t_args *)ag)->time_to_eat);
	pthread_mutex_lock(((t_args *)ag)->mtx_eaten[((t_args *)ag)->idx]);
	((t_args *)ag)->num_eaten[((t_args *)ag)->idx]++;
	pthread_mutex_unlock(((t_args *)ag)->mtx_eaten[((t_args *)ag)->idx]);
	unlock_mutex_fork(ag);
	return (1);
}

int philo_sleep(void *args)
{
	if (check_death(args))
		return (0);
	print_msg(SLEEP, args);
	usleep(((t_args *)args)->time_to_sleep);
	return (1);
}

int philo_think(void *args)
{
	if (check_death(args))
		return (0);
	print_msg(THINK, args);
	usleep(MIN_THINK_TIME);
	return (1);
}

void	*philo_func(void *args)
{
	if (check_death(args))
		return (NULL);
	if (((t_args *)args)->idx % 2)
	{
		if (!philo_think(args))
			return (NULL);
	}
	while(1)
	{
		if (check_death(args))
			return (NULL);
		if (!philo_eat(args))
			return (NULL);
		if (!philo_sleep(args))
			return (NULL);
		if (!philo_think(args))
			return (NULL);
	}
	return (NULL);
}