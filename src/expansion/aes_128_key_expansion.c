/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_128_key_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 07:45:43 by stales            #+#    #+#             */
/*   Updated: 2024/10/22 08:12:01 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

/**
* @prototype aes_128_key_expansion
*
* @brief  Just simple brief 
*
*	There is simple description 
*
* @param[in] int: a	   simple integer.
* @param[in] int: b       simple integer.
* @return int             Returns sum of a + b.
*/

key_exp_status_t	aes_128_key_expansion(const aes_key_t *k)
{
	if (!k) return (AES_KEY_EXP_ERR);

	return (AES_KEY_EXP_OK);
}
