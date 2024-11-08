#include "aes.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	print_hex(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[] = {
		'0', '1', '2',
		'3', '4', '5',
		'6', '7', '8',
		'9', 'A', 'B',
		'C', 'D', 'E',
		'F'
	};

	if (!buf) return ;

	while (size) {
		putchar(HEX_TAB[*buf >> 0x4 & 0xF]);
		putchar(HEX_TAB[*buf++ & 0xF]);
		putchar(' ');
		size--;
	}

	putchar('\n');
}

static char *read_file(const char *filename, size_t *size)
{
    if (!filename || !size) return (NULL);
    
    char    *ptr = NULL;

    int fd = open(filename, O_RDONLY, S_IRUSR);

    if (fd < 0)
        return (NULL);

    *size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    ptr = (char *)malloc((sizeof(char) * (*size) + 0x10));

    if (!ptr)
        return (NULL);

    if (read(fd, ptr, *size) < 0) {
        free(ptr);
        ptr = NULL;
    }

    close(fd);

    return (ptr);
}

int main(int ac, char **av)
{
	aes_ctx_t ctx;
    char    *data = NULL;
    size_t  size = 0;
    int fd = 0;

    if (ac != 3) {
        fprintf(stderr, "usage %s <img> <output>\n", av[0]);
        return (1);
    }

	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_128;
	ctx.mode = AES_128_ECB;

    memcpy(ctx.key.key_128, "AAAAAAAAAAAAAAAA", 0x10);

	printf("Original AES-128-ECB Key : ");
	print_hex(ctx.key.key_128, sizeof(ctx.key.key_128));

	aes_128_key_expansion(&ctx.key);

    printf("%zd\n", (sizeof(ctx.key.sched_128) / sizeof(aes_round_key_t)) );

	for (size_t i = 0; i < sizeof(ctx.key.sched_128) / sizeof(aes_round_key_t); i++) {
		printf("AES-128-ECB Round Key [%ld] = ", i);
    	print_hex((const uint8_t *)&ctx.key.sched_128[i], sizeof(ctx.key.sched_128[i]));
	}

    data = read_file(av[1], &size);

    if (!data) {
        fprintf(stderr, "Error during reading file !\n");
        return (1);
    }

    aes_128_ecb_enc((byte_t *)data, size, (const byte_t *)data, size, &ctx);

    fd = open(av[2], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    if (fd < 0) return (1);

    if (write(fd, data, size) < 0)
        return (1);

    close(fd);

    aes_128_ecb_dec((byte_t *)data, size, (const byte_t *)data, size, &ctx);

    fd = open("toto.bmp", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    if (fd < 0) return (1);

    if (write(fd, data, size) < 0)
        return (1);

    close(fd);

    free(data);
    data = NULL;

	return (0);
}
