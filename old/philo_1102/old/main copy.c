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

void	*philo_func(void *args)
{
	time_t start_time = ((t_args *)args)->start_time;
	int p_index = ((t_args *)args)->philo_index;
	int n_must_eat = ((t_args *)args)->number_of_times_each_philosopher_must_eat;
	pthread_mutex_t mutex = *(((t_args *)args)->mutex);
	time_t last_eat_time = start_time;

	/*----------------initialize forks--------------------------*/
	int fork_left = 0;
	int fork_right = 0;
	/*----------------initialize forks--------------------------*/

	while(1)
	{
		/*----------------take forks--------------------------*/
		pthread_mutex_lock(&mutex);//-----
		if (forks_available(&fork_right, &fork_left, args))
		{
			take_fork_right(&fork_right, args);
			printf("%ld %d has taken a fork\n", timestamp(start_time), p_index + 1);
			take_fork_left(&fork_left, args);
			printf("%ld %d has taken a fork\n", timestamp(start_time), p_index + 1);
		}
		pthread_mutex_unlock(&mutex);//------
		/*----------------take forks--------------------------*/

		/*----------------philo eat--------------------------*/
		if (fork_right == 1 && fork_right == 1)
		{
			((t_args *)args)->alive[p_index] = philo_eat(&last_eat_time, args);
			if (!((t_args *)args)->alive[p_index])
				break ;
			if (n_must_eat >= 0 && ((t_args *)args)->number_eaten[p_index] >= n_must_eat)///////
				break ;/////////////////////////////////////////////////////////////////////////
		}
		/*----------------philo eat--------------------------*/

		/*----------------return forks--------------------------*/
		pthread_mutex_lock(&mutex);//------
		return_fork_right(&fork_right, args);
		return_fork_left(&fork_left, args);
		pthread_mutex_unlock(&mutex);//-------
		/*----------------return forks--------------------------*/

		/*----------------philo sleep--------------------------*/
		((t_args *)args)->alive[p_index] = philo_sleep(last_eat_time, args);
		if (!((t_args *)args)->alive[p_index])
			break;
		/*----------------philo sleep--------------------------*/

		/*----------------philo think--------------------------*/
		((t_args *)args)->alive[p_index] = philo_think(last_eat_time, &fork_right, &fork_left, args);
		if (!((t_args *)args)->alive[p_index])
			break;
		/*----------------philo think--------------------------*/
	}
	return (NULL);
}

int	check_sim_end(void *args)
{
	int	sim_end_flag;

	sim_end_flag = 0;
	sim_end_flag = *(((t_args *)args)->sim_end);
	return (sim_end_flag);
}

int check_alive(void *args_monitor)
{
	int i;
	int num;

	num = ((t_args_monitor *)args_monitor)->number_of_philosophers;
	i = 0;
	while (i < num)
	{
		if (((t_args_monitor *)args_monitor)->alive[i] == 0)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_func(void *args_monitor)
{
	while(1)
	{
		if (!check_alive(args_monitor))
		{
			*(((t_args_monitor *)args_monitor)->sim_end) = 1;
			break ;
		}
		usleep(WAIT_INTERVAL_MONITOR);
	}
	//printf("SIMULATION STOP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	return (NULL);
}

void init_vars(t_args *args, int ac, char **av, int i, pthread_mutex_t *mutex, int *fork_ontable, int *alive, int *sim_end)
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
	
	args->number_eaten = 0;
	args->sim_end = sim_end;
	args->start_time = now_time();
	args->mutex = mutex;
	args->fork_ontable = fork_ontable; //////////////////////?????????????????????????????
	args->alive = alive;
}

void init_vars_monitor(t_args_monitor *args_monitor, pthread_mutex_t *mutex, int *alive, int *sim_end, int num)
{
	args_monitor->sim_end = sim_end;
	args_monitor->mutex = mutex;
	args_monitor->alive = alive;
	args_monitor->number_of_philosophers = num;
}

int	main(int ac, char **av)
{
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	
	int	sim_end = 0;
	
	/*----------------check_input_error--------------------------*/
	if (input_error(ac, av))
		return (1);
	/*----------------check_input_error--------------------------*/

	int	num_philo = ft_atoi(av[1]);

	void	**arg_tab; ///MAKE ARG_TAB ON STACK
	arg_tab = (void **)malloc(sizeof(t_args) * (num_philo + 1));
	if (arg_tab == NULL)
		return (1);

	/*----------------initialize forks on the table--------------------------*/
	int	*fork_ontable = malloc(sizeof(int) * num_philo);
	if (!fork_ontable)
		return (1);
	
	int j = 0;
	while (j < num_philo)
	{
		fork_ontable[j] = 1;
		j++;
	}
	/*----------------initialize forks on the table--------------------------*/

	/*----------------Set_vars_for_each_philo--------------------------*/
	int i = 0;////////////////////////////////////////////////////////////////////
	int	*alive = malloc(sizeof(int) * num_philo);///////////////////////
	if (!alive)/////////////////////////////////////////////////////////
	{
		//free;
		return (1);
	}
	while(i < num_philo)
	{
		alive[i] = 1;
		i++;
	}
	i = 0;
	while (i < num_philo)
	{
		arg_tab[i] = (t_args *)malloc(sizeof(t_args));
		if (arg_tab[i] == NULL)
		{
			free_tab(arg_tab);
			return (1);
		}
		init_vars((t_args *)arg_tab[i], ac, av, i, &mutex, fork_ontable, alive, &sim_end);
		i++;
	}
	arg_tab[i] = NULL;
	/*----------------Create_threads--------------------------*/
	pthread_t	philo[num_philo];
	pthread_t	monitor;
	t_args_monitor monitor_vars;

	init_vars_monitor(&monitor_vars, &mutex, alive, &sim_end, 5);//ft_atoi(av[1]));
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
	pthread_mutex_destroy(&mutex);
	free_tab(arg_tab);//need to free tab
	free(fork_ontable);
	
	return (0);
}
