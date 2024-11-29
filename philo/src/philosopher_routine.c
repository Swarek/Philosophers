/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:50:05 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/29 12:56:44 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_usleep(t_data *data, int duration)
{
	long	start_time;

	start_time = get_timestamp();
	while (get_timestamp() - start_time < duration)
	{
		if (get_stop_simulation(data))
			break ;
		usleep(500);
	}
}

void	print_status(t_data *data, t_philosophers *philo, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!get_stop_simulation(data))
		printf("%ld %d %s\n", get_timestamp() - data->start_time, philo->id,
			status);
	pthread_mutex_unlock(&data->print_mutex);
}

int have_they_all_eat_necessary(t_data *data)
{
    t_philosophers *memo;
    int i;

    memo = data->philosophers;
    if (get_all_eat_necessary(data) == -1)
        return (-1);
    i = 0;
    while (i < data->number_of_philosophers)
    {
        if (get_number_of_time_he_eat(memo)
            < get_all_eat_necessary(data))
        {
            return (0);
        }
        memo = memo->next;
        i++;
    }
    set_stop_simulation(data, 1);
    return (1);
}



void	*verif_eat_limit(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (have_they_all_eat_necessary(data) == -1)
		return (NULL);
	while (get_stop_simulation(data) != 1)
	{
		if (have_they_all_eat_necessary(data) == 1)
			return (NULL);
	}
	return (NULL);
}

void	take_forks(t_data *data, t_philosophers *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	int				left_fork_id;
	int				right_fork_id;

	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % data->number_of_philosophers;
	if (data->number_of_philosophers == 1)
	{
		print_status(data, philo, "has taken a fork");
		return ;
	}
	if (get_last_meal(philo) > get_last_meal(philo->next)
		|| get_last_meal(philo) > get_last_meal(philo->prev))
		ft_usleep(data, 50);
	if (left_fork_id < right_fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_status(data, philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(data, philo, "has taken a fork");
}

void	release_forks(t_philosophers *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
void eat(t_data *data, t_philosophers *philo)
{
    pthread_mutex_lock(&philo->state_mutex);
    if (get_stop_simulation(data) || 
        (philo->number_of_time_he_eat >= get_all_eat_necessary(data) 
        && get_all_eat_necessary(data) != -1))
    {
        pthread_mutex_unlock(&philo->state_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->state_mutex);

    take_forks(data, philo);

    if (data->number_of_philosophers == 1)
        return;

    pthread_mutex_lock(&philo->state_mutex);
    if (!get_stop_simulation(data))
    {
        philo->last_meal_time = get_timestamp();
        philo->number_of_time_he_eat++;
        print_status(data, philo, "is eating");
        pthread_mutex_unlock(&philo->state_mutex);
        ft_usleep(data, data->time_to_eat);
        release_forks(philo);
    }
    else
    {
        pthread_mutex_unlock(&philo->state_mutex);
        release_forks(philo);
    }
}

void	*philosopher_routine(void *arg)
{
	t_data			*data;
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
	{
		print_status(data, philo, "is thinking");
		ft_usleep(data, data->time_to_eat);
	}
	while (!get_stop_simulation(data))
	{
		eat(data, philo);
		if (get_stop_simulation(data))
			break ;
		print_status(data, philo, "is sleeping");
		ft_usleep(data, data->time_to_sleep);
		if (get_stop_simulation(data))
			break ;
		print_status(data, philo, "is thinking");
	}
	return (NULL);
}
