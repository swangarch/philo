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

# define WAIT_INTERVAL_MONITOR 100
# define MIN_THINK_TIME 1000

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3

# define DEAD 1
# define ALIVE 0

typedef struct s_setup
{
	int		num;
	time_t	time_to_die;
	time_t	time_to_eat;
	time_t	time_to_sleep;
	int		num_must_eat;
}	t_setup;

typedef struct s_state{
	int		*fork_ontable;
	time_t	*last_eat_time;
	int		*death_flag;
	int		*num_eaten;
	time_t	start_time;
}	t_state;

typedef struct s_mutex{
	pthread_mutex_t	*mtx_printf;
	pthread_mutex_t	*mtx_death;
	pthread_mutex_t	**mtx_fork;
	pthread_mutex_t	**mtx_eaten;
	pthread_mutex_t	**mtx_lasteat;
}	t_mutex;

typedef struct s_args
{
	int				idx;
	int				num;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				num_must_eat;
	int				*num_eaten;
	int				*death_flag;
	time_t			*last_eat_time;
	time_t			start_time;
	pthread_mutex_t	**mtx_fork;
	pthread_mutex_t	**mtx_eaten;
	pthread_mutex_t	**mtx_lasteat;
	pthread_mutex_t	*mtx_printf;
	pthread_mutex_t	*mtx_death;
}	t_args;

typedef struct s_monitor
{
	int				num;
	int				num_must_eat;
	int				*num_eaten;
	int				*death_flag;
	time_t			time_to_die;
	time_t			*last_eat_time;
	time_t			start_time;
	pthread_mutex_t	*mtx_death;
	pthread_mutex_t	**mtx_eaten;
	pthread_mutex_t	**mtx_lasteat;
	pthread_mutex_t	*mtx_printf;
}	t_monitor;

void			ft_putstr_fd(char *s, int fd);
int				ft_atoi(const char *nptr);
int				input_error(int ac, char **av);
size_t			ft_strlen(const char *s);
void			print_msg(int message, void *args);
void			print_death(void *args_monitor, int idx);
time_t			timestamp(time_t t_begin);
time_t			now_time(void);

void			sim_setup(int ac, char **av, t_setup *set);
int				state_setup(t_setup *set, t_state *state);
int				mutex_setup(t_setup *set, t_mutex *mutexes);
int				*create_tab(int num, int value);
time_t			*create_last_eat_time(int num, time_t last_eat_time);
pthread_mutex_t	**create_mutex_tab(int num);
void			init_args_philo(t_args *args, t_setup *set, t_state *state, \
	t_mutex *mutex);
void			**set_args_philo(t_setup *set, t_state *state, \
	t_mutex *mutexes);
void			init_args_monitor(t_monitor *args_monitor, t_setup *set, \
	t_state *state, t_mutex *mutex);

int				create_thread(pthread_t	*philo, void **arg_tab, t_setup *set);
int				lock_mutex_fork(void *args);
void			unlock_mutex_fork(void *args);
int				check_death(void *args);
void			*philo_func(void *args);
void			*monitor_func(void *args_monitor);
int				join_philo(pthread_t *philo, int num);

void			destroy_mutex_tab(pthread_mutex_t **mutex_forks, int num);
void			destroy_mutexes(t_mutex *mutexes, int num);
void			destroy_state(t_state *state);
void			free_mutex_tab(pthread_mutex_t **mutex_forks, int num);
void			free_tab(void **tab);

#endif
