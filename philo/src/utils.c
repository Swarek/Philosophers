/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:49:21 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/30 02:03:32 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_philosophers(t_philosophers *head, int count)
{
	t_philosophers	*current;
	t_philosophers	*next;
	int				i;

	current = head;
	i = 0;
	while (current && i < count)
	{
		next = current->next;
		pthread_mutex_destroy(&current->state_mutex);
		free(current);
		current = next;
		i++;
	}
}

int	parse_int(const char *str)
{
	int	value;

	value = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		value = value * 10 + (*str - '0');
		str++;
	}
	return (value);
}

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
