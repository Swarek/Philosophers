/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:02:57 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/30 02:16:44 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_stop_simulation(t_data *data, int value)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop_simulation = value;
	pthread_mutex_unlock(&data->stop_mutex);
}

int	verif_all_philosophers_eat(t_data *data)
{
	t_philosophers	*philo;
	int				i;

	i = 0;
	philo = data->philosophers;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_lock(&philo->state_mutex);
		if (philo->number_of_time_he_eat < get_all_eat_necessary(data))
		{
			pthread_mutex_unlock(&philo->state_mutex);
			return (0);
		}
		{
			pthread_mutex_unlock(&philo->state_mutex);
			return (0);
		}
		pthread_mutex_unlock(&philo->state_mutex);
		philo = philo->next;
		i++;
	}
	pthread_mutex_lock(&data->stop_mutex);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->stop_mutex);
	return (1);
}

int	check_philosopher(t_data *data, t_philosophers *philo)
{
	long	time_diff;

	pthread_mutex_lock(&philo->state_mutex);
	time_diff = get_timestamp() - philo->last_meal_time;
	if (time_diff >= data->time_to_die)
	{
		print_status(data, philo, "died");
		set_stop_simulation(data, 1);
		pthread_mutex_unlock(&philo->state_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->state_mutex);
	return (0);
}

void	check_philosophers_state(t_data *data)
{
	int				i;
	t_philosophers	*philo;

	while (!get_stop_simulation(data))
	{
		philo = data->philosophers;
		i = 0;
		while (i < data->number_of_philosophers && !get_stop_simulation(data))
		{
			if (check_philosopher(data, philo))
				break ;
			philo = philo->next;
			i++;
		}
		usleep(1000);
	}
}

void	verif_threads(t_data *data)
{
	pthread_t	verif_thread;
	int			i;

	if (pthread_create(&verif_thread, NULL, verif_eat_limit, data) != 0)
	{
		set_stop_simulation(data, 1);
		printf("Error: Failed to create thread\n");
	}
	if (get_stop_simulation(data))
		return ;
	check_philosophers_state(data);
	pthread_join(verif_thread, NULL);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}
