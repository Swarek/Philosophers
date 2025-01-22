/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:50:05 by mblanc            #+#    #+#             */
/*   Updated: 2025/01/19 07:54:00 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	have_they_all_eat_necessary(t_data *data)
{
	t_philosophers	*memo;
	int				i;

	ft_usleep(data, 10);
	memo = data->philosophers;
	if (get_all_eat_necessary(data) == -1)
		return (-1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (get_number_of_time_he_eat(memo) < get_all_eat_necessary(data))
			return (0);
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

	if (data->number_of_philosophers == 1)
	{
		print_status(data, philo, "has taken a fork");
		return ;
	}
	if (philo->id - 1 < philo->id % data->number_of_philosophers)
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

void	eat(t_data *data, t_philosophers *philo)
{
	pthread_mutex_lock(&philo->state_mutex);
	if (get_stop_simulation(data)
		|| (philo->number_of_time_he_eat >= get_all_eat_necessary(data)
			&& get_all_eat_necessary(data) != -1))
	{
		pthread_mutex_unlock(&philo->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->state_mutex);
	take_forks(data, philo);
	if (data->number_of_philosophers == 1)
		return ;
	pthread_mutex_lock(&philo->state_mutex);
	if (!get_stop_simulation(data))
	{
		philo->last_meal_time = get_timestamp();
		philo->number_of_time_he_eat++;
		pthread_mutex_unlock(&philo->state_mutex);
		ft_usleep(data, data->time_to_eat);
		print_status(data, philo, "is eating");
	}
	else
		pthread_mutex_unlock(&philo->state_mutex);
	release_forks(philo);
	ft_usleep(data, 2);
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
