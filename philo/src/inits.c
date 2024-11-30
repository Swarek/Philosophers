/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:09:52 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/30 02:14:05 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

static pthread_mutex_t	*init_mutex_fork(int number_of_philosophers)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* number_of_philosophers);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < number_of_philosophers)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

static t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		return (NULL);
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->number_of_philosophers = parse_int(argv[1]);
	data->time_to_die = parse_int(argv[2]);
	data->time_to_eat = parse_int(argv[3]);
	data->time_to_sleep = parse_int(argv[4]);
	data->number_of_time_each_philosopher_must_eat = -1;
	if (argc == 6)
		data->number_of_time_each_philosopher_must_eat = parse_int(argv[5]);
	data->stop_simulation = 0;
	data->philosophers = NULL;
	data->forks = NULL;
	if (data->number_of_philosophers <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (argc == 6
			&& data->number_of_time_each_philosopher_must_eat <= 0))
		return (free(data), NULL);
	return (data);
}

t_philosophers	*create_philosopher(int id, t_data *data)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)malloc(sizeof(t_philosophers));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->what_doing = 0;
	philo->number_of_time_he_eat = 0;
	philo->last_meal_time = data->start_time;
	philo->left_fork = &data->forks[id - 1];
	philo->right_fork = &data->forks[id % data->number_of_philosophers];
	if (pthread_mutex_init(&philo->state_mutex, NULL) != 0)
		return (free(philo), NULL);
	philo->next = NULL;
	philo->prev = NULL;
	philo->data = data;
	return (philo);
}

t_data	*init_structures(int argc, char **argv)
{
	t_data	*data;

	data = init_data(argc, argv);
	if (!data)
		return (printf("Error: Initialization failed\n"), NULL);
	data->forks = init_mutex_fork(data->number_of_philosophers);
	if (!data->forks)
		return (free(data), printf("Error: Forks initialization failed\n"),
			NULL);
	data->start_time = get_timestamp();
	if (!init_philosophers(data))
		return (free(data), printf("Philosophers initialization failed\n"),
			NULL);
	if (pthread_mutex_init(&data->fork_mutex, NULL) != 0)
		return (free(data), printf("Fork mutex initialization failed\n"), NULL);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (free(data), printf("Print mutex initialization failed\n"),
			NULL);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (free(data), printf("Stop mutex initialization failed\n"), NULL);
	if (create_philosopher_threads(data) != 0)
		return (free(data), printf("Could not create philosopher threads\n"),
			NULL);
	verif_threads(data);
	return (data);
}
