/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 18:03:59 by awoimbee          #+#    #+#             */
/*   Updated: 2020/03/05 18:06:55 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <libft.h>

typedef int(t_subcmd)(char *[]);

struct s_cmds
{
	char		*name;
	t_subcmd	*fn_ptr;
};

t_subcmd	*get_subcmd(char *requested_cmd);
int main_md5(char *s[]);
int main_sha256(char *s[]);


#endif