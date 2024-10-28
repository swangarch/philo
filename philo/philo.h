/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:32:57 by shuwang           #+#    #+#             */
/*   Updated: 2024/08/09 14:41:00 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <limits.h>

#define WAIT_INTERVAL 1
#define WAIT_INTERVAL_MONITOR 1
#define TIME_TO_THINK 1000//////////////////////XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#define FORK 0
#define EAT 1
#define SLEEP 2
#define THINK 3
#define DEAD 4

typedef struct s_args
{
	int	philo_index;
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
	int	*number_eaten;
	int	*sim_end;
	int	*fork_ontable;
	int *alive;
	time_t	start_time;
	pthread_mutex_t **mutex_fork;
	pthread_mutex_t *mutex_printf;
}	t_args;

typedef struct s_args_monitor
{
	int	number_of_philosophers;
	int	*number_eaten;
	int	*sim_end;
	pthread_mutex_t *mutex_printf;
	int *alive;
}	t_args_monitor;

int	all_digits(char *s);
int	int_overflow(const char *nptr, size_t i, int sign, long num);
void    ft_putstr_fd(char *s, int fd);
int	input_error(int ac, char **av);
int ft_atoi(const char *nptr);
int	input_error(int ac, char **av);
size_t	ft_strlen(const char *s);
void init_vars(t_args *args, int ac, char **av, int i, pthread_mutex_t *mutex_printf, int *alive, int *sim_end);
void	free_tab(void **tab);

time_t	t_interval(time_t t_begin, time_t t_end);
time_t	timestamp(time_t t_begin);
time_t	t_ms(struct timeval *tv);
time_t now_time(void);

int	check_sim_end(void *args);

int forks_available(int *fork_right, int *fork_left, void *args);
void take_forks(int *fork_right, int *fork_left, void *args);
void return_forks(int *fork_right, int *fork_left, void *args);

int die(time_t last_eat_time, time_t time_to_die);
int philo_eat(time_t *last_eat_time, void *args);
int philo_sleep(time_t last_eat_time, void *args);
int philo_think(time_t last_eat_time, int *fork_right, int *fork_left, void *args);

void	*philo_func(void *args);
void	*monitor_func(void *args_monitor);

void    print_msg(int message, void *args);

#endif
