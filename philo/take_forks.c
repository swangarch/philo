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

void lock_mutex_fork(void *args)
{
	int 	num;
	int		p_index;
	int		l_fork_index;
	int		r_fork_index;

	num = ((t_args *)args)->num;
	p_index = ((t_args *)args)->idx;
	l_fork_index = p_index;
	r_fork_index = (p_index + 1) % ((t_args *)args)->num;
	if (p_index / 2)
	{
		pthread_mutex_lock(((t_args *)args)->mutex_fork[l_fork_index]);
		if (check_death(args))
			return(pthread_mutex_unlock(((t_args *)args)->mutex_fork[l_fork_index]), (void)0);
		print_msg(FORK, args);
		if (num > 1)
		{
			pthread_mutex_lock(((t_args *)args)->mutex_fork[r_fork_index]);
			if (check_death(args))
				return(pthread_mutex_unlock(((t_args *)args)->mutex_fork[r_fork_index]), (void)0);
			print_msg(FORK, args);
		}
	}
	else
	{
		pthread_mutex_lock(((t_args *)args)->mutex_fork[r_fork_index]);
		if (check_death(args))
			return(pthread_mutex_unlock(((t_args *)args)->mutex_fork[r_fork_index]), (void)0);
		print_msg(FORK, args);
		if (num > 1)
		{
			pthread_mutex_lock(((t_args *)args)->mutex_fork[l_fork_index]);
			if (check_death(args))
				return(pthread_mutex_unlock(((t_args *)args)->mutex_fork[l_fork_index]), (void)0);
			print_msg(FORK, args);
		}
	}
}

void unlock_mutex_fork(void *args)
{
	int 	num;
	int		p_index;
	int		l_fork_index;
	int		r_fork_index;

	num = ((t_args *)args)->num;
	p_index = ((t_args *)args)->idx;
	l_fork_index = p_index;
	r_fork_index = (p_index + 1) % ((t_args *)args)->num;
	if (p_index / 2)
	{
		if (num > 1)
			pthread_mutex_unlock(((t_args *)args)->mutex_fork[r_fork_index]);
		pthread_mutex_unlock(((t_args *)args)->mutex_fork[l_fork_index]);
	}
	else
	{
		if (num > 1)
			pthread_mutex_unlock(((t_args *)args)->mutex_fork[l_fork_index]);
		pthread_mutex_unlock(((t_args *)args)->mutex_fork[r_fork_index]);
	}
}
