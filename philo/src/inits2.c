/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 02:10:56 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/30 02:12:54 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	create_philosopher_threads(t_data *data)
{
	t_philosophers	*philo;
	int				i;

	data->threads = malloc(sizeof(pthread_t) * data->number_of_philosophers);
	if (!data->threads)
		return (1);
	philo = data->philosophers;
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		if (pthread_create(&data->threads[i], NULL, philosopher_routine,
				philo) != 0)
		{
			printf("Error: Failed to create philosopher thread\n");
			free(data->threads);
			return (1);
		}
		philo = philo->next;
	}
	return (0);
}

t_philosophers	*init_philosophers(t_data *data)
{
	t_philosophers	*head;
	t_philosophers	*current;
	int				i;

	head = create_philosopher(1, data);
	if (!head)
		return (NULL);
	current = head;
	i = 1;
	while (i < data->number_of_philosophers)
	{
		current->next = create_philosopher(i + 1, data);
		if (!current->next)
		{
			free_philosophers(head, i);
			return (NULL);
		}
		current->next->prev = current;
		current = current->next;
		i++;
	}
	current->next = head;
	head->prev = current;
	data->philosophers = head;
	return (head);
}
