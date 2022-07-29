/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Wati-Theo <wati-theo@protonmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 17:46:58 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/29 21:58:21 by Wati-Theo        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"
/*
** La wati-Freebox qui va free correctement promis 
*/
void	freebox(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philo[i].thread, NULL);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->eat_check);
	pthread_mutex_destroy(&data->is_snitching);
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

int	check_if_dead(t_philo *philo, int choice)
{
	pthread_mutex_lock(&philo->data->dead_check);
	if (philo->data->oh_no_a_dead)
	{	
		pthread_mutex_unlock(&philo->data->dead_check);
		if (!choice)
		{
			pthread_mutex_lock(&philo->data->is_snitching);
			unlock_forks(philo);
			printf("%d %doh_no_died\n",
				get_time_difference(philo->data->start_time), philo->id);
			pthread_mutex_unlock(&philo->data->is_snitching);
		}
		return (0);
	}
	pthread_mutex_unlock(&philo->data->dead_check);
	pthread_mutex_lock(&philo->data->last_meal_security);
	if ((get_time_difference(philo->data->start_time)
			- philo->last_meal)
		> (unsigned int)philo->data->time_to_die)
	{
		if (!choice)
			unlock_forks(philo);
		pthread_mutex_lock(&philo->data->is_snitching);
		printf("%d %d died\n",
			get_time_difference(philo->data->start_time), philo->id);
		pthread_mutex_unlock(&philo->data->is_snitching);
		pthread_mutex_lock(&philo->data->dead_check);
		philo->data->oh_no_a_dead++;
		pthread_mutex_unlock(&philo->data->dead_check);
		pthread_mutex_unlock(&philo->data->last_meal_security);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->last_meal_security);
	return (1);
}

int	check_nb_eat(t_philo *philo)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < philo->data->nb_philo)
	{
		pthread_mutex_lock(&philo->data->eat_check);
		if (philo->data->philo[i].nb_eat >= philo->data->nb_eat_max)
			count++;
		i++;
		pthread_mutex_unlock(&philo->data->eat_check);
	}
	if (count != philo->data->nb_philo)
		return (1);
	unlock_forks(philo);
	return (0);
}
