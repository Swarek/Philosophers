/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:50:05 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/13 17:55:30 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int duration)
{
	long	start_time;
	long	current_time;

	start_time = get_timestamp();
	while ((current_time = get_timestamp() - start_time) < duration)
	{
		usleep(100);
	}
}

void	print_status(t_data *data, t_philosophers *philo, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->stop_simulation)
		printf("%ld %d %s\n", get_timestamp() - data->start_time, philo->id,
			status);
	pthread_mutex_unlock(&data->print_mutex);
}

void take_forks(t_data *data, t_philosophers *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
    int first_locked;

    if (data->number_of_philosophers == 1)
    {
        // Same as before
    }

    // Determine fork order based on philosopher ID
    if (philo->id % 2 == 0)
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    while (1)
    {
        pthread_mutex_lock(first_fork);
        first_locked = 1;

        if (pthread_mutex_trylock(second_fork) == 0)
        {
            print_status(data, philo, "has taken a fork");
            print_status(data, philo, "has taken a fork");
            break;
        }
        else
        {
            pthread_mutex_unlock(first_fork);
            first_locked = 0;
            ft_usleep(10); // Short delay
        }
    }
}


// void	take_forks(t_data *data, t_philosophers *philo)
// {
// 	pthread_mutex_lock(philo->left_fork);
// 	print_status(data, philo, "has taken a fork");
// 	if (data->number_of_philosophers == 1)
// 		return ;
// 	pthread_mutex_lock(philo->right_fork);
// 	print_status(data, philo, "has taken a fork");
// }

void	release_forks(t_philosophers *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eat(t_data *data, t_philosophers *philo)
{
	take_forks(data, philo);
	if (data->number_of_philosophers == 1)
		return ;
	pthread_mutex_lock(&philo->state_mutex);
	philo->last_meal_time = get_timestamp();
	philo->number_of_time_he_eat++;
	print_status(data, philo, "is eating");
	pthread_mutex_unlock(&philo->state_mutex);
	ft_usleep(data->time_to_eat);
	release_forks(philo);
}

void	*philosopher_routine(void *arg)
{
	t_data			*data;
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	data = philo->data;
	if (philo->id % 2 == 0 && data->number_of_philosophers != 1)
		ft_usleep(10);
	while (!get_stop_simulation(data))
	{
		eat(data, philo);
		if (data->stop_simulation == 1)
			break ;
		print_status(data, philo, "is sleeping");
		if (data->stop_simulation == 1)
			break ;
		ft_usleep(data->time_to_sleep);
		print_status(data, philo, "is thinking");
		if (data->stop_simulation == 1)
			break ;
	}
	return (NULL);
}
