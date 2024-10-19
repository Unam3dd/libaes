#include "pkcs.h"
#include <string.h>

int main(void)
{
	uint8_t	buf[0x20] = "pass";

	return (pkcs_pad(buf, sizeof(buf), strlen((char *)buf), 0) == PKCS_ERR_INVALID_BLK_SIZE ? 0 : 1);
}

