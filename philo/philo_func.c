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

	pthread_mutex_lock(((t_args *)args)->mutex_death);
	death = *(((t_args *)args)->death_flag);
	pthread_mutex_unlock(((t_args *)args)->mutex_death);
	return (death);
}

int philo_eat(void *args)
{
	lock_mutex_fork(args);
	if (check_death(args))
		return (0);
	print_msg(EAT, args);
	pthread_mutex_lock(((t_args *)args)->mutex_last_eat[((t_args *)args)->idx]);
	((t_args *)args)->last_eat_time[((t_args *)args)->idx] = now_time();
	pthread_mutex_unlock(((t_args *)args)->mutex_last_eat[((t_args *)args)->idx]);
	usleep(((t_args *)args)->time_to_eat);
	pthread_mutex_lock(((t_args *)args)->mutex_eaten[((t_args *)args)->idx]);
	((t_args *)args)->num_eaten[((t_args *)args)->idx]++;
	pthread_mutex_unlock(((t_args *)args)->mutex_eaten[((t_args *)args)->idx]);
	unlock_mutex_fork(args);
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
		usleep(5000);
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