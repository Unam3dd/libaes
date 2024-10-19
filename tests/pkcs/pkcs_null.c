#include "pkcs.h"

int main(void)
{
	return (pkcs_pad(NULL, 16, 16, 16) == PKCS_ERR_BUF_NULL ? 0 : 1);
}
