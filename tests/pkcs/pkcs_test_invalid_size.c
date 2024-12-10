#include "pkcs.h"
#include <stdio.h>

int main(void)
{
	uint8_t buf[0x100];

	puts("SHOULD FAIL !");

	return (pkcs_pad(buf, 0, 0x10) == PKCS_OK);
}

