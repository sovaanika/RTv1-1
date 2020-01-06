/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonia <sleonia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 23:14:52 by sleonia           #+#    #+#             */
/*   Updated: 2020/01/06 17:38:08 by sleonia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char		*check_argument_name(char *arg)
{
	if (ft_strcmp(arg, SCENE_1_CODE) == 0)
		return (SCENE_1);
	else if (ft_strcmp(arg, SCENE_2_CODE) == 0)
		return (SCENE_2);
	else if (ft_strcmp(arg, SCENE_3_CODE) == 0)
		return (SCENE_3);
	else if (ft_strcmp(arg, SCENE_4_CODE) == 0)
		return (SCENE_4);
	else
		return (NULL);
}

int			find_camera(int i, char **file_split, t_vector *camera)
{
	if (!file_split[0])
		ft_exit(ERROR_INPUT);
	if (ft_strcmp("-", file_split[++i]) == 0)
	{
		if (ft_strcmp(CAMERA, file_split[++i]) == 0)
		{
			if (ft_strstr(file_split[++i], CAMERA_CENTER))
				*camera = get_array_value(file_split[i]);
			else
				ft_exit(ERROR_INPUT);
		}
		else
			ft_exit(ERROR_INPUT);
	}
	else
		ft_exit(ERROR_INPUT);
	return (i + 2);
}

void		validation(char *arg, t_rt *rt)
{
	int		i;
	char	*file;
	char	*filename;
	char	**file_split;

	i = -1;
	if (!(filename = check_argument_name(arg)))
	{
		show_example_input();
		ft_exit(ERROR_INPUT);
	}
	if (!(file = read_big_file(file, filename)))
		ft_exit(ERROR_READ_BIG_FILE);
	if (!(file_split = ft_strsplit(file, '\n')))
		ft_exit(ERROR_STRSPLIT);
	i = (find_camera(i, file_split, &(rt->camera)));
	i = (lights_processing(i, file_split, &rt->light));
	i = (objects_processing(i, file_split, &rt->obj));
	free(file);
	ft_destroy_string_arr(file_split);
}
