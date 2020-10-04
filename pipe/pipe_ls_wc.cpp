// 900p
// create two child process
// first child binds stdout to pipe, and exec ls
// second child binds stdin to pipe, and exec wc

#include <sys/wait.h>

extern "C" {
#include <tlpi_hdr.h>
}
#include "pipe.hpp"

void run_ls_child(cp::pipe p) {
	p.close_read();
	if (p.write_fd() != STDOUT_FILENO) {
		if (dup2(p.write_fd(), STDOUT_FILENO) == -1) {
			errExit("dup2 1");
		}
		if (close(p.write_fd()) == -1) {
			errExit("close 2");
		}
	}

	execlp("ls", "ls", nullptr);
}

void run_wc_child(cp::pipe p) {
	p.close_write();
	if (p.read_fd() != STDIN_FILENO) {
		if (dup2(p.read_fd(), STDIN_FILENO) == -1) {
			errExit("dup2 1");
		}
		if (close(p.read_fd()) == -1) {
			errExit("close 2");
		}
	}

	execlp("wc", "wc", "-l", nullptr);
}

int main() {

	auto p = cp::pipe();

	switch (fork()) {
	case -1:
		errExit("fork");
	case 0:
		run_ls_child(p);
		exit(0);
	}

	switch (fork()) {
	case -1:
		errExit("fork");
	case 0:
		run_wc_child(p);
		exit(0);
	}

	p.close_read();
	p.close_write();

	if (wait(nullptr) == -1) {
		errExit("wait 1");
	}

	if (wait(nullptr) == -1) {
		errExit("wait 1");
	}
}
