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

int	*init_fork_on_table(int num_philo)
{
	int		i;
	int		*fork_ontable;

	fork_ontable = malloc(sizeof(int) * num_philo);
	if (!fork_ontable)
		return (NULL);
	i = 0;
	while (i < num_philo)
	{
		fork_ontable[i] = 1;
		i++;
	}
	return (fork_ontable);
}

int	*init_philo_alive(int num_philo)
{
	int i;
	int	*alive;
	i = 0;
	alive = malloc(sizeof(int) * num_philo);
	if (!alive)
		return (NULL);
	while(i < num_philo)
	{
		alive[i] = 1;
		i++;
	}
	return (alive);
}

int *init_num_eaten(int num_philo)
{
	int i;
	int	*number_eaten;

	number_eaten = malloc(sizeof(int) * num_philo);
	i = 0;
	if (!number_eaten)
		return (NULL);
	while(i < num_philo)
	{
		number_eaten[i] = 0;
		i++;
	}
	return (number_eaten);
}

void init_vars_setup(t_args *args, char **av, int i)
{
	args->philo_index = i;
	args->number_of_philosophers = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]) * 1000;
	args->time_to_eat = ft_atoi(av[3]) * 1000;
	args->time_to_sleep = ft_atoi(av[4]) * 1000;
}

void init_vars_shared(t_args *args, pthread_mutex_t *mutex_printf, pthread_mutex_t **mutex_forks, int *alive, int *fork_ontable)
{
	args->mutex_printf = mutex_printf;
	args->number_eaten = 0;
	args->mutex_fork = mutex_forks;
	if (!args->mutex_fork)
		return ; /////////////protect
	args->fork_ontable = fork_ontable; /////////////////////protect
	args->alive = alive;
}

void init_sim_state(t_args *args, int *sim_end, time_t start_time, int num_must_eat, int *number_eaten)
{
	args->sim_end = sim_end;
	args->start_time = start_time;
	args->number_of_times_each_philosopher_must_eat = num_must_eat;
	args->number_eaten = number_eaten;
}

void init_vars_monitor(t_args_monitor *args_monitor, pthread_mutex_t *mutex_printf, int *alive, int *sim_end,int num_must_eat, int *number_eaten, int num)
{
	args_monitor->sim_end = sim_end;
	args_monitor->mutex_printf = mutex_printf;
	args_monitor->alive = alive;
	args_monitor->number_must_eat = num_must_eat;
	args_monitor->number_eaten = number_eaten;
	args_monitor->number_of_philosophers = num;
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


void	simulation(int ac, char **av)
{
	int num_must_eat;
	if (ac == 6)
		num_must_eat = ft_atoi(av[5]);
	else if (ac == 5)
		num_must_eat = -1;

	time_t start_time;

	start_time = now_time();
	int	num_philo = ft_atoi(av[1]);
	pthread_mutex_t mutex_printf;
	pthread_mutex_init(&mutex_printf, NULL);

	int i = 0;
	int	sim_end = 0;
	int	*fork_ontable = init_fork_on_table(num_philo);
	int	*alive = init_philo_alive(num_philo);
	int *number_eaten = init_num_eaten(num_philo);
	pthread_mutex_t **mutex_forks = init_mutex_forks(num_philo);
	/*----------------Set_vars_for_each_philo--------------------------*/
	void	**arg_tab; ///MAKE ARG_TAB 
	arg_tab = (void **)malloc(sizeof(t_args) * (num_philo + 1));
	if (arg_tab == NULL)
		return (1);

	i = 0;
	while (i < num_philo)
	{
		arg_tab[i] = (t_args *)malloc(sizeof(t_args));
		if (arg_tab[i] == NULL)
		{
			free_tab(arg_tab);
			return (1);
		}
		init_vars_setup((t_args *)arg_tab[i], av, i);
		init_vars_shared((t_args *)arg_tab[i], &mutex_printf, mutex_forks, alive, fork_ontable);
		init_sim_state((t_args *)arg_tab[i], &sim_end, start_time, num_must_eat, number_eaten);
		i++;
	}
	arg_tab[i] = NULL;
	/*----------------Set_vars_for_each_philo--------------------------*/
	/*----------------Create_threads--------------------------*/
	
	pthread_t	monitor;
	t_args_monitor monitor_vars;

	init_vars_monitor(&monitor_vars, &mutex_printf, alive, &sim_end, num_must_eat, number_eaten, num_philo);//ft_atoi(av[1]));
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);


	pthread_t	*philo;

	philo = malloc(sizeof(pthread_t) * num_philo);
	if (!philo)
		return (1);
	i = 0;
	while (i < num_philo)
	{
		pthread_create(&philo[i], NULL, &philo_func, arg_tab[i]);
		i++;
	}
	/*----------------Create_threads--------------------------*/
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);
	/*----------------Join_threads--------------------------*/
	pthread_join(monitor, NULL);
	i = 0;
	while (i < num_philo)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	
	/*----------------Join_threads--------------------------*/
	destroy_mutex_forks(num_philo, mutex_forks);
	/////pthread_mutex_destroy(&mutex_forks[k]);
	free_tab(arg_tab);//need to free tab
	free(fork_ontable);
}

int	main(int ac, char **av)
{
	/*----------------check_input_error--------------------------*/
	if (input_error(ac, av))
		return (1);
	/*----------------check_input_error--------------------------*/
	
	
	return (0);
}