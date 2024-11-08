#include "aes.h"
#include "rand.h"

#include <immintrin.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	print_hex_bytes(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[0x10] = {
		'0', '1', '2',
		'3', '4', '5',
		'6', '7', '8',
		'9', 'A', 'B',
		'C', 'D', 'E',
		'F'
	};

	if (!buf) return ;

	while (size > 0) {
        
        fflush(stdout);
       
        if (write(1, (const void *)&HEX_TAB[(*buf >> 0x4) & 0xF], 1) != 1)
            return ;

        if (write(1, (const void *)&HEX_TAB[*buf++ & 0xF], 1) != 1)
            return ;

        if (write(1, " ", 1) != 1)
            return ;
        
		size--;

        fflush(stdout);
	}
    
    fflush(stdout);

    if (write(1, "\n", 1) != 1)
        return ;
}

int main(void)
{
	aes_key_t	keys;
	
    memset(&keys, 0, sizeof(keys));
	
    urandom(keys.key_128, sizeof(keys.key_128));

    printf("Number of keys : %zd\n", sizeof(keys.sched_128) / sizeof(aes_round_key_t));
	
    aes_128_key_expansion(&keys);
    
    puts("Original Keys :\n");

    for (size_t i = 0; i < sizeof(keys.sched_128) / sizeof(aes_round_key_t); i++) {
        print_hex_bytes((const uint8_t *)(&keys.sched_128[i]), sizeof(keys.sched_128[i]));
    }

	return (0);
}
