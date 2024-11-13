/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:02:57 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/13 17:40:58 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_stop_simulation(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

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
		if (philo->number_of_time_he_eat < data->number_of_time_each_philosopher_must_eat)
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

// int	verifications_philos(t_philosophers *philo, t_data *data)
// {
// 	int	is_dead;

// 	pthread_mutex_lock(&philo->state_mutex);
// 	is_dead = (get_timestamp() - philo->last_meal_time >= data->time_to_die);
// 	pthread_mutex_unlock(&philo->state_mutex);
// 	if (is_dead)
// 	{
// 		pthread_mutex_lock(&data->print_mutex);
// 		if (!data->stop_simulation)
// 		{
// 			print_status(data, philo, "died");
// 			pthread_mutex_lock(&data->stop_mutex);
// 			data->stop_simulation = 1;
// 			pthread_mutex_unlock(&data->stop_mutex);
// 		}
// 		pthread_mutex_unlock(&data->print_mutex);
// 		return (1);
// 	}
// 	if (data->number_of_time_each_philosopher_must_eat == -1)
// 		return (0);
// 	return (verif_all_philosophers_eat(data));
// }

void	verif_threads(t_data *data)
{
	int				i;
	t_philosophers	*philo;
	long			time_diff;

	while (!get_stop_simulation(data))
	{
		philo = data->philosophers;
		i = 0;
		while (i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&philo->state_mutex);
			time_diff = get_timestamp() - philo->last_meal_time;
			if (time_diff >= data->time_to_die && !get_stop_simulation(data))
			{
				print_status(data, philo, "died");
				set_stop_simulation(data, 1);
				pthread_mutex_unlock(&philo->state_mutex);
				return ;
			}
			pthread_mutex_unlock(&philo->state_mutex);
			philo = philo->next;
			i++;
		}
		ft_usleep(50); // Réduire le délai de sommeil pour une vérification plus fréquente
	}
}
