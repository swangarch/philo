/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 17:28:15 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/28 17:28:17 by shuwang          ###   ########.fr       */
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
	int			i;
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

void	print_msg(int message, void *args)
{
	time_t	start_time;
	time_t	time_stamp;
	int		idx;

	start_time = ((t_args *)args)->start_time;
	time_stamp = timestamp(start_time);
	idx = ((t_args *)args)->idx;
	pthread_mutex_lock(((t_args *)args)->mtx_printf);
	if (message == FORK)
		printf("%ld %d has taken a fork\n", time_stamp, idx + 1);
	else if (message == EAT)
		printf("%ld %d is eating\n", time_stamp, idx + 1);
	else if (message == SLEEP)
		printf("%ld %d is sleeping\n", time_stamp, idx + 1);
	else if (message == THINK)
		printf("%ld %d is thinking\n", time_stamp, idx + 1);
	pthread_mutex_unlock(((t_args *)args)->mtx_printf);
}

void	print_death(void *args_monitor, int idx)
{
	time_t	start_time;
	time_t	time_stamp;

	start_time = ((t_monitor *)args_monitor)->start_time;
	time_stamp = timestamp(start_time);
	pthread_mutex_lock(((t_monitor *)args_monitor)->mtx_printf);
	printf("%ld %d died\n", time_stamp, idx + 1);
	pthread_mutex_unlock(((t_monitor *)args_monitor)->mtx_printf);
}
