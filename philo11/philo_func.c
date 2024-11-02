/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:41:04 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/28 16:41:06 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_sim_end(void *args)
{
	int	sim_end_flag;

	sim_end_flag = 0;
	sim_end_flag = *(((t_args *)args)->sim_end);
	return (sim_end_flag);
}

void	*philo_func(void *args)
{
	time_t start_time = ((t_args *)args)->start_time;
	int p_index = ((t_args *)args)->philo_index;
	int n_must_eat = ((t_args *)args)->number_of_times_each_philosopher_must_eat;
	pthread_mutex_t **mutex_fork = ((t_args *)args)->mutex_fork;
	time_t last_eat_time = start_time;

	/*----------------initialize forks--------------------------*/
	int fork_left = 0;
	int fork_right = 0;
	/*----------------initialize forks--------------------------*/

	while(1)
	{
		/*----------------take forks--------------------------*/
		pthread_mutex_lock(mutex_fork[p_index]);//-----
		if (forks_available(&fork_right, &fork_left, args))
			take_forks(&fork_right, &fork_left, args);
		pthread_mutex_unlock(mutex_fork[p_index]);//------
		/*----------------take forks--------------------------*/

		/*----------------philo eat--------------------------*/
		if (fork_right == 1 && fork_right == 1)
		{
			((t_args *)args)->alive[p_index] = philo_eat(&last_eat_time, args);
			if (!((t_args *)args)->alive[p_index])
				break ;
			//printf("number of eat of philo %d is %d\n", p_index, ((t_args *)args)->number_eaten[p_index]);
			if (n_must_eat >= 0 && ((t_args *)args)->number_eaten[p_index] >= n_must_eat)///////
				break ;/////////////////////////////////////////////////////////////////////////
		}
		/*----------------philo eat--------------------------*/

		/*----------------return forks--------------------------*/
		pthread_mutex_lock(mutex_fork[p_index]);//------
		return_forks(&fork_right, &fork_left, args);
		pthread_mutex_unlock(mutex_fork[p_index]);//-------
		/*----------------return forks--------------------------*/

		/*----------------philo sleep--------------------------*/
		((t_args *)args)->alive[p_index] = philo_sleep(last_eat_time, args);
		if (!((t_args *)args)->alive[p_index])
			break;
		/*----------------philo sleep--------------------------*/

		/*----------------philo think--------------------------*/
		((t_args *)args)->alive[p_index] = philo_think(last_eat_time, &fork_right, &fork_left, args);
		if (!((t_args *)args)->alive[p_index])
			break;
		/*----------------philo think--------------------------*/
	}
	return (NULL);
}