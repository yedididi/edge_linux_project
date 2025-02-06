#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
	int fd, ret;
	char *dev_name;
	struct input_event ev;	

	if(argc != 2) {
		printf("touchtest: there is no input device\n");
		return EXIT_FAILURE;
	}

	dev_name = argv[1];

	fd = open(dev_name, O_RDONLY);
	if(fd == -1) {
		printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
		return EXIT_FAILURE;
	}
	printf("touchtest: %s opened\n", dev_name);
	sleep(1); // to avoid mixing messages

	for(;;) {
		ret = read(fd, &ev, sizeof(struct input_event));
		if(ret == -1) {
			printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
			return EXIT_FAILURE;
		}
		//if(ev.type == 3 && ev.code == 1) {
			printf("touchtest: ev=%d, code=%d, value=%d\n", ev.type, ev.code, ev.value);
		//}
	}

	close(fd);
	printf("touchtest: %s closed\n", dev_name);

	return EXIT_SUCCESS;
}
