
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define STREQ(str1, str2) (strcmp(str1, str2) == 0)

void usage() {
	printf("\n");
	printf("SysV IPC shared memory print on the standard output. \n");
	printf("\n");
	printf("Usage: \n\t shmcat <shmid> [length] \n");
	printf("\n");
}


int main(int argc, char **argv) {
	
	if (argc < 2) {
		printf("ERROR: invalid number of arguments\n");
		usage();
		return EXIT_FAILURE;
	}
	
	if(STREQ(argv[1], "-h") || STREQ(argv[1], "--help")) {
		usage();
		return EXIT_SUCCESS;
	}
		
	int shmid = atoi(argv[1]);
	if(shmid < 0) {
		printf("ERROR: shared memory id out of range: '%lu'\n", shmid);
		return EXIT_FAILURE;
	}
	
	struct shmid_ds ds;
	memset(&ds, 0x00, sizeof(ds));
	int err = shmctl(shmid, IPC_STAT, &ds);
	if (err < 0) {
		printf("ERROR: shared memory control failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
		
	size_t shm_bytes = ds.shm_segsz;
	if (argc >= 3) {
		shm_bytes = atoi(argv[2]);
	}
	
	if (shm_bytes <= 0) {
		printf("ERROR: invalid number of bytes\n");
		return -1;
	}
	
	void *s = shmat(shmid, NULL, SHM_RDONLY);
	if (s == (void *)-1) {
		printf("ERROR: shared memory attach failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	printf("\nshmid: %i \t bytes: %zu\n\n", shmid, shm_bytes);
	
	unsigned char sbuf[32];
	memset(sbuf, 0x00, sizeof sbuf);
	size_t si = 0;
	printf("\n%8.8zX  ", si);
	
	for (si = 0; si < shm_bytes; si++) {
		size_t sis = si % 16;
		if (si && si % 16 == 0) {
			printf(" ");
			for (size_t i = 0; i < 16; ++i) {
				printf("%c", isprint(sbuf[i]) ? sbuf[i] : '.');
			}
			memset(sbuf, 0, sizeof sbuf);
			printf("\n%8.8zX  ", si);
		}
		sbuf[sis] = ((unsigned char *)s)[si];
		printf("%2.2X ", sbuf[sis]);
	}
	
	if (si) {
		size_t sis = si % 16;
		sis = sis ? sis : 16;
		for (size_t i = sis; i < 16; ++i)
			printf("   ");
		printf(" ");
		for (size_t i = 0; i < sis; ++i) {
			printf("%c", isprint(sbuf[i]) ? sbuf[i] : '.');
		}
		memset(sbuf, 0, sizeof sbuf);
		printf("\n");
	}
	printf("\n");

	err = shmdt(s);
	if (err < 0) {
		printf("ERROR: shared memory detach failed: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}
