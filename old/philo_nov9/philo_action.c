/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:18:03 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/24 19:18:04 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int die(time_t last_eat_time, time_t time_to_die)
{
	if (now_time() - last_eat_time > time_to_die)
		return (1);
	else
		return (0);
}

int philo_eat(time_t *last_eat_time, void *args)
{
	time_t	start_eat_time;
	time_t	current_time;

	start_eat_time = now_time();
	current_time = start_eat_time;
	*last_eat_time = start_eat_time;
	if (check_death_flag(args))
		return (0);
	print_msg(EAT, args);
	while ((current_time - start_eat_time) < ((t_args *)args)->time_to_eat)
	{
		if (check_death_flag(args))
			return (0);
		if (die(*last_eat_time, ((t_args *)args)->time_to_die))
			return(set_death_flag(args), print_msg(DEAD, args), 0);
		usleep(WAIT_INTERVAL);
		current_time = now_time();
	}
	((t_args *)args)->num_eaten[((t_args *)args)->philo_index]++;
	return (1);
}

int philo_sleep(time_t last_eat_time, void *args)
{
	time_t start_sleep_time;
	time_t current_time;

	start_sleep_time = now_time();
	current_time = start_sleep_time;
	if (check_death_flag(args))
			return (0);
	print_msg(SLEEP, args);
	while ((current_time - start_sleep_time) < ((t_args *)args)->time_to_sleep)
	{
		if (check_death_flag(args))
			return (0);
		if (die(last_eat_time, ((t_args *)args)->time_to_die))
			return(set_death_flag(args), print_msg(DEAD, args), 0);
		usleep(WAIT_INTERVAL);
		current_time = now_time();
	}
	return (1);
}

int philo_think(time_t last_eat_time, void *args)
{
	time_t		time_to_die;
	time_t		start_think_time;

	time_to_die = ((t_args *)args)->time_to_die;
	start_think_time = now_time();

	if (check_death_flag(args))
			return (0);
	print_msg(THINK, args);
	while (1)
	{
		if (check_death_flag(args))
			return (0);
		if (die(last_eat_time, time_to_die))
			return(set_death_flag(args), print_msg(DEAD, args), 0);
		if	(now_time() - start_think_time > MIN_THINK_TIME)
		{
			lock_mutex_fork(args);
			if (forks_available(args))
			{
				unlock_mutex_fork(args);
				return(1);
			}
			unlock_mutex_fork(args);
		}
		usleep(WAIT_INTERVAL);
	}
	return (1);
}