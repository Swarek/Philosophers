/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:02:57 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/29 10:39:32 by mblanc           ###   ########.fr       */
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

void verif_threads(t_data *data)
{
    int i;
    t_philosophers *philo;
    pthread_t verif_thread;
    long time_diff;

    if (pthread_create(&verif_thread, NULL, verif_eat_limit, data) != 0)
    {
        set_stop_simulation(data, 1);
        printf("Error: Failed to create thread\n");
        return;
    }

    while (!get_stop_simulation(data))
    {
        philo = data->philosophers;
        i = 0;
        while (i < data->number_of_philosophers && !get_stop_simulation(data))
        {
            pthread_mutex_lock(&philo->state_mutex);
            time_diff = get_timestamp() - philo->last_meal_time;
            if (time_diff >= data->time_to_die)
            {
                print_status(data, philo, "died");
                set_stop_simulation(data, 1);
                pthread_mutex_unlock(&philo->state_mutex);
                break;
            }
            pthread_mutex_unlock(&philo->state_mutex);
            philo = philo->next;
            i++;
        }
        usleep(1000);
    }
    pthread_join(verif_thread, NULL);

    // Join philosopher threads
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_join(data->threads[i], NULL);
    }
}

