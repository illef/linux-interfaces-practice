#include <stdlib.h>
#include <sys/wait.h>
extern "C" {
#include <tlpi_hdr.h>
}

constexpr size_t BUF_SIZE = 10;

void child_side(int pfd[2]) {
	char buf[BUF_SIZE];
	// close write end
	if (close(pfd[1]) == -1) {
		errExit("close - child");
	}

	for (;;) {
		auto numRead = read(pfd[0], buf, BUF_SIZE);
		if (numRead == -1) {
			errExit("read");
		}
		if (numRead == 0) {
			break;
		}
		if (write(STDOUT_FILENO, buf, numRead) != numRead) {
			fatal("child - partail/faild write");
		}
	}

	write(STDERR_FILENO, "\n", 1);
	// close read End
	if (close(pfd[0]) == -1) {
		errExit("close");
	}
	_exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	int pfd[2];

	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		usageErr("%s string\n", argv[0]);
	}

	if (pipe(pfd) == -1) {
		errExit("pipe");
	}

	switch (fork()) {
	case -1:
		errExit("fork");
	case 0: // child
		child_side(pfd);
		_exit(EXIT_SUCCESS);
	default:
		// close read end
		if (close(pfd[0]) == -1)
			errExit("close - parent");
		if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
			fatal("parent - partial/failed write");
		}
		if (close(pfd[1]) == -1) {
			errExit("close");
		}
	}

	wait(NULL);
	exit(EXIT_SUCCESS);
}
