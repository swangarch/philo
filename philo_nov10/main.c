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

int	create_thread(pthread_t	*philo, void **arg_tab, t_setup *set)
{
	int i;
	int num;

	num = set->num;
	i = 0;
	while (i < num)
	{
		pthread_create(&philo[i], NULL, &philo_func, arg_tab[i]);
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
	philo = malloc(sizeof(pthread_t) * set->num);
	if (!philo)
		return (write(2, "Error\n", 6), 0);
	init_args_monitor(&monitor_vars, set, state, mutexes);
	monitor_vars.philo = philo;
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);
	create_thread(philo, arg_tab, set);
	//ft_putstr_fd("simulation start\n", 2);
	pthread_join(monitor, NULL);
	//ft_putstr_fd("case03\n", 2);///////////////////////////////////////////
	free_tab(arg_tab);
	free(philo);
	return (1);
}

int philo_simulation(int ac, char **av)
{
	/*----------------setup--------------------------*/
	t_setup	set;
	t_state state;
	t_mutex mutexes;

	sim_setup(ac, av, &set);
	if (!state_setup(&set, &state))//destroy setup
		return (0);
	if (!mutex_setup(&set, &mutexes))//destroy &mutexe
		return (0);

	//printf("the death state is %d\n", *(state.death_flag));////////////////////////////////////////
	run_simulation(&set, &state, &mutexes);
	//ft_putstr_fd("case02\n", 2);///////////////////////////////////////////
	destroy_mutexes(&mutexes, set.num);
	destroy_state(&state);
 	return (1);
}

int	main(int ac, char **av)
{
	if (input_error(ac, av))
		return (1);
	if (!philo_simulation(ac, av))
		return (write(2, "Error\n", 6), 1);
	return (0);
}