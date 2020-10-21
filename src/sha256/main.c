/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:20:20 by awoimbee          #+#    #+#             */
/*   Updated: 2020/10/21 12:34:58 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "args.h"
#include "ft_sha256.h"
#include <libft/ft_mem.h>



int		main_sha256(int ac, char *argv[])
{
	t_digest_args	a;
	t_global	globals;


	ft_bzero(&globals, sizeof(globals));
	a = parse_digest_args(&argv[2]);
	if (!a.input)
		sha256_stdin();
	else if (!a.str)
		sha256_fd();
	else
		sha256_str();
	return (0);
}
