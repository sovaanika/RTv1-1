/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonia <sleonia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 22:46:14 by sleonia           #+#    #+#             */
/*   Updated: 2019/11/14 19:45:46 by sleonia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
**	Calculate FOV to fullscreen image
*/

t_vector		canvas_to_viewport(int x, int y, t_rt *rt)
{
	t_vector		vector;

	vector.x = (double)x * 1.15 / rt->sdl.sur->w;
	vector.y = (double)y * 0.62 / rt->sdl.sur->h;
	vector.z = 1;
	return (vector);
}

void			intersect_ray(t_rt *rt, t_vector point, t_vector vector, t_figures figure)
{
	t_vector	oc;
	double		k1;
	double		k2;
	double		k3;
	double		discriminant;

	oc.x = point.x - figure.point.x;
	oc.y = point.y - figure.point.y;
	oc.z = point.z - figure.point.z;
	k1 = ft_vec_dot(vector, vector);
	k2 = 2 * ft_vec_dot(oc, vector);
	k3 = ft_vec_dot(oc, oc) - figure.radius * figure.radius;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		rt->calculate.t1 = INFINITY;
		rt->calculate.t2 = INFINITY;
	}
	rt->calculate.t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
	rt->calculate.t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
}

void			init_figure(t_figures *figure)
{
	figure->color.x = 0;
	figure->color.y = 0;
	figure->color.z = 0;
	figure->specular = -1;
	figure->point.x = 0;
	figure->point.y = 0;
	figure->point.z = 0;
	figure->radius = 0;
}


// int				ray_trace(t_vector point, t_vector vector, double min,
// 					double max, t_rt *rt)
// {
// 	double		closest_t;
// 	t_figures	figure;
// 	t_figures	*tmp = rt->figure;

// 	closest_t = INFINITY;
// 	init_figure(&figure);
// 	while (tmp)
// 	{
// 		intersect_ray(rt, point, vector, *tmp);
// 		if (rt->calculate.t1 >= min && rt->calculate.t1 <= max
// 			&& rt->calculate.t1 < closest_t)
// 		{
// 			closest_t = rt->calculate.t1;
// 			figure = *tmp;
// 		}
// 		if (rt->calculate.t2 >= min && rt->calculate.t2 <= max
// 			&& rt->calculate.t2 < closest_t)
// 		{
// 			closest_t = rt->calculate.t2;
// 			figure = *tmp;
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (figure.radius == 0 ? rgb(0, 0, 0) : rgb((int)figure.color.x, (int)figure.color.y, (int)figure.color.z));
// 	// return (figure.radius == 0 ? rgb(255, 255, 255) : figure.color);
// }

int				ray_trace(t_vector point, t_vector vector, double min,
					double max, t_rt *rt)
{
	double		closest_t;
	t_figures	figure;
	t_figures	*tmp = rt->figure;
	t_vector	normal;
	t_vector	color;
	int			color_in_figure;
	t_vector	point2;


	closest_t = INFINITY;
	init_figure(&figure);
	while (tmp)
	{
		intersect_ray(rt, point, vector, *tmp);
		if (rt->calculate.t1 >= min && rt->calculate.t1 <= max
			&& rt->calculate.t1 < closest_t)
		{
			closest_t = rt->calculate.t1;
			figure = *tmp;
		}
		if (rt->calculate.t2 >= min && rt->calculate.t2 <= max
			&& rt->calculate.t2 < closest_t)
		{
			closest_t = rt->calculate.t2;
			figure = *tmp;
		}
		tmp = tmp->next;
	}
	if (figure.radius == 0)
		return (rgb(0, 0, 0));
	// return (figure.radius == 0 ? rgb(0, 0, 0) : rgb((int)figure.color.x, (int)figure.color.y, (int)figure.color.z));
	point2 = ft_vec_multiplication_num(vector, closest_t);
	point2.x = point.x + point2.x;
	point2.y = point.y + point2.y;
	point2.z = point.z + point2.z;
	normal.x = point2.x - figure.point.x;
	normal.y = point2.y - figure.point.y;
	normal.z = point2.z - figure.point.z;
	normal = ft_vec_multiplication_num(normal, (double)(1.0 / ft_vec_length(normal)));
	color_in_figure = rgb((int)figure.color.x, (int)figure.color.y, (int)figure.color.z);


int red = color_in_figure & 0xFF;
int green = (color_in_figure >> 8) & 0xFF;
int blue = (color_in_figure >> 16) & 0xFF;
	color.x = color_in_figure & 0xFF;
	color.y = (color_in_figure >> 8) & 0xFF;
	color.z = (color_in_figure >> 16) & 0xFF;


	// color.x = (0xFF0000 & 0xFF0000) >> 16;
	// color.x = 0xFF0000 & 0xFF0000;
	// color.y = (0xFF0000 & 0xFF0000 ) >> 8;
	color = (ft_vec_multiplication_num(color, compute_light(point, normal, rt->light, figure.specular, ft_vec_multiplication_num(vector, -3))));
	if (color.x > 255)
		color.x = 255;
	if (color.y > 255)
		color.y = 255;
	if (color.z > 255)
		color.z = 255;
	// int rgb_var = rgb(((int)color.x), (((int)color.y) ), ((int)color.z));
	// int rgb = (((int)color.x) | (((int)color.y) ) | ((int)color.z));
	int rgb = (((int)color.x & 0xff) << 16) | (((int)color.y & 0xff) << 8) | ((int)color.z & 0xff);
	return (rgb);
}
