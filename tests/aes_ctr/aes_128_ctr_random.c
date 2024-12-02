#include "pkcs.h"
#include "aes.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "rand.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	aes_ctx_t ctx;
	aes_counter_t cnt;

    uint8_t *in = (uint8_t *)malloc(0x1000);

    if (!in)
        return (1);
    
    uint8_t *out = (uint8_t *)malloc(0x1000);

    if (!out) {
        free(in);
        return (1);
    }

	memset(in, 0, 0x1000);
    memset(out, 0, 0x1000);
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_128;

    if (!urandom(in, 0x1000))
        return (1);
	
    pkcs_status_t status = PKCS_OK;

	if ((status = pkcs_pad(in, 0x1000, 0x10)) != PKCS_OK) {
        free(in);
        free(out);
		fprintf(stderr, "Error: pkcs_pad() = %d\n", status);
		return (1);
	}

	memset((char *)ctx.key.key_128, 'A', 0x10);
	memset(&cnt, 0, sizeof(aes_counter_t));
    memset(cnt.nonce, 0x41, sizeof(cnt.nonce));

	aes_128_key_expansion(&ctx.key);

	aes_ctr_enc(out, 0x1000, &cnt, in, 0x1000, &ctx);

    free(in);
    free(out);

    in = NULL;
    out = NULL;

	return (0);
}

