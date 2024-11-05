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

static int	*init_tab(int num_philo, int value)
{
	int		i;
	int		*tab;

	tab = malloc(sizeof(int) * num_philo);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < num_philo)
	{
		tab[i] = value;
		i++;
	}
	return (tab);
}

static pthread_mutex_t **init_mutex_forks(int num_philo)
{
	pthread_mutex_t **mutex_forks;
	int	i;

	mutex_forks = malloc((num_philo) * sizeof(pthread_mutex_t *));
	if (!mutex_forks)
		return (NULL);
	i = 0;
	while (i < num_philo)
	{
		mutex_forks[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
}

void sim_setup(int ac, char **av, t_setup *set)
{
	set->number_of_philosophers = ft_atoi(av[1]);
	set->time_to_die = ft_atoi(av[2]) * 1000;
	set->time_to_eat = ft_atoi(av[3]) * 1000;
	set->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		set->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else if (ac == 5)
		set->number_of_times_each_philosopher_must_eat = -1;
}

void state_setup(t_setup *set, t_state *state)
{
	int num;

	num = set->number_of_philosophers;
	state->fork_ontable = init_tab(num, 1);  //protect
	state->alive = init_tab(num, _ALIVE);
	state->number_eaten = init_tab(num, 0);
	state->sim_end = malloc(sizeof(int));
	if(!state->sim_end)
		return ;   ///////////////////////////can be removed
	*(state->sim_end) = 0;
	state->start_time = now_time();
}

void mutex_setup(t_setup *set, t_mutex *mutexes)
{
	int num;

	num = set->number_of_philosophers;
	mutexes->mutex_printf = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mutex_printf)
		return ;////////////protect
	pthread_mutex_init(mutexes->mutex_printf, NULL);
	mutexes->mutex_forks = init_mutex_forks(num);//protect
}