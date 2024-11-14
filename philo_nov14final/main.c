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

int	join_philo(pthread_t *philo, int num)
{
	int	i;

	i = 0;
	if (!philo || num < 1)
		return (0);
	while (i < num)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	return (1);
}

int	run_simulation(t_setup *set, t_state *state, t_mutex *mutexes)
{
	pthread_t	monitor;
	pthread_t	*philo;
	t_monitor	monitor_vars;
	void		**arg_tab;

	arg_tab = set_args_philo(set, state, mutexes);
	philo = malloc(sizeof(pthread_t) * set->num);
	if (!philo)
		return (ft_putstr_fd("Error: malloc failed\n", 2), 0);
	init_args_monitor(&monitor_vars, set, state, mutexes);
	if (pthread_create(&monitor, NULL, &monitor_func, &monitor_vars) != 0)
		return (ft_putstr_fd("Error: fail to create thread\n", 2), 0);
	if (!create_thread(philo, arg_tab, set))
		return (pthread_join(monitor, NULL), 0);
	join_philo(philo, set->num);
	pthread_join(monitor, NULL);
	free_tab(arg_tab);
	free(philo);
	return (1);
}

int	philo_simulation(int ac, char **av)
{
	t_setup	set;
	t_state	state;
	t_mutex	mutexes;

	sim_setup(ac, av, &set);
	if (!state_setup(&set, &state))
		return (ft_putstr_fd("Error: fail to setup\n", 2), 0);
	if (!mutex_setup(&set, &mutexes))
		return (ft_putstr_fd("Error: fail to setup mutex\n", 2), 0);
	if (!run_simulation(&set, &state, &mutexes))
		return (0);
	destroy_mutexes(&mutexes, set.num);
	destroy_state(&state);
	return (1);
}

int	main(int ac, char **av)
{
	if (input_error(ac, av))
		return (1);
	if (!philo_simulation(ac, av))
		return (1);
	return (0);
}
