/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_test1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:26:20 by stales            #+#    #+#             */
/*   Updated: 2024/10/20 15:23:57 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"
#include <stdio.h>

int main(void)
{
	uint8_t	value = 0;
	
	for (int i = 0; i <= AES_256_NR; i++) {
		
		if (round_constant(i) != value) {
			printf("RC[%d] = %x != %x\n", i, round_constant(i), value);
			return (1);
		}

		if (!value) {
			value |= 1;
			continue ;
		}

		if (value >= 0x80) {
			value = (value * 2) ^ 0x11b;
			continue ;
		}

		value <<= 1;
	}
	return (0);
}
