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

void init_vars(t_args *args, int ac, char **av, int i, pthread_mutex_t **mutex_forks, pthread_mutex_t *mutex_printf, int *fork_ontable, int *alive, int *sim_end)
{
	int	num_philos = ft_atoi(av[1]);

	args->philo_index = i;
	args->number_of_philosophers = num_philos;
	args->time_to_die = ft_atoi(av[2]) * 1000;
	args->time_to_eat = ft_atoi(av[3]) * 1000;
	args->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		args->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else if (ac == 5)
		args->number_of_times_each_philosopher_must_eat = -1;
	/*----------------initialize num eaten--------------------------*/
	int j = 0;////////////////////////////////////////////////////////////////////
	int	*number_eaten = malloc(sizeof(int) * num_philos);///////////////////////
	if (!number_eaten)/////////////////////////////////////////////////////////
		return ;//free;
	while(j < num_philos)
	{
		number_eaten[j] = 1;
		j++;
	}
	/*----------------initialize num eaten--------------------------*/
	args->mutex_printf = mutex_printf;
	args->number_eaten = 0;
	args->sim_end = sim_end;
	args->start_time = now_time();
	args->mutex_fork = mutex_forks;
	args->fork_ontable = fork_ontable; //////////////////////?????????????????????????????
	args->alive = alive;
}

void init_vars_monitor(t_args_monitor *args_monitor, pthread_mutex_t *mutex_printf, int *alive, int *sim_end, int num)
{
	args_monitor->sim_end = sim_end;
	args_monitor->mutex_printf = mutex_printf;
	args_monitor->alive = alive;
	args_monitor->number_of_philosophers = num;
}

int	*init_fork_on_table(int num_philo)
{
	int		j;
	int		*fork_ontable;

	fork_ontable = malloc(sizeof(int) * num_philo);
	if (!fork_ontable)
		return (NULL);
	j = 0;
	while (j < num_philo)
	{
		fork_ontable[j] = 1;
		j++;
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

int	main(int ac, char **av)
{
	int	num_philo = ft_atoi(av[1]);

	pthread_mutex_t mutex_printf;
	pthread_mutex_init(&mutex_printf, NULL);

	pthread_mutex_t **mutex_forks;
	mutex_forks = malloc((num_philo) * sizeof(pthread_mutex_t *));
	int i = 0;

	///protect
	int k = 0;
	while (k < num_philo)
	{
		mutex_forks[k] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex_forks[k], NULL);
		k++;
	}
	
	int	sim_end = 0;
	
	/*----------------check_input_error--------------------------*/
	if (input_error(ac, av))
		return (1);
	/*----------------check_input_error--------------------------*/

	/*----------------initialize forks on the table--------------------------*/
	int	*fork_ontable = init_fork_on_table(num_philo);
	/*----------------initialize forks on the table--------------------------*/

	/*----------------initialize alive state on the table--------------------------*/
	int	*alive = init_philo_alive(num_philo);
	/*----------------initialize alive state on the table--------------------------*/

	/*----------------Set_vars_for_each_philo--------------------------*/

	void	**arg_tab; ///MAKE ARG_TAB ON STACK
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
		init_vars((t_args *)arg_tab[i], ac, av, i, mutex_forks, &mutex_printf, fork_ontable, alive, &sim_end);
		i++;
	}
	arg_tab[i] = NULL;

	/*----------------Set_vars_for_each_philo--------------------------*/
	/*----------------Create_threads--------------------------*/
	pthread_t	philo[num_philo];
	pthread_t	monitor;
	t_args_monitor monitor_vars;

	init_vars_monitor(&monitor_vars, &mutex_printf, alive, &sim_end, 5);//ft_atoi(av[1]));
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);

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
	k = 0;

	while (k < num_philo)
	{
		pthread_mutex_destroy(mutex_forks[k]);
		k++;
	}
	/////pthread_mutex_destroy(&mutex_forks[k]);
	free_tab(arg_tab);//need to free tab
	free(fork_ontable);
	
	return (0);
}
