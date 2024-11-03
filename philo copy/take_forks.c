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

int forks_available(int *fork_right, int *fork_left, void *args)
{
	int 	left_available = 0;
	int		right_available = 0;
	int		**fork_ontable =  &(((t_args *)args)->fork_ontable);
	int		num = ((t_args *)args)->number_of_philosophers;
	int		p_index = ((t_args *)args)->philo_index;

	(void)fork_left;
	(void)fork_right;

    if (num < 2)
		return (0);
	if (p_index < num - 1 && (*fork_ontable)[p_index + 1] == 1)
		right_available = 1;
	else if (p_index == num - 1 && (*fork_ontable)[0] == 1) /////take the last one
		right_available = 1;
	if ((*fork_ontable)[p_index] == 1)
		left_available = 1;
	if (!left_available || !right_available)
		return (0);
	return (1);
}

void take_forks(int *fork_right, int *fork_left, void *args)
{
	int		num = ((t_args *)args)->number_of_philosophers;
	int		p_index = ((t_args *)args)->philo_index;
	int		**fork_ontable =  &(((t_args *)args)->fork_ontable);

	if (p_index < num - 1 && (*fork_ontable)[p_index + 1] == 1)
	{
		(*fork_ontable)[p_index + 1] = 0;
		*fork_right = 1;
	}
	else if (p_index == num - 1 && (*fork_ontable)[0] == 1) /////take the last one
	{
		(*fork_ontable)[0] = 0;
		*fork_right = 1;
	}
	print_msg(FORK, args);
	if ((*fork_ontable)[p_index] == 1)
	{
		(*fork_ontable)[p_index] = 0;
		*fork_left = 1;
	}
	print_msg(FORK, args);
}

void return_forks(int *fork_right, int *fork_left, void *args)
{
	int		num;
	int		p_index;
	int		**fork_ontable;
	num = ((t_args *)args)->number_of_philosophers;
	p_index = ((t_args *)args)->philo_index;

	pthread_mutex_lock(((t_args *)args)->mutex_fork[p_index]);
	fork_ontable =  &(((t_args *)args)->fork_ontable);
	if (*fork_right == 1)
	{
		if (p_index < num - 1 && (*fork_ontable)[p_index + 1] == 0)
		{
			(*fork_ontable)[p_index + 1] = 1;
			*fork_right = 0;
		}
		else if (p_index == num - 1 && (*fork_ontable)[0] == 0) /////take the last one
		{
			(*fork_ontable)[0] = 1;
			*fork_right = 0;
		}
	}
	if (*fork_left == 1) ////return left
	{
		if ((*fork_ontable)[p_index] == 0)
		{
			(*fork_ontable)[p_index] = 1;
			*fork_left = 0;
		}
	}
	pthread_mutex_unlock(((t_args *)args)->mutex_fork[p_index]);
}
