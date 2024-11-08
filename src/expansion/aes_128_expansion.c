/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_128_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:11:52 by stales            #+#    #+#             */
/*   Updated: 2024/11/08 20:58:44 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

#if defined (__AES__) && defined(__SSE2__)
#include "./aes_ni_128_expansion.c"
#endif

/**
* @prototype aes_128_key_expansion
*
* @brief  This function Initialize
* each key for each round from cipher key
* with AES Algorithm expansion for
* AES-128 Bits So 10 + 1 First Round
* Total Rounds = 11 (if you count the first)
*
* @param[in] const aes_key_t*: key AES Cipher Key.
*
* @return aes_status_t    Returns AES_OK if success.
* 						Otherwise AES ERR is Returned.
*/

__attribute__((optimize("O0")))
aes_status_t	aes_128_key_expansion(const aes_key_t *k)
{
	if (!k) return (AES_ERR);

	bool_t aes_ni = FALSE;

	__asm__ volatile ("push rbx\n\t"
			"push rcx\n\t"
			"push rdx\n\t"
			:
			:
			: "memory");

	aes_ni = check_cpu_support_aes();

	__asm__ volatile ("pop rdx\n\t"
			"pop rcx\n\t"
			"pop rbx\n\t"
			:
			:
			: "memory");

	if (aes_ni)
		AES_NI_128_KEY_EXPANSION(k->key_128, (aes_round_key_t *)k->sched_128);

	return (AES_OK);
}
