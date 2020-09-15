#pragma once

extern "C" {
#include <tlpi_hdr.h>
}

namespace cp {
class pipe {
public:
	pipe() {
		if (::pipe(pfd) == -1) {
			errExit("pipe");
		}
	}

	int read_fd() {
		return pfd[0];
	}
	int write_fd() {
		return pfd[1];
	}
	void close_read() {
		if (close(pfd[0]) == -1) {
			errExit("close");
		}
	}
	void close_write() {
		if (close(pfd[1]) == -1) {
			errExit("close");
		}
	}

private:
	int pfd[2];
};
} // namespace cp
