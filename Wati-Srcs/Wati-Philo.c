/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 19:27:09 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 19:25:16 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

int	lock_forks(t_philo *philo)
{
	if (!check_time(philo))
		return (0);
	pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
	if (!check_time(philo) || !snitching(philo, FORK))
		return (0);
	if (philo->id == 1)
	{
		if (!check_time(philo))
			return (0);
		pthread_mutex_lock(&philo->data->forks[philo->data->nb_philo - 1]);
	}
	else
	{
		if (!check_time(philo))
			return (0);
		pthread_mutex_lock(&philo->data->forks[philo->id - 2]);
	}
	if (!check_time(philo) || !snitching(philo, FORK))
		return (0);
	return (1);
}

int	snitching(t_philo *philo, int choice)
{
	if (!check_time(philo))
		return (0);
	pthread_mutex_lock(&philo->data->is_snitching);
	pthread_mutex_lock(&philo->data->dead_check);
	if (philo->data->oh_no_a_dead)
	{
		unlock_forks(philo);
		pthread_mutex_unlock(&philo->data->is_snitching);
		pthread_mutex_unlock(&philo->data->dead_check);
		return (0);
	}
	snitching_choice(philo, choice);
	pthread_mutex_unlock(&philo->data->is_snitching);
	return (1);
}

void	*sleep_philo(t_philo *philo)
{
	if (philo->data->nb_eat_max != -42 && !check_nb_eat(philo))
		return (NULL);
	if (!snitching(philo, SLEEP))
		return (NULL);
	pthread_mutex_lock(&philo->data->last_meal_security);
	if (get_time_difference(philo->data->start_time)
		+ philo->data->time_to_sleep
		> (unsigned int)philo->data->time_to_die + philo->last_meal)
	{
		pthread_mutex_unlock(&philo->data->last_meal_security);
		if (!wati_usleep(philo, philo->data->time_to_die
				- philo->data->time_to_eat))
			return (NULL);
		is_dead(philo);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->data->last_meal_security);
	if (!wati_usleep(philo, philo->data->time_to_sleep))
		return (NULL);
	return (think_philo(philo));
}

void	*eat_philo(t_philo *philo)
{
	if (!check_time(philo) || (philo->data->nb_eat_max != -42
			&& !check_nb_eat(philo)))
		return (NULL);
	if (!lock_forks(philo))
		return (NULL);
	pthread_mutex_lock(&philo->data->last_meal_security);
	philo->last_meal = get_time_difference(philo->data->start_time);
	pthread_mutex_unlock(&philo->data->last_meal_security);
	if (!snitching(philo, EAT))
		return (NULL);
	if (!wati_usleep(philo, philo->data->time_to_eat))
	{
		unlock_forks(philo);
		return (NULL);
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
		return (NULL);
	if (!special_arg(philo, 696))
		return (NULL);
	if (!check_time(philo))
		return (NULL);
	if (!special_arg(philo, 42))
		return (NULL);
	if (!(philo->id % 2) && get_time_difference(philo->data->start_time)
		< (unsigned int)(philo->data->time_to_sleep + philo->data->time_to_eat))
	{
		if (!wati_usleep(philo, 10))
			return (NULL);
	}
	return (eat_philo(philo));
}
