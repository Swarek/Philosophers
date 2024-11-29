/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:05:02 by mblanc            #+#    #+#             */
/*   Updated: 2024/11/29 11:59:18 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cleanup(t_data *data)
{
    int i;

    // Attendre que tous les threads des philosophes se terminent
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_join(data->threads[i], NULL);
    }

    // Détruire les mutex des fourchettes
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_mutex_destroy(&data->forks[i]);
    }

    // Détruire les mutex dans la structure data
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->stop_mutex);
    pthread_mutex_destroy(&data->fork_mutex);

    // Libérer les ressources
    free_philosophers(data->philosophers, data->number_of_philosophers);
    free(data->forks);
    free(data->threads);
    free(data);
}




// Arg1 : number_of_philosophers
// Arg2 : time_to_die
// Arg3 : time_to_eat
// Arg4 : time_to_sleep
// Arg5 (optinal) : number_of_times_each_philosopher_must_eat
#include "philosophers.h"

int main(int argc, char **argv)
{
    t_data *data;

    data = init_structures(argc, argv);
    if (!data)
    {
        printf("Error: Initialization failed\n");
        return (1);
    }
    if (create_philosopher_threads(data) != 0)
    {
        cleanup(data);
        return (1);
    }
    cleanup(data);
    return (0);
}

