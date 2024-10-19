#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pkcs.h"

int main(void)
{
	uint8_t	buf[0x40] = "passwordpassword";
	uint8_t	cmp[0x40];

	size_t	sz = strlen((char *)buf);

	memset(cmp, 0, sizeof(cmp));
	
	uint8_t	pad = PKCS_PAD_LEN(16, sz);
	
	memset(cmp, pad, pad);

	if (pkcs_pad(buf, sizeof(buf), sz, 16) != PKCS_OK)
		return (1);

	return (memcmp((char *)buf+sz, cmp, pad));
}
