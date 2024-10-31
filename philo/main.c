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

// int	*init_fork_on_table(int num_philo)
// {
// 	int		i;
// 	int		*fork_ontable;

// 	fork_ontable = malloc(sizeof(int) * num_philo);
// 	if (!fork_ontable)
// 		return (NULL);
// 	i = 0;
// 	while (i < num_philo)
// 	{
// 		fork_ontable[i] = 1;
// 		i++;
// 	}
// 	return (fork_ontable);
// }

// int	*init_philo_alive(int num_philo)
// {
// 	int i;
// 	int	*alive;
// 	i = 0;
// 	alive = malloc(sizeof(int) * num_philo);
// 	if (!alive)
// 		return (NULL);
// 	while(i < num_philo)
// 	{
// 		alive[i] = 1;
// 		i++;
// 	}
// 	return (alive);
// }

// int *init_num_eaten(int num_philo)
// {
// 	int i;
// 	int	*number_eaten;

// 	number_eaten = malloc(sizeof(int) * num_philo);
// 	i = 0;
// 	if (!number_eaten)
// 		return (NULL);
// 	while(i < num_philo)
// 	{
// 		number_eaten[i] = 0;
// 		i++;
// 	}
// 	return (number_eaten);
// }


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

// typedef struct s_setup{
// 	number_of_philosophers;
// 	time_to_die;
// 	time_to_eat;
// 	time_to_sleep;
// 	number_of_times_each_philosopher_must_eat;
// } t_setup;

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
	state->alive = init_tab(num, 1);
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

void init_args_philo(t_args *args, t_setup *set, t_state *state, t_mutex *mutex, int index)
{
	if (!args || !set || !state || !mutex)
		return ;
	args->philo_index = index;
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
		init_args_philo(arg_tab[i], set, state, mutexes, i);
		i++;
	}
	arg_tab[i] = NULL;
	return (arg_tab);
}


int run_simulation(int ac, char **av)
{
	/*----------------check_input_error--------------------------*/
	t_setup	set;
	t_state state;
	t_mutex mutexes;
	sim_setup(ac, av, &set);
	state_setup(&set, &state);
	mutex_setup(&set, &mutexes);



	int num_must_eat;
	num_must_eat = set.number_of_times_each_philosopher_must_eat;
	int	num_philo = ft_atoi(av[1]);

	pthread_mutex_t mutex_printf;
	// pthread_mutex_t **mutex_forks;
	pthread_mutex_init(&mutex_printf, NULL);
	// mutex_forks = init_mutex_forks(num_philo);

	int	sim_end = 0;
	int	*alive = state.alive;
	int *number_eaten = state.number_eaten;


	/*----------------Set_vars_for_each_philo--------------------------*/

	void	**arg_tab = set_args_philo(&set, &state, &mutexes); 

	// int i;
	// void	**arg_tab; 

	// arg_tab = (void **)malloc(sizeof(t_args) * (num_philo + 1));
	// if (arg_tab == NULL)
	// 	return (1);
	// i = 0;
	// while (i < num_philo)
	// {
	// 	arg_tab[i] = (t_args *)malloc(sizeof(t_args));
	// 	if (arg_tab[i] == NULL)
	// 	{
	// 		free_tab(arg_tab);
	// 		return (1);
	// 	}
	// 	init_args_philo(arg_tab[i], &set, &state, &mutexes, i);
	// 	i++;
	// }
	// arg_tab[i] = NULL;
	/*----------------Set_vars_for_each_philo--------------------------*/
	/*----------------Create_threads--------------------------*/
	
	int i;
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
	//destroy_mutex_forks(num_philo, mutex_forks);
	/////pthread_mutex_destroy(&mutex_forks[k]);
	free_tab(arg_tab);//need to free tab
	//free(fork_ontable);
	return (0);
}

int	main(int ac, char **av)
{
	if (input_error(ac, av))
		return (1);
	run_simulation(ac, av);
	return (0);
}