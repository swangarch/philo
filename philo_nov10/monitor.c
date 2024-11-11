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

void 	monitor_set_death_flag(void *args_monitor)
{
	pthread_mutex_lock(((t_args_monitor *)args_monitor)->mutex_death);
	*(((t_args_monitor *)args_monitor)->death_flag) = _DEAD;
	pthread_mutex_unlock(((t_args_monitor *)args_monitor)->mutex_death);
}

int check_all_eat_enough(void *args_monitor)
{
	int	i;
	int	num;

	i = 0;
	num = ((t_args_monitor *)args_monitor)->num;
	// if (((t_args_monitor *)args_monitor)->number_must_eat < 0)
	// 		return (0);
	while (i < num)
	{

		//printf("number %d has eaten %d times\n", i, ((t_args_monitor *)args_monitor)->number_eaten[i]);
		if (((t_args_monitor *)args_monitor)->num_eaten[i] < ((t_args_monitor *)args_monitor)->num_must_eat)
			return (0);
		i++;
	}
	//printf("finished\n");////////////////////////////////////////////////////
	return (1);
}

int	monitor_check_death_flag(void *args_monitor)
{
	return (*(((t_args_monitor *)args_monitor)->death_flag));
}

int join_philo(pthread_t *philo, int num)
{
	int i;

	i = 0;
	if (!philo || num < 1)
		return (0);
	while (i < num)
	{
		pthread_join(philo[i], NULL);  ///check
		i++;
	}
	return (1);
}

void	*monitor_func(void *args_monitor)
{
	int num;

	if (monitor_check_death_flag(args_monitor) == _DEAD)
	{
		ft_putstr_fd("dead\n", 2);//////
	}
	else
		ft_putstr_fd("not dead\n", 2);//////

	num = ((t_args_monitor *)args_monitor)->num;
	if (((t_args_monitor *)args_monitor)->num_must_eat < 0)
	{
		while(1)
		{
			if (monitor_check_death_flag(args_monitor) == _DEAD)
				break ;
			usleep(WAIT_INTERVAL_MONITOR);
		}
	}
	else 
	{
		//ft_putstr_fd("case02\n", 2);
		while(1)
		{
			// if (monitor_check_death_flag(args_monitor))
			// 	ft_putstr_fd("monitor check death\n", 2);//////
			// else
			// 	ft_putstr_fd("not dead\n", 2);//////
			if (monitor_check_death_flag(args_monitor) == _DEAD)
			{
				ft_putstr_fd("dead\n", 2);//////
				break ;
			}
			else
				ft_putstr_fd("not dead\n", 2);//////
			//printf("flag is %d\n", monitor_check_death_flag(args_monitor));
			if (!check_all_eat_enough(args_monitor))  /////////only if argv not -1
			{
				monitor_set_death_flag(args_monitor);
				//printf("flag is %d\n", monitor_check_death_flag(args_monitor));///////////////////
				//break ;
			}
			usleep(WAIT_INTERVAL_MONITOR);
		}
	}
	printf("wait\n");/////////////////////////////////////////////////////////////////
	join_philo(((t_args_monitor *)args_monitor)->philo, num);
	return (NULL);
}