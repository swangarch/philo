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

int forks_available(void *args)
{
	int 	left_available = 0;
	int		right_available = 0;
	int		**fork_ontable =  &(((t_args *)args)->fork_ontable);
	int		num = ((t_args *)args)->number_of_philosophers;
	int		p_index = ((t_args *)args)->philo_index;
	int		l_fork_index = p_index;
	int		r_fork_index = (p_index + 1) % num;

    if (num < 2)
		return (0);
	if ((*fork_ontable)[r_fork_index] == 1)
		right_available = 1;
	if ((*fork_ontable)[l_fork_index] == 1)
		left_available = 1;
	if (!left_available || !right_available)
		return (0);
	return (1);
}

void take_forks_action(int *fork_right, int *fork_left, void *args)
{
	int		p_index;
	int		**fork_ontable;
	int		l_fork_index;
	int		r_fork_index;

	p_index = ((t_args *)args)->philo_index;
	fork_ontable =  &(((t_args *)args)->fork_ontable);
	l_fork_index = p_index;
	r_fork_index = (p_index + 1) % ((t_args *)args)->number_of_philosophers;
	if ((*fork_ontable)[r_fork_index] == 1)
	{
		(*fork_ontable)[r_fork_index] = 0;
		*fork_right = 1;
	}
	print_msg(FORK, args);
	if ((*fork_ontable)[l_fork_index] == 1)
	{
		(*fork_ontable)[l_fork_index] = 0;
		*fork_left = 1;
	}
	print_msg(FORK, args);
}

void lock_mutex_fork(void *args)
{
	int 	num;
	int		p_index;
	int		l_fork_index;
	int		r_fork_index;

	num = ((t_args *)args)->number_of_philosophers;
	p_index = ((t_args *)args)->philo_index;
	l_fork_index = p_index;
	r_fork_index = (p_index + 1) % ((t_args *)args)->number_of_philosophers;
	pthread_mutex_lock(((t_args *)args)->mutex_fork[l_fork_index]);
	if (num > 1)
		pthread_mutex_lock(((t_args *)args)->mutex_fork[r_fork_index]);
}

void unlock_mutex_fork(void *args)
{
	int 	num;
	int		p_index;
	int		l_fork_index;
	int		r_fork_index;

	num = ((t_args *)args)->number_of_philosophers;
	p_index = ((t_args *)args)->philo_index;
	l_fork_index = p_index;
	r_fork_index = (p_index + 1) % ((t_args *)args)->number_of_philosophers;
	if (num > 1)
		pthread_mutex_unlock(((t_args *)args)->mutex_fork[r_fork_index]);
	pthread_mutex_unlock(((t_args *)args)->mutex_fork[l_fork_index]);
}

void take_forks(int *fork_right, int *fork_left, void *args)
{
	lock_mutex_fork(args);
	if (forks_available(args))
		take_forks_action(fork_right, fork_left, args);
	unlock_mutex_fork(args);
}

void return_forks(int *fork_right, int *fork_left, void *args)
{
	int		num;
	int		p_index;
	int		**fork_ontable;
	num = ((t_args *)args)->number_of_philosophers;
	p_index = ((t_args *)args)->philo_index;

	int		l_fork_index = p_index;
	int		r_fork_index = (p_index + 1) % num;

	lock_mutex_fork(args);
	fork_ontable =  &(((t_args *)args)->fork_ontable);
	if (*fork_right == 1)
	{
		if ((*fork_ontable)[r_fork_index] == 0)
		{
			(*fork_ontable)[r_fork_index] = 1;
			*fork_right = 0;
		}
	}
	if (*fork_left == 1)
	{
		if ((*fork_ontable)[l_fork_index] == 0)
		{
			(*fork_ontable)[l_fork_index] = 1;
			*fork_left = 0;
		}
	}
	unlock_mutex_fork(args);
}
