/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:32:50 by shuwang           #+#    #+#             */
/*   Updated: 2024/08/10 14:58:48 by shuwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t **init_mutex_forks(int num_philo)
{
	pthread_mutex_t **mutex_forks;
	mutex_forks = malloc((num_philo) * sizeof(pthread_mutex_t *));
	if (!mutex_forks)
		return (NULL);

	int i = 0;
	while (i < num_philo)
	{
		mutex_forks[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
}

int	*init_tab(int num_philo, int value)
{
	int		i;
	int		*tab;

	tab = malloc(sizeof(int) * num_philo);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < num_philo)
	{
		tab[i] = value;
		i++;
	}
	return (tab);
}

void destroy_mutex_forks(int num_philo, pthread_mutex_t **mutex_forks)
{
	int i = 0;

	while (i < num_philo)
	{
		pthread_mutex_destroy(mutex_forks[i]);
		i++;
	}
}

void sim_setup(int ac, char **av, t_setup *set)
{
	set->number_of_philosophers = ft_atoi(av[1]);
	set->time_to_die = ft_atoi(av[2]) * 1000;
	set->time_to_eat = ft_atoi(av[3]) * 1000;
	set->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		set->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else if (ac == 5)
		set->number_of_times_each_philosopher_must_eat = -1;
}

typedef struct s_state{
	int	*fork_ontable;
	int	*alive;                  //0 die 1 alive 2 sim_end
	int *number_eaten;
	int *sim_end;
	time_t start_time;
} t_state;


void state_setup(t_setup *set, t_state *state)
{
	int num;

	num = set->number_of_philosophers;
	state->fork_ontable = init_tab(num, 1);  //protect
	state->alive = init_tab(num, _ALIVE);
	state->number_eaten = init_tab(num, 0);
	state->sim_end = malloc(sizeof(int));
	if(!state->sim_end)
		return ;   ///////////////////////////can be removed
	*(state->sim_end) = 0;
	state->start_time = now_time();
}

typedef struct s_mutex{
	pthread_mutex_t *mutex_printf;
	pthread_mutex_t **mutex_forks;
} t_mutex;

void mutex_setup(t_setup *set, t_mutex *mutexes)
{
	int num;

	num = set->number_of_philosophers;
	mutexes->mutex_printf = malloc(sizeof(pthread_mutex_t));
	if (!mutexes->mutex_printf)
		return ;////////////protect
	pthread_mutex_init(mutexes->mutex_printf, NULL);
	mutexes->mutex_forks = init_mutex_forks(num);//protect
}

void init_args_philo(t_args *args, t_setup *set, t_state *state, t_mutex *mutex)
{
	if (!args || !set || !state || !mutex)
		return ;
	//args->philo_index = index;
	args->number_of_philosophers = set->number_of_philosophers;
	args->time_to_die = set->time_to_die;
	args->time_to_eat = set->time_to_eat;
	args->time_to_sleep = set->time_to_sleep;
	args->number_of_times_each_philosopher_must_eat = set->number_of_times_each_philosopher_must_eat;

	args->fork_ontable = state->fork_ontable;
	args->alive = state->alive;
	args->number_eaten = state->number_eaten;
	args->sim_end = state->sim_end;
	args->start_time = state->start_time;

	args->mutex_fork = mutex->mutex_forks;
	args->mutex_printf = mutex->mutex_printf;
}

void **set_args_philo(t_setup *set, t_state *state, t_mutex *mutexes)
{
	int i;
	void	**arg_tab;
	int num;

	num = set->number_of_philosophers;
	arg_tab = (void **)malloc(sizeof(t_args*) * (num + 1));
	if (arg_tab == NULL)
		return (NULL);
	i = 0;
	while (i < num)
	{
		arg_tab[i] = (t_args *)malloc(sizeof(t_args));
		if (arg_tab[i] == NULL)
		{
			free_tab(arg_tab);
			return (NULL);
		}
		((t_args*)arg_tab[i])->philo_index = i;
		init_args_philo(arg_tab[i], set, state, mutexes);
		i++;
	}
	arg_tab[i] = NULL;
	return (arg_tab);
}

void init_args_monitor(t_args_monitor *args_monitor, t_setup *set, t_state *state, t_mutex *mutex)
{
	if (!args_monitor || !set || !state || !mutex)
		return ;
	args_monitor->sim_end = state->sim_end;
	args_monitor->mutex_printf = mutex->mutex_printf;
	args_monitor->alive = state->alive;
	args_monitor->number_must_eat = set->number_of_times_each_philosopher_must_eat;
	args_monitor->number_eaten = state->number_eaten;
	args_monitor->number_of_philosophers = set->number_of_philosophers;
}

int	create_thread(pthread_t	*philo, void **arg_tab, t_setup *set)
{
	int i;
	int num;

	num = set->number_of_philosophers;
	philo = malloc(sizeof(pthread_t) * num);
	if (!philo)
		return (0);
	i = 0;
	while (i < num)
	{
		pthread_create(&philo[i], NULL, &philo_func, arg_tab[i]);
		i++;
	}
	return (1);
}

int join_philo(pthread_t *philo, t_setup *set)
{
	int i;

	i = 0;
	if (!philo || !set)
		return (0);
	while (i < set->number_of_philosophers)
	{
		pthread_join(philo[i], NULL);  ///check
		i++;
	}
	return (1);
}

int run_simulation(t_setup *set, t_state *state, t_mutex *mutexes)
{
	pthread_t		monitor;
	pthread_t		*philo;
	t_args_monitor	monitor_vars;
	void			**arg_tab;

	/*----------------Set_vars_for_each_philo--------------------------*/
	arg_tab = set_args_philo(set, state, mutexes); 

	/*----------------Create_threads--------------------------*/
	init_args_monitor(&monitor_vars, set, state, mutexes);
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);
	philo = NULL;
	create_thread(philo, arg_tab, set);
	/*----------------Create_threads--------------------------*/

	/*----------------Join_threads--------------------------*/
	join_philo(philo, set);
	pthread_join(monitor, NULL);
	/*----------------Join_threads--------------------------*/

	free_tab(arg_tab);
	/*----------------destroy_vars--------------------------*/
	return (1);
}

int philo_simulation(int ac, char **av)
{
	/*----------------setup--------------------------*/
	t_setup	set;
	t_state state;
	t_mutex mutexes;

	sim_setup(ac, av, &set);
	state_setup(&set, &state);
	mutex_setup(&set, &mutexes);

	/*----------------Set_vars_for_each_philo--------------------------*/
	run_simulation(&set, &state, &mutexes);

	/*----------------Join_threads--------------------------*/
	//destroy_mutex_forks(num_philo, mutex_forks);
	/////pthread_mutex_destroy(&mutex_forks[k]);
	//need to free tab
	//free(fork_ontable);
	return (0);
}

int	main(int ac, char **av)
{
	if (input_error(ac, av))
		return (1);
	philo_simulation(ac, av);
	return (0);
}