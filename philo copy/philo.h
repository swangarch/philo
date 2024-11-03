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
#define WAIT_INTERVAL_MONITOR 0
#define TIME_TO_THINK 1000//////////////////////XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#define FORK 0
#define EAT 1
#define SLEEP 2
#define THINK 3
#define DEAD 4

#define _DEAD 0
#define _ALIVE 1
//#define _SIMEND 2

typedef struct s_setup
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_setup;

typedef struct s_state{
	int	*fork_ontable;
	int	*alive;
	int *number_eaten;
	int *sim_end;
	time_t start_time;
} t_state;

typedef struct s_mutex{
	pthread_mutex_t *mutex_printf;
	pthread_mutex_t **mutex_forks;
} t_mutex;

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
	int number_must_eat;
	int	*number_eaten;
	int	*sim_end;
	pthread_mutex_t *mutex_printf;
	int *alive;
	pthread_t *philo;
}	t_args_monitor;



int	all_digits(char *s);
int	int_overflow(const char *nptr, size_t i, int sign, long num);
void    ft_putstr_fd(char *s, int fd);
int	input_error(int ac, char **av);
int ft_atoi(const char *nptr);
int	input_error(int ac, char **av);
size_t	ft_strlen(const char *s);
//void init_vars(t_args *args, int ac, char **av, int i, pthread_mutex_t *mutex_printf, int *alive, int *sim_end, time_t start_time);
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
int join_philo(pthread_t *philo, int num);

void sim_setup(int ac, char **av, t_setup *set);
void state_setup(t_setup *set, t_state *state);
void mutex_setup(t_setup *set, t_mutex *mutexes);

void init_args_philo(t_args *args, t_setup *set, t_state *state, t_mutex *mutex);
void **set_args_philo(t_setup *set, t_state *state, t_mutex *mutexes);
void init_args_monitor(t_args_monitor *args_monitor, t_setup *set, t_state *state, t_mutex *mutex);

void destroy_mutex_forks(int num_philo, pthread_mutex_t **mutex_forks);

#endif
