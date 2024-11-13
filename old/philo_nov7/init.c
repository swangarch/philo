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
	args->number_of_philosophers = set->number_of_philosophers;
	args->time_to_die = set->time_to_die;
	args->time_to_eat = set->time_to_eat;
	args->time_to_sleep = set->time_to_sleep;
	args->number_of_times_each_philosopher_must_eat = set->\
		number_of_times_each_philosopher_must_eat;
	args->fork_ontable = state->fork_ontable;
	args->alive = state->alive;
	args->number_eaten = state->number_eaten;
	args->sim_end = state->sim_end;
	args->start_time = state->start_time;
	args->mutex_fork = mutex->mutex_forks;
	args->mutex_printf = mutex->mutex_printf;
}

void	**set_args_philo(t_setup *set, t_state *state, t_mutex *mutexes)
{
	int		i;
	void	**arg_tab;
	int		num;

	num = set->number_of_philosophers;
	arg_tab = (void **)malloc(sizeof(t_args *) * (num + 1));
	if (arg_tab == NULL)
		return (NULL);
	i = -1;
	while (++i < num)
	{
		arg_tab[i] = (t_args *)malloc(sizeof(t_args));
		if (arg_tab[i] == NULL)
			return (free_tab(arg_tab), NULL);
		((t_args *)arg_tab[i])->philo_index = i;
		init_args_philo(arg_tab[i], set, state, mutexes);
	}
	arg_tab[i] = NULL;
	return (arg_tab);
}

void	init_args_monitor(t_args_monitor *args_monitor, t_setup *set, t_state \
	*state, t_mutex *mutex)
{
	if (!args_monitor || !set || !state || !mutex)
		return ;
	args_monitor->sim_end = state->sim_end;
	args_monitor->mutex_printf = mutex->mutex_printf;
	args_monitor->alive = state->alive;
	args_monitor->number_must_eat = set->number_of_times_each_philosopher_must_eat;
	args_monitor->number_eaten = state->number_eaten;
	args_monitor->number_of_philosophers = set->number_of_philosophers;
}
