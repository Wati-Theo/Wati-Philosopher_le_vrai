/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Wati-Theo <wati-theo@protonmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 19:27:09 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/29 21:57:36 by Wati-Theo        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
	snitching(philo, FORK);
	if (philo->id == 1)
		pthread_mutex_lock(&philo->data->forks[philo->data->nb_philo - 1]);
	else
		pthread_mutex_lock(&philo->data->forks[philo->id - 2]);
	snitching(philo, FORK);
}

void	snitching(t_philo *philo, int choice)
{
	if (!check_if_dead(philo, 696))
		return ;
	pthread_mutex_lock(&philo->data->is_snitching);
	if (choice == 1)
		printf("%d %d is thinking\n",
			get_time_difference(philo->data->start_time), philo->id);
	else if (choice == 2)
		printf("%d %d has taken a fork\n",
			get_time_difference(philo->data->start_time), philo->id);
	else if (choice == 3)
		printf("%d %d is eating\n",
			get_time_difference(philo->data->start_time), philo->id);
	else if (choice == 4)
		printf("%d %d is sleeping\n",
			get_time_difference(philo->data->start_time), philo->id);
	else if (choice == 5)
		printf("%d %d died\n",
			get_time_difference(philo->data->start_time), philo->id);
	pthread_mutex_unlock(&philo->data->is_snitching);
}

void	*sleep_philo(t_philo *philo)
{
	if (!check_if_dead(philo, 696))
		return ((void *)0);
	snitching(philo, SLEEP);
	wati_usleep(philo->data, philo->data->time_to_sleep);
	return (think_philo(philo));
}

void	*eat_philo(t_philo *philo)
{
	if ((philo->data->nb_eat_max != -42 && !check_nb_eat(philo))
		|| !check_if_dead(philo, 0))
		return ((void *)0);
	lock_forks(philo);
	pthread_mutex_lock(&philo->data->last_meal_security);
	philo->last_meal = get_time_difference(philo->data->start_time);
	pthread_mutex_unlock(&philo->data->last_meal_security);
	snitching(philo, EAT);
	if ((get_time_difference(philo->data->start_time) + philo->data->time_to_eat
			- philo->last_meal)
		> (unsigned int)philo->data->time_to_die)
	{
		if (!check_if_dead(philo, 0))
			return ((void *)0);
		wati_usleep(philo->data, philo->data->time_to_die);
		pthread_mutex_lock(&philo->data->dead_check);
		philo->data->oh_no_a_dead++;
		pthread_mutex_unlock(&philo->data->dead_check);
		unlock_forks(philo);
		return ((void *)0);
	}
	wati_usleep(philo->data, philo->data->time_to_eat);
	unlock_forks(philo);
	pthread_mutex_lock(&philo->data->eat_check);
	philo->nb_eat++;
	pthread_mutex_unlock(&philo->data->eat_check);
	return (sleep_philo(philo));
}

void	*think_philo(void *arg)
{
	t_philo		*philo;

	philo = arg;
	if (!check_if_dead(philo, 696))
		return ((void *)0);
	snitching(philo, THINK);
	if (!(philo->id % 2) && get_time_difference(philo->data->start_time)
		< (unsigned int)(philo->data->time_to_sleep + philo->data->time_to_eat))
		wati_usleep(philo->data, 88);
	return (eat_philo(philo));
}
