/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_gets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:23:15 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/29 03:03:29 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	get_stop_simulation(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

long	get_last_meal(t_philosophers *philo)
{
	long	last_meal_time;

	pthread_mutex_lock(&philo->state_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->state_mutex);
	return (last_meal_time);
}

int	get_all_eat_necessary(t_data *data)
{
	int	verif;

	pthread_mutex_lock(&data->stop_mutex);
	verif = data->number_of_time_each_philosopher_must_eat;
	pthread_mutex_unlock(&data->stop_mutex);
	return (verif);
}

int	get_number_of_time_he_eat(t_philosophers *philo)
{
	int	number_of_time_he_eat;

	pthread_mutex_lock(&philo->state_mutex);
	number_of_time_he_eat = philo->number_of_time_he_eat;
	pthread_mutex_unlock(&philo->state_mutex);
	return (number_of_time_he_eat);
}
