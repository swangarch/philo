/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:03:22 by shuwang           #+#    #+#             */
/*   Updated: 2024/11/13 19:03:24 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*create_tab(int num, int value)
{
	int		i;
	int		*tab;

	tab = malloc(sizeof(int) * num);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < num)
	{
		tab[i] = value;
		i++;
	}
	return (tab);
}

time_t	*create_last_eat_time(int num, time_t last_eat_time)
{
	int		i;
	time_t		*tab;

	tab = malloc(sizeof(time_t) * num);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < num)
	{
		tab[i] = last_eat_time;
		i++;
	}
	return (tab);
}

pthread_mutex_t	**create_mutex_tab(int num)
{
	pthread_mutex_t	**mutex_tab;
	int				i;

	mutex_tab = malloc((num) * sizeof(pthread_mutex_t *));
	if (!mutex_tab)
		return (NULL);
	i = 0;
	while (i < num)
	{
		mutex_tab[i] = malloc(sizeof(pthread_mutex_t));
		if (!mutex_tab[i])
		{
			destroy_mutex_tab(mutex_tab, i);
			return (NULL);
		}
		i++;
	}
	return (mutex_tab);
}
