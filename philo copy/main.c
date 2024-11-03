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

	num = set->number_of_philosophers;
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
	philo = malloc(sizeof(pthread_t) * set->number_of_philosophers);
	if (!philo)
		return (write(2, "Error\n", 6), 0);
	init_args_monitor(&monitor_vars, set, state, mutexes);
	monitor_vars.philo = philo;
	pthread_create(&monitor, NULL, &monitor_func, &monitor_vars);
	create_thread(philo, arg_tab, set);
	/*----------------Create_threads--------------------------*/

	/*----------------Join_threads--------------------------*/
	pthread_join(monitor, NULL);
	/*----------------Join_threads--------------------------*/

	//printf("SIMULATION STOP!!!!!\n");

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
	//destroy state  //destroy setup
 	return (0);
}

int	main(int ac, char **av)
{
	if (input_error(ac, av))
		return (1);
	philo_simulation(ac, av);
	return (0);
}