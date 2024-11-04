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

int	check_sim_end(void *args)
{
	int	sim_end_flag;

	// sim_end_flag = 0;
	sim_end_flag = *(((t_args *)args)->sim_end);
	return (sim_end_flag);
}

int		p_eat(time_t *last_eat_time, void *args, int fork_right, int fork_left)
{
	int p_index = ((t_args *)args)->philo_index;

	if (fork_left == 1 && fork_right == 1)
	{
		((t_args *)args)->alive[p_index] = philo_eat(last_eat_time, args);
		if (!((t_args *)args)->alive[p_index])
			return (0);
	}
	return (1);
}

int		p_sleep(time_t *last_eat_time, void *args)
{
	int p_index = ((t_args *)args)->philo_index;

	((t_args *)args)->alive[p_index] = philo_sleep(*last_eat_time, args);
		if (!((t_args *)args)->alive[p_index])
			return (0);
	return (1);
}

int		p_think(time_t *last_eat_time, void *args)
{
	int p_index = ((t_args *)args)->philo_index;

	((t_args *)args)->alive[p_index] = philo_think(*last_eat_time, args);
		if (!((t_args *)args)->alive[p_index])
			return (0);
	return (1);
}

void	*philo_func(void *args)
{
	time_t last_eat_time = ((t_args *)args)->start_time;
	int fork_left = 0;
	int fork_right = 0;

	while(1)
	{
		take_forks(&fork_right, &fork_left, args);
		if (!p_eat(&last_eat_time, args, fork_right, fork_left))
			break ;
		return_forks(&fork_right, &fork_left, args);
		if (!p_sleep(&last_eat_time, args))
			break;
		if (!p_think(&last_eat_time, args))
			break;
	}
	return (NULL);
}