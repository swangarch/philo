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

void	sim_setup(int ac, char **av, t_setup *set)
{
	set->num = ft_atoi(av[1]);
	set->time_to_die = ft_atoi(av[2]) * 1000;
	set->time_to_eat = ft_atoi(av[3]) * 1000;
	set->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		set->num_must_eat = ft_atoi(av[5]);
	else if (ac == 5)
		set->num_must_eat = -1;
}

int	state_setup(t_setup *set, t_state *state)
{
	int	num;

	num = set->num;
	state->num_eaten = create_tab(num, 0);
	if (!state->num_eaten)
		return (0);
	state->death_flag = malloc(sizeof(int));
	if (!state->death_flag)
		return (free(state->num_eaten), 0);
	*(state->death_flag) = 0;
	state->start_time = now_time();
	state->last_eat_time = create_last_eat_time(num, state->start_time);
	if (!state->last_eat_time)
		return (free(state->num_eaten), free(state->last_eat_time), 0);
	return (1);
}

static void	init_mutex_tab(pthread_mutex_t **mtx_tab, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_init(mtx_tab[i], NULL);
		i++;
	}
}

static void	init_mutex(t_mutex *mutexes, int num)
{
	init_mutex_tab(mutexes->mtx_fork, num);
	init_mutex_tab(mutexes->mtx_eaten, num);
	init_mutex_tab(mutexes->mtx_lasteat, num);
	pthread_mutex_init(mutexes->mtx_printf, NULL);
	pthread_mutex_init(mutexes->mtx_death, NULL);
}

int	mutex_setup(t_setup *set, t_mutex *mutexes)
{
	int	num;

	num = set->num;
	mutexes->mtx_printf = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mtx_printf)
		return (0);
	mutexes->mtx_death = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mtx_death)
		return (free(mutexes->mtx_printf), 0);
	mutexes->mtx_fork = create_mutex_tab(num);
	if (!mutexes->mtx_fork)
		return (free(mutexes->mtx_printf), free(mutexes->mtx_death), 0);
	mutexes->mtx_eaten = create_mutex_tab(num);
	if (!mutexes->mtx_eaten)
		return (free_mutex_tab(mutexes->mtx_fork, num), \
			free(mutexes->mtx_printf), free(mutexes->mtx_death), 0);
	mutexes->mtx_lasteat = create_mutex_tab(num);
	if (!mutexes->mtx_lasteat)
		return (free_mutex_tab(mutexes->mtx_eaten, num), \
			free_mutex_tab(mutexes->mtx_fork, num), free(mutexes->mtx_printf), \
				free(mutexes->mtx_death), 0);
	init_mutex(mutexes, num);
	return (1);
}
