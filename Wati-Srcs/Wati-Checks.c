/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 17:46:58 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 19:48:58 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"
/*
** La wati-Freebox qui va free correctement promis 
*/
void	*freebox(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].thread, NULL);
	i = -1;
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->is_snitching);
	pthread_mutex_destroy(&data->last_meal_security);
	pthread_mutex_destroy(&data->eat_check);
	pthread_mutex_destroy(&data->dead_check);
	return (NULL);
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
	if (philo->id == 1)
		pthread_mutex_unlock
			(&philo->data->forks[philo->data->nb_philo - 1]);
	else
		pthread_mutex_unlock(&philo->data->forks[philo->id - 2]);
}

int	is_dead(t_philo *philo)
{
	unlock_forks(philo);
	pthread_mutex_lock(&philo->data->is_snitching);
	pthread_mutex_lock(&philo->data->dead_check);
	if (philo->data->oh_no_a_dead)
	{
		pthread_mutex_unlock(&philo->data->is_snitching);
		pthread_mutex_unlock(&philo->data->dead_check);
		return (0);
	}
	philo->data->oh_no_a_dead++;
	pthread_mutex_unlock(&philo->data->dead_check);
	printf("%d %d died\n",
		get_time_difference(philo->data->start_time), philo->id);
	pthread_mutex_unlock(&philo->data->is_snitching);
	return (0);
}

int	check_nb_eat(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->data->eat_check);
	if (philo->nb_eat >= philo->data->nb_eat_max)
	{
		unlock_forks(philo);
		pthread_mutex_unlock(&philo->data->eat_check);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->eat_check);
	return (1);
}
