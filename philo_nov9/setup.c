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

static pthread_mutex_t	**init_mutex_forks(int num_philo)
{
	pthread_mutex_t	**mutex_forks;
	int				i;

	mutex_forks = malloc((num_philo) * sizeof(pthread_mutex_t *));
	if (!mutex_forks)
		return (NULL);
	i = 0;
	while (i < num_philo)
	{
		mutex_forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!mutex_forks[i])
		{
			//free_mutex_forks
			return (NULL);
		}
		pthread_mutex_init(mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
}

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
	state->fork_ontable = init_tab(num, 1);
	if (!state->fork_ontable)
		return (0);
	state->num_eaten = init_tab(num, 0);
	if (!state->num_eaten)
		return (free(state->fork_ontable), 0);
	state->death_flag = malloc(sizeof(int));
	if (!state->death_flag)
		return (free(state->num_eaten), free(state->fork_ontable), 0);
	*(state->death_flag) = 0;
	state->start_time = now_time();
	return (1);
}

int	mutex_setup(t_setup *set, t_mutex *mutexes)
{
	int	num;

	num = set->num;
	mutexes->mutex_printf = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mutex_printf)
		return (0);
	mutexes->mutex_death = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mutex_death)
		return (free(mutexes->mutex_printf), 0);
	mutexes->mutex_forks = init_mutex_forks(num);
	if (!mutexes->mutex_forks)
		return (free(mutexes->mutex_printf), free(mutexes->mutex_death), 0);
	pthread_mutex_init(mutexes->mutex_printf, NULL);
	pthread_mutex_init(mutexes->mutex_death, NULL);
	return (1);
}
