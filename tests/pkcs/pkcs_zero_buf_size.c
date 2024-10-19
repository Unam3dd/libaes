#include "pkcs.h"

int main(void)
{
	uint8_t	buf[0x10];

	return (pkcs_pad(buf, 0, 16, 16) == PKCS_ERR_BUFF_SIZE_ZERO ? 0 : 1);
}
