/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:12:13 by shuwang           #+#    #+#             */
/*   Updated: 2024/09/18 16:12:14 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int lock_mutex_fork_old(void *args, int l_idx, int r_idx)
{
	int	fork;

	fork = 0;
	pthread_mutex_lock(((t_args *)args)->mtx_fork[l_idx]);
	fork++;
	if (check_death(args))
	{
		pthread_mutex_unlock(((t_args *)args)->mtx_fork[l_idx]);
		return (0);
	}
	print_msg(FORK, args);
	if (((t_args *)args)->num > 1)
	{
		pthread_mutex_lock(((t_args *)args)->mtx_fork[r_idx]);
		fork++;
		if (check_death(args))
		{
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[r_idx]);
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[l_idx]);
			return (0);
		}
		print_msg(FORK, args);
	}
	return (fork);
}

int lock_mutex_fork_even(void *args, int l_idx, int r_idx)
{
	int	fork;

	fork = 0;
	pthread_mutex_lock(((t_args *)args)->mtx_fork[r_idx]);
	fork++;
	if (check_death(args))
	{
		pthread_mutex_unlock(((t_args *)args)->mtx_fork[r_idx]);
		return (0);
	}
	print_msg(FORK, args);
	if (((t_args *)args)->num > 1)
	{
		pthread_mutex_lock(((t_args *)args)->mtx_fork[l_idx]);
		fork++;
		if (check_death(args))
		{
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[l_idx]);
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[r_idx]);
			return (0);
		}
		print_msg(FORK, args);
	}
	return (fork);
}

int lock_mutex_fork(void *args)
{
	int		l_idx;
	int		r_idx;

	l_idx = ((t_args *)args)->idx;
	r_idx = (((t_args *)args)->idx + 1) % ((t_args *)args)->num;
	if (((t_args *)args)->idx + 1 / 2)
		return (lock_mutex_fork_old(args, l_idx, r_idx));
	else
		return (lock_mutex_fork_even(args, l_idx, r_idx));
}

void unlock_mutex_fork(void *args)
{
	int		l_idx;
	int		r_idx;

	l_idx = ((t_args *)args)->idx;
	r_idx = (((t_args *)args)->idx + 1) % ((t_args *)args)->num;
	if (((t_args *)args)->idx/ 2)
	{
		if (((t_args *)args)->num > 1)
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[r_idx]);
		pthread_mutex_unlock(((t_args *)args)->mtx_fork[l_idx]);
	}
	else
	{
		if (((t_args *)args)->num > 1)
			pthread_mutex_unlock(((t_args *)args)->mtx_fork[l_idx]);
		pthread_mutex_unlock(((t_args *)args)->mtx_fork[r_idx]);
	}
}
