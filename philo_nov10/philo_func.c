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

// int	check_death_flag(void *args)
// {
// 	return (*(((t_args *)args)->death_flag));
// }

// void 	set_death_flag(void *args)
// {
// 	pthread_mutex_lock(((t_args *)args)->mutex_death);
// 	*(((t_args *)args)->death_flag) = _DEAD;
// 	pthread_mutex_unlock(((t_args *)args)->mutex_death);
// }

// int die(time_t last_eat_time, time_t time_to_die)
// {
// 	if (now_time() - last_eat_time > time_to_die)
// 		return (1);
// 	else
// 		return (0);
// }

int philo_eat(time_t *last_eat_time, void *args)
{
	lock_mutex_fork(args);
	print_msg(EAT, args);
	*last_eat_time = now_time();
	usleep(((t_args *)args)->time_to_eat);
	((t_args *)args)->num_eaten[((t_args *)args)->idx]++;
	unlock_mutex_fork(args);
	return (1);
}

int philo_sleep(void *args)
{
	print_msg(SLEEP, args);
	usleep(((t_args *)args)->time_to_sleep);
	return (1);
}

int philo_think(void *args)
{
	print_msg(THINK, args);
	usleep(MIN_THINK_TIME);
	return (1);
}

void	*philo_func(void *args)
{
	time_t last_eat_time;

	last_eat_time = ((t_args *)args)->start_time;
	if (((t_args *)args)->idx % 2)
	{
		if (!philo_eat(&last_eat_time, args))
			return (NULL);
	}
	while(1)
	{
		if (!philo_sleep(args))
			return (NULL);///////////
		if (!philo_think(args))
			return (NULL);///////////
		if (!philo_eat(&last_eat_time, args))
			return (NULL);///////////
	}
	return (NULL);
}