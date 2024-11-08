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

int	check_death_flag(void *args)
{
	return (*(((t_args *)args)->death_flag));
}

void 	set_death_flag(void *args)
{
	pthread_mutex_lock(((t_args *)args)->mutex_death);
	*(((t_args *)args)->death_flag) = _DEAD;
	pthread_mutex_unlock(((t_args *)args)->mutex_death);
}

int		take_fork_eat(time_t *last_eat_time, void *args, int *fork_right, int *fork_left)
{
	take_forks(fork_right, fork_left, args);
	if (*fork_left == 1 && *fork_right == 1)
	{
		if(!philo_eat(last_eat_time, args))
			return (0);
	}
	return_forks(fork_right, fork_left, args);
	return (1);
}

void	*philo_func(void *args)
{
	time_t last_eat_time;
	int fork_left;
	int fork_right;

	last_eat_time = ((t_args *)args)->start_time;
	fork_left = 0;
	fork_right = 0;
	if (((t_args *)args)->philo_index % 2 == 1)
	{
		if (!take_fork_eat(&last_eat_time, args, &fork_right, &fork_left))
			return (NULL);
	}
	while(1)
	{
		if (!philo_sleep(last_eat_time, args))
			return (NULL);///////////
		if (!philo_think(last_eat_time, args))
			return (NULL);///////////
		if (!take_fork_eat(&last_eat_time, args, &fork_right, &fork_left))
			return (NULL);///////////
	}
	return (NULL);
}