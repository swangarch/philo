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

void	free_tab(void **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	tab = NULL;
}

void	free_mutex_tab(pthread_mutex_t **mutex_forks, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		free(mutex_forks[i]);
		mutex_forks[i] = NULL;
		i++;
	}
	free(mutex_forks);
}

void	destroy_mutex_tab(pthread_mutex_t **mutex_forks, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(mutex_forks[i]);
		free(mutex_forks[i]);
		mutex_forks[i] = NULL;
		i++;
	}
	free(mutex_forks);
}

void	destroy_state(t_state *state)
{
	free(state->num_eaten);
	free(state->death_flag);
	free(state->last_eat_time);
}

void	destroy_mutexes(t_mutex *mutexes, int num)
{
	destroy_mutex_tab(mutexes->mtx_eaten, num);
	destroy_mutex_tab(mutexes->mtx_fork, num);
	destroy_mutex_tab(mutexes->mtx_lasteat, num);
	pthread_mutex_destroy(mutexes->mtx_death);
	pthread_mutex_destroy(mutexes->mtx_printf);
	free(mutexes->mtx_printf);
	free(mutexes->mtx_death);
}
