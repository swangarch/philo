/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 15:22:25 by shuwang           #+#    #+#             */
/*   Updated: 2024/08/10 15:22:26 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

int ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * num);
}

time_t	t_interval(time_t t_begin, time_t t_end)
{
	return (t_end - t_begin);
}

time_t	timestamp(time_t t_begin)
{
	return ((now_time()- t_begin) / 1000);
}

time_t	t_ms(struct timeval *tv)
{
	time_t time_ms;

	time_ms = tv->tv_sec * 1000000 + tv->tv_usec;
	return (time_ms);
}

time_t now_time(void) 
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (t_ms(&tv));
}
