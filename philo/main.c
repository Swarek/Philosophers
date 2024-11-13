/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:05:02 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/11 11:34:46 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	cleanup(t_data *data)
{
	int				i;
	t_philosophers	*philo;
	t_philosophers	*next_philo;

	if (!data)
		return (1);
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_join(data->threads[i++], NULL);
	free(data->threads);
	pthread_mutex_destroy(&data->print_mutex);
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_destroy(&data->forks[i++]);
	free(data->forks);
	philo = data->philosophers;
	i = 0;
	while (i < data->number_of_philosophers)
	{
		next_philo = philo->next;
		pthread_mutex_destroy(&philo->state_mutex);
		free(philo);
		philo = next_philo;
		i++;
	}
	free(data);
	return (0);
}

// Arg1 : number_of_philosophers
// Arg2 : time_to_die
// Arg3 : time_to_eat
// Arg4 : time_to_sleep
// Arg5 (optinal) : number_of_times_each_philosopher_must_eat
int	main(int argc, char **argv)
{
	t_data			*data;

	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments\n"));
	data = init_structures(argc, argv);
	if (!data)
		return (cleanup(data));
	return (cleanup(data));
}
