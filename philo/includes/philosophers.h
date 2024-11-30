/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mblanc <mblanc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:22:47 by dmathis           #+#    #+#             */
/*   Updated: 2024/11/29 23:08:57 by mblanc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define DEAD 3

typedef struct s_data
{
    int                 number_of_philosophers;
    int                 number_of_time_each_philosopher_must_eat;
    int                 time_to_die;
    int                 time_to_eat;
    int                 time_to_sleep;
    int                 stop_simulation;
    pthread_mutex_t     stop_mutex;
    long                start_time;
    pthread_mutex_t     *forks;
    pthread_mutex_t     print_mutex;
    struct s_philosophers *philosophers;
    pthread_mutex_t     fork_mutex;
    pthread_t           *threads;        // Threads des philosophes
    pthread_t           verif_thread;    // Thread de vérification
} t_data;


typedef struct s_philosophers
{
	int						id;
	int what_doing;              // État actuel : manger, dormir, penser
	int number_of_time_he_eat;   // Nombre de repas
	long last_meal_time;         // Timestamp du dernier repas
	pthread_mutex_t state_mutex; // Mutex pour protéger l'état du philosophe
	pthread_mutex_t			*left_fork;
	// Pointeur vers le mutex de la fourchette gauche
	pthread_mutex_t			*right_fork;
	// Pointeur vers le mutex de la fourchette droite
	struct s_philosophers	*next;
	struct s_philosophers	*prev;
	t_data					*data;
}							t_philosophers;

t_data						*init_structures(int argc, char **argv);
void	ft_usleep(t_data *data, int duration);
void						print_status(t_data *data, t_philosophers *philo,
								char *status);
void						take_forks(t_data *data, t_philosophers *philo);
void						release_forks(t_philosophers *philo);
void						eat(t_data *data, t_philosophers *philo);
void						*philosopher_routine(void *arg);
int							create_philosopher_threads(t_data *data);
void						free_philosophers(t_philosophers *head, int count);
long						get_timestamp(void);
int							verif_all_philosophers_eat(t_data *data);
void						verif_threads(t_data *data);
int							verifications_philos(t_philosophers *philo,
								t_data *data);
int							get_stop_simulation(t_data *data);
void						set_stop_simulation(t_data *data, int value);
int	have_they_all_eat_necessary(t_data *data);
void	*verif_eat_limit(void *arg);
long	get_last_meal(t_philosophers *philo);
int	get_all_eat_necessary(t_data *data);
int get_number_of_time_he_eat(t_philosophers *philo);
int	parse_int(const char *str);

#endif
