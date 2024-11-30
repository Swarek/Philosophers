/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:05:02 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/30 02:16:06 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->fork_mutex);
	free_philosophers(data->philosophers, data->number_of_philosophers);
	free(data->forks);
	free(data->threads);
	free(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	data = init_structures(argc, argv);
	if (!data)
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	if (create_philosopher_threads(data) != 0)
	{
		cleanup(data);
		return (1);
	}
	cleanup(data);
	return (0);
}
