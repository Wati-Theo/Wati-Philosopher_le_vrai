/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:59:41 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 19:27:43 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

int	check_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->last_meal_security);
	if (get_time_difference(philo->data->start_time)
		>= (unsigned int)philo->data->time_to_die + philo->last_meal)
	{
		pthread_mutex_unlock(&philo->data->last_meal_security);
		is_dead(philo);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->last_meal_security);
	return (1);
}

int	wati_usleep(t_philo *philo, unsigned int sleep_time)
{
	struct timeval	s_start_time;

	gettimeofday(&s_start_time, NULL);
	while (sleep_time > get_time_difference(s_start_time))
	{
		if (!check_time(philo))
			return (0);
		pthread_mutex_lock(&philo->data->dead_check);
		if (philo->data->oh_no_a_dead)
		{	
			pthread_mutex_unlock(&philo->data->dead_check);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->dead_check);
		usleep(60);
	}
	return (1);
}

unsigned int	get_time_difference(struct timeval old_time)
{
	struct timeval	s_current_time;
	unsigned int	time_current;
	unsigned int	time_old;

	gettimeofday(&s_current_time, NULL);
	time_current = s_current_time.tv_sec * (unsigned int)1000
		+ s_current_time.tv_usec / 1000;
	time_old = old_time.tv_sec * (unsigned int)1000 + old_time.tv_usec / 1000;
	return (time_current - time_old);
}
