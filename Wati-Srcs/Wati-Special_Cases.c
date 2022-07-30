/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Special_Cases.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 19:13:05 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 19:24:46 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

void	snitching_choice(t_philo *philo, int choice)
{
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
}

int	special_arg(t_philo *philo, int choice)
{
	if (choice == 696)
	{
		if (philo->data->time_to_eat > philo->data->time_to_sleep
			&& philo->data->time_to_die > philo->data->time_to_eat
			+ philo->data->time_to_sleep && philo->data->nb_philo != 1)
		{
			if (!(philo->id % 2))
				eat_philo(philo);
			wati_usleep(philo, philo->data->time_to_die);
			is_dead(philo);
			return (0);
		}
	}
	else if (choice == 42)
	{
		if (philo->data->nb_philo == 1)
		{
			pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
			snitching(philo, FORK);
			wati_usleep(philo, philo->data->time_to_die);
			is_dead(philo);
			return (0);
		}
	}
	return (1);
}
