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

void	destroy_state(t_state *state)
{
	free(state->fork_ontable);
	free(state->alive);
	free(state->number_eaten);
}

void	destroy_mutex_forks(pthread_mutex_t **mutex_forks, int num)
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

void	destroy_mutex_printf(pthread_mutex_t *mutex_printf)
{
	pthread_mutex_destroy(mutex_printf);
}

void	destroy_mutexes(t_mutex *mutexes, int num)
{
	destroy_mutex_forks(mutexes->mutex_forks, num);
	destroy_mutex_printf(mutexes->mutex_printf);
	free(mutexes->mutex_printf);
}
