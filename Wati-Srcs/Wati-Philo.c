/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 19:27:09 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 16:31:50 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

int	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
	if (!snitching(philo, FORK))
		return (0);
	if (philo->id == 1)
		pthread_mutex_lock(&philo->data->forks[philo->data->nb_philo - 1]);
	else
		pthread_mutex_lock(&philo->data->forks[philo->id - 2]);
	if (!snitching(philo, FORK))
		return (0);
	return (1);
}

int	snitching(t_philo *philo, int choice)
{
	pthread_mutex_lock(&philo->data->is_snitching);
	pthread_mutex_lock(&philo->data->dead_check);
	if (philo->data->oh_no_a_dead)
	{
		unlock_forks(philo);
		pthread_mutex_unlock(&philo->data->dead_check);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->dead_check);
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
	return (1);
}

void	*sleep_philo(t_philo *philo)
{
	if (philo->data->nb_eat_max != -42 && !check_nb_eat(philo))
		return ((void *)0);
	if (!snitching(philo, SLEEP))
		return ((void *)0);
	if (get_time_difference(philo->data->start_time)
		+ philo->data->time_to_sleep
		> (unsigned int)philo->data->time_to_die + philo->last_meal)
	{
		if (!wati_usleep(philo, philo->data->time_to_die
				- philo->data->time_to_eat))
			return ((void *)0);
		is_dead(philo);
		return ((void *)0);
	}
	if (!wati_usleep(philo, philo->data->time_to_sleep))
		return ((void *)0);
	return (think_philo(philo));
}

void	*eat_philo(t_philo *philo)
{
	if (philo->data->nb_eat_max != -42 && !check_nb_eat(philo))
		return ((void *)0);
	if (!lock_forks(philo))
		return (NULL);
	pthread_mutex_lock(&philo->data->last_meal_security);
	philo->last_meal = get_time_difference(philo->data->start_time);
	pthread_mutex_unlock(&philo->data->last_meal_security);
	if (!snitching(philo, EAT))
		return ((void *)0);
	if (!wati_usleep(philo, philo->data->time_to_eat))
	{
		unlock_forks(philo);
		return ((void *)0);
	}
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
	if (!snitching(philo, THINK))
		return ((void *)0);
	if (!(philo->id % 2) && get_time_difference(philo->data->start_time)
		< (unsigned int)(philo->data->time_to_sleep + philo->data->time_to_eat))
	{
		if (!wati_usleep(philo, 10))
			return ((void *)0);
	}
	return (eat_philo(philo));
}
