#include "pkcs.h"
#include <stdio.h>

int main(void)
{
	char	buf[0x20];
	pkcs_status_t status = 0;

	puts("SHOULD FAIL !");

	if ((status = pkcs_pad((uint8_t *)buf, sizeof(buf), 0x0)) != PKCS_OK) {
		fprintf(stderr, "pkcs_pad() : %d\n", status);
		return (0);
	}
	

	return (1);
}
