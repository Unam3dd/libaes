#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pkcs.h"

int main(void)
{
	uint8_t	buf[] = "passwordpassword";
	uint8_t	cmp[0x40];
	size_t	sz = 0;
	uint8_t	pad = 0;

	sz = strlen((char *)buf);

	memset(cmp, 0, sizeof(cmp));
	
	pad = PKCS_PAD_LEN(16, sz);
	
	memset(cmp, pad, pad*sizeof(uint8_t));

	pkcs_status_t status = pkcs_pad(buf, sizeof(buf), sz, 16);

	return (status == PKCS_ERR_BUF_TOO_SMALL ? 0 : 1);
}
