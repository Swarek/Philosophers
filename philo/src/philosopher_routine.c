/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:50:05 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/19 09:41:40 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int duration)
{
	long	start_time;

	start_time = get_timestamp();
	while (get_timestamp() - start_time < duration)
		usleep(50);
}

void print_status(t_data *data, t_philosophers *philo, char *status)
{
    pthread_mutex_lock(&data->print_mutex);
    if (!get_stop_simulation(data))
        printf("%ld %d %s\n", get_timestamp() - data->start_time, philo->id, status);
    pthread_mutex_unlock(&data->print_mutex);
}

void	have_they_all_eat_necessary(t_data *data)
{
	t_philosophers	*memo;

	memo = data->philosophers->next;
	if (data->number_of_time_each_philosopher_must_eat == -1)
		return ;
	while (data->philosophers != memo)
	{
		if (memo->number_of_time_he_eat
			< data->number_of_time_each_philosopher_must_eat)
			return ;
		memo = memo->next;
	}
	set_stop_simulation(data, 1);
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
		pthread_mutex_lock(philo->left_fork);
		print_status(data, philo, "has taken a fork");
		return ;
	}
	if (philo->last_meal_time > philo->next->last_meal_time
		|| philo->last_meal_time > philo->prev->last_meal_time)
		ft_usleep(50);
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

void	eat(t_data *data, t_philosophers *philo)
{
	take_forks(data, philo);
	if (data->number_of_philosophers == 1)
		return ;
	pthread_mutex_lock(&philo->state_mutex);
	philo->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&philo->state_mutex);
	print_status(data, philo, "is eating");
	ft_usleep(data->time_to_eat);
	philo->number_of_time_he_eat++;
	release_forks(philo);
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
		ft_usleep(data->time_to_eat);
	}
	while (!get_stop_simulation(data))
	{
		eat(data, philo);
		if (get_stop_simulation(data))
			break ;
		print_status(data, philo, "is sleeping");
		ft_usleep(data->time_to_sleep);
		if (get_stop_simulation(data))
			break ;
		print_status(data, philo, "is thinking");
	}
	return (NULL);
}
