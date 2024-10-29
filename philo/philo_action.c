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
	time_t	start_eat_time = now_time();
	time_t	current_time = start_eat_time;
	int 	time_to_die = ((t_args *)args)->time_to_die;
	int		time_to_eat = ((t_args *)args)->time_to_eat;
	int     p_index = ((t_args *)args)->philo_index;

	*last_eat_time = start_eat_time;

	if (now_time() - *last_eat_time + 5 < time_to_die)
		return (1);

	if (check_sim_end(args))
		return (0);
	print_msg(EAT, args);
	while ((current_time - start_eat_time) < time_to_eat)
	{
		if (check_sim_end(args))
			return (0);
		if (die(*last_eat_time, time_to_die))
		{
			print_msg(DEAD, args);
			return(0);
		}
		usleep(WAIT_INTERVAL);
		current_time = now_time();
	}
	((t_args *)args)->number_eaten[p_index]++;
	return (1);
}

int philo_sleep(time_t last_eat_time, void *args)
{
	time_t start_sleep_time = now_time();
	time_t current_time = start_sleep_time;

	int 	time_to_die = ((t_args *)args)->time_to_die;
	int		time_to_sleep = ((t_args *)args)->time_to_sleep;

	if (check_sim_end(args))
			return (0);
	
	print_msg(SLEEP, args);
	while ((current_time - start_sleep_time) < time_to_sleep)
	{
		if (check_sim_end(args))
			return (0);
		if (die(last_eat_time, time_to_die))
		{
			print_msg(DEAD, args);
			return(0);
		}
		usleep(WAIT_INTERVAL);
		current_time = now_time();
	}
	return (1);
}

int philo_think(time_t last_eat_time, int *fork_right, int *fork_left, void *args)
{
	int		time_to_die = ((t_args *)args)->time_to_die;

	if (check_sim_end(args))
			return (0);
	
	print_msg(THINK, args);
	while (1)
	{
		if (check_sim_end(args))
			return (0);
		if (die(last_eat_time, time_to_die))
		{
			print_msg(DEAD, args);
			return(0);
		}
		if	(forks_available(fork_right, fork_left, args))
		{
			return(1);
		}
		usleep(WAIT_INTERVAL);
	}
	return (1);
}
