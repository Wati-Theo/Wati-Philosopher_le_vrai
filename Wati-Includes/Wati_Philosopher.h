/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati_Philosopher.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 17:45:25 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 19:24:54 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef WATI_PHILOSOPHER_H
# define WATI_PHILOSOPHER_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define THINK 1
# define FORK 2
# define EAT 3
# define SLEEP 4
# define DIED 5

typedef struct data		t_data;
typedef struct timeval	t_time;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				nb_eat;
	unsigned int	last_meal;
	t_data			*data;
}	t_philo;

typedef struct data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_eat_max;
	pthread_mutex_t	is_snitching;
	pthread_mutex_t	*forks;
	pthread_mutex_t	last_meal_security;
	pthread_mutex_t	eat_check;
	pthread_mutex_t	dead_check;
	int				oh_no_a_dead;
	t_philo			*philo;
	t_time			start_time;
}	t_data;

void			check_if_digit(int argc, char *argv[], t_data *data);
int				ft_isdigit(int c);
long long int	ft_atoi(const char *str);
void			init_data_and_forchetta(char *argv[], t_data *data);
void			*think_philo(void *arg);
void			*sleep_philo(t_philo *philo);
unsigned int	get_time_difference(struct timeval old_time);
int				wati_usleep(t_philo *philo, unsigned int sleep_time);
void			*freebox(t_data *data);
int				is_dead(t_philo *philo);
int				check_nb_eat(t_philo *philo);
int				snitching(t_philo *philo, int choice);
void			unlock_forks(t_philo *philo);
int				check_time(t_philo *philo);
int				special_arg(t_philo *philo, int choice);
void			*eat_philo(t_philo *philo);
void			snitching_choice(t_philo *philo, int choice);

#endif
