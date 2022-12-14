/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wati-Init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschlege <tschlege@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:53:58 by tschlege          #+#    #+#             */
/*   Updated: 2022/07/30 18:40:41 by tschlege         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Wati-Includes/Wati_Philosopher.h"

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(data->nb_philo * sizeof(t_philo));
	if (data->philo == NULL)
		return (0);
	while (i < data->nb_philo)
	{
		data->philo[i].data = data;
		data->philo[i].id = i + 1;
		data->philo[i].nb_eat = 0;
		data->philo[i].last_meal = 0;
		pthread_create(&data->philo[i].thread, NULL,
			&think_philo, &data->philo[i]);
		usleep(30);
		i++;
	}
	i = -1;
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].thread, freebox(data));
	return (0);
}

void	init_data_and_forchetta(char *argv[], t_data *data)
{
	int	i;

	i = 0;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->nb_eat_max == 0)
		data->nb_eat_max = ft_atoi(argv[5]);
	data->forks = malloc(data->nb_philo * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return ;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->is_snitching, NULL);
	pthread_mutex_init(&data->last_meal_security, NULL);
	pthread_mutex_init(&data->eat_check, NULL);
	pthread_mutex_init(&data->dead_check, NULL);
	gettimeofday(&data->start_time, NULL);
	if (init_philo(data) == 0)
		return ;
}
