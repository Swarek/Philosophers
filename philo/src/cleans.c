/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleans.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:49:21 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/07 14:49:38 by mblanc           ###   ########.fr       */
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
