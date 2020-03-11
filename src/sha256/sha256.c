/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 17:57:50 by awoimbee          #+#    #+#             */
/*   Updated: 2020/03/11 14:48:46 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sha256.h"
#include <stdint.h>
#include <libft/ft_mem.h>
#include <libft/ft_prtf.h>
#include <stdlib.h>
#include <libft/ft_exit.h>

/* first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311 */
static const uint32_t	constants[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

static uint32_t rotright(uint32_t x, uint32_t shift)
{
	return ((x >> shift) | (x << (32 - shift)));
}

static void	set_hashing(t_global *g)
{
	if (g->hashing)
		return;
	g->hashing = true;
	g->hashes[0] = 0x6a09e667;
	g->hashes[1] = 0xbb67ae85;
	g->hashes[2] = 0x3c6ef372;
	g->hashes[3] = 0xa54ff53a;
	g->hashes[4] = 0x510e527f;
	g->hashes[5] = 0x9b05688c;
	g->hashes[6] = 0x1f83d9ab;
	g->hashes[7] = 0x5be0cd19;
}

void		sha256_unset_hashing(t_global *g)
{
	ft_bzero(g->hashes, sizeof(g->hashes));
	g->hashing = false;
}

#include <libft/ft_nb.h>

void		set_hashes_big_endian(t_global *g)
{
	size_t		i;

	i = -1;
	while (++i < sizeof(g->hashes) / sizeof(g->hashes[0]))
	{
		g->hashes[i] = ft_swap_endian32(g->hashes[i]);
	}
}

char		*sha256_get_digest(t_global *g)
{
	const char	*str_base;
	char		*digest[2];
	uint8_t		*sha256;

	str_base = "0123456789abcdef";
	ft_assertp_safe(digest[0] = malloc(65), "malloc failed", NULL);
	ft_memset(digest[0], '0', 64);
	digest[0][64] = '\0';
	digest[1] = digest[0];
	set_hashes_big_endian(g);
	sha256 = (uint8_t*)g->hashes;
	while (*digest[1])
	{
		int v = *(sha256++);
		digest[1][1] = str_base[v % 16];
		v /= 16;
		digest[1][0] = str_base[v % 16];
		digest[1] += 2;
	}
	sha256_unset_hashing(g);
	return digest[0];
}

void		sha256_chunk(t_global *g, void *chunk)
{
	uint32_t	msa[64];
	uint32_t	h[8];
	int			i;

	ft_bzero(msa, sizeof(msa));
	set_hashing(g);
	ft_memcpy(h, g->hashes, sizeof(h));

	// ft_memcpy(msa, chunk, 64);

	for (int ii = 0; ii < 16; ++ii) {
		msa[ii] = ft_swap_endian32(((uint32_t*)chunk)[ii]);
	}
	i = 15;
	while (++i < 64)
	{
		uint32_t s0 = rotright(msa[i - 15], 7) ^ rotright(msa[i-15], 18) ^ (msa[i-15] >> 3);
		uint32_t s1 = rotright(msa[i - 2], 17) ^ rotright(msa[i-2], 19) ^ (msa[i-2] >> 10);
		msa[i] = s1 + msa[i-7] + s0 + msa[i-16];
	}

	i = -1;
	while (++i < 64)
	{
		uint32_t EP1 = rotright(h[4], 6) ^ rotright(h[4], 11) ^ rotright(h[4], 25);
		uint32_t ch = (h[4] & h[5]) ^ ((~h[4]) & h[6]);
		uint32_t temp1 = h[7] + EP1 + ch + constants[i] + msa[i];
		uint32_t EP0 = rotright(h[0], 2) ^ rotright(h[0], 13) ^ rotright(h[0], 22);
		uint32_t maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
		uint32_t temp2 = EP0 + maj;

		ft_memmove(&h[1], &h[0], sizeof(h) - sizeof(h[0]));
		// ft_memcpy(&h[1], &h[0], sizeof(h) - sizeof(h[0]));
		h[4] += temp1;
		h[0] = temp1 + temp2;
	}
	g->hashes[0] += h[0];
	g->hashes[1] += h[1];
	g->hashes[2] += h[2];
	g->hashes[3] += h[3];
	g->hashes[4] += h[4];
	g->hashes[5] += h[5];
	g->hashes[6] += h[6];
	g->hashes[7] += h[7];
}
