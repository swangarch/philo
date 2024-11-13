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
