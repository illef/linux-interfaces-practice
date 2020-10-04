#include <fcntl.h>
#include <iostream>
#include <unistd.h>

extern "C" {
#include <tlpi_hdr.h>
}

//"stdout을 args[1]으로 보낸다"
int main(int argc, char** argv) {
	if (argc < 2) {
		usageErr("%s <file>\n", argv[0]);
	}

	int fd = ::open(argv[1], O_RDWR | O_CREAT);
	if (fd == -1) {
		errExit("open file %s", argv[1]);
	}

	if (dup2(fd, STDOUT_FILENO) == -1) {
		errExit("dup2");
	}

	std::cout << "test" << std::endl;

	if (close(fd) == -1) {
		errExit("close");
	}

	return 0;
}
