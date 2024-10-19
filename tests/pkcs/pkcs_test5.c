#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pkcs.h"

int main(void)
{
	uint8_t	buf[0x20];
	uint8_t	cmp[0x40];
	size_t	sz = 0;
	uint8_t	pad = 0;


	memset(buf, 0, sizeof(buf));
	memset(cmp, 0, sizeof(cmp));
	
	sz = strlen((char *)buf);
	
	pad = PKCS_PAD_LEN(16, sz);
	
	memset(cmp, pad, pad);

	pkcs_status_t status = pkcs_pad(buf, sizeof(buf), sz, 16);

	if (status)
		return (1);

	if (memcmp(buf, cmp, pad))
		return (1);

	status = pkcs_unpad(buf, sizeof(buf), strlen((char *)buf), 16);

	if (status)
		return (1);

	memset(cmp, 0, sizeof(cmp));

	if (memcmp(buf, cmp, sizeof(buf)))
		return (1);

	return (0);
}
