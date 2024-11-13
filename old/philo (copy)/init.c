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

void	init_args_philo(t_args *args, t_setup *set, t_state *state, \
	t_mutex *mutex)
{
	if (!args || !set || !state || !mutex)
		return ;
	args->num = set->num;
	args->time_to_die = set->time_to_die;
	args->time_to_eat = set->time_to_eat;
	args->time_to_sleep = set->time_to_sleep;
	args->num_must_eat = set->num_must_eat;
	args->num_eaten = state->num_eaten;
	args->last_eat_time = state->last_eat_time;
	args->death_flag = state->death_flag;
	args->start_time = state->start_time;
	args->mtx_fork = mutex->mtx_fork;
	args->mtx_eaten = mutex->mtx_eaten;
	args->mtx_printf = mutex->mtx_printf;
	args->mtx_death = mutex->mtx_death;
	args->mtx_lasteat = mutex->mtx_lasteat;
}

void	**set_args_philo(t_setup *set, t_state *state, t_mutex *mutexes)
{
	int		i;
	void	**arg_tab;
	int		num;

	num = set->num;
	arg_tab = (void **)malloc(sizeof(t_args *) * (num + 1));
	if (arg_tab == NULL)
		return (NULL);
	i = -1;
	while (++i < num)
	{
		arg_tab[i] = (t_args *)malloc(sizeof(t_args));
		if (arg_tab[i] == NULL)
			return (free_tab(arg_tab), NULL);
		((t_args *)arg_tab[i])->idx = i;
		init_args_philo(arg_tab[i], set, state, mutexes);
	}
	arg_tab[i] = NULL;
	return (arg_tab);
}

void	init_args_monitor(t_monitor *args_monitor, t_setup *set, t_state \
	*state, t_mutex *mutex)
{
	if (!args_monitor || !set || !state || !mutex)
		return ;
	args_monitor->death_flag = state->death_flag;
	args_monitor->num_must_eat = set->num_must_eat;
	args_monitor->num_eaten = state->num_eaten;
	args_monitor->start_time = state->start_time;
	args_monitor->num = set->num;
	args_monitor->time_to_die = set->time_to_die;
	args_monitor->last_eat_time = state->last_eat_time;
	args_monitor->mtx_printf = mutex->mtx_printf;
	args_monitor->mtx_eaten = mutex->mtx_eaten;
	args_monitor->mtx_death = mutex->mtx_death;
	args_monitor->mtx_lasteat = mutex->mtx_lasteat;
}
