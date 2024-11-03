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

void destroy_mutex_forks(int num_philo, pthread_mutex_t **mutex_forks)
{
	int i = 0;

	while (i < num_philo)
	{
		pthread_mutex_destroy(mutex_forks[i]);
		i++;
	}
}