

extern "C" {
#include <curr_time.h>
#include <tlpi_hdr.h>
}

#include "pipe.hpp"

int main(int argc, char** argv) {
	if (argc < 2 || strcmp(argv[1], "--help") == 0) {
		usageErr("%s sleep-time...\n", argv[0]);
	}

	// stdout unbuffered
	setbuf(stdout, NULL);

	printf("%s Parent started\n", currTime("%T"));

	auto pipe = cp::pipe();

	for (auto j = 1; j < argc; j++) {
		switch (fork()) {
		case -1:
			errExit("fork %d", j);
		case 0: // child
			pipe.close_read();
			sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
			printf("%s Child %d (PID=%ld) closing pipe\n", currTime("%T"), j, (long)getpid());
			// write side를 close 한다. 이 pfd에 대한 read는 0이 반환된다
			pipe.close_write();
			_exit(EXIT_SUCCESS);
		default:
			break;
		}
	}
	pipe.close_write();
	int dummy;

	//모든 child가 pfd[1]을 close한 뒤에야 이 read 함수를 빠져나올 수 있다
	if (read(pipe.read_fd(), &dummy, 1) != 0) {
		fatal("parent didn't get EOF");
	}

	printf("%s Parent ready to go\n", currTime("%T"));

	exit(EXIT_SUCCESS);
}
