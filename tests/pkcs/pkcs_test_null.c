#include "pkcs.h"
#include <stdio.h>

int main(void)
{
	puts("SHOULD FAIL !");
	return (pkcs_pad(NULL, 0x100, 0x10) == PKCS_OK);
}
