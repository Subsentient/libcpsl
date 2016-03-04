#include <stdio.h>
#include <stdlib.h>
#include "../libcpsl.h"
#include <signal.h>

#define RUNTEST(x) ((CurrentTest = #x), x(), printf("Subtest \"%s\" passed.\n", #x))

static const char *CurrentTest = NULL;

static void SigHandler(const int Signal);

static inline void InitTestUnit(const char *const TestName)
{
	CPSL_Configure(malloc, free, realloc);
	signal(SIGABRT, SigHandler);
	signal(SIGSEGV, SigHandler);
	signal(SIGTERM, SigHandler);
	signal(SIGILL, SigHandler);
	signal(SIGFPE, SigHandler);
	signal(SIGQUIT, SigHandler);
	signal(SIGBUS, SigHandler);
	printf("Running test \"%s\"...\n", TestName);
}

static void SigHandler(const int Signal)
{
	fprintf(stderr, "Received signal number %d while running test \"%s()\"\n", Signal, CurrentTest);
	exit(1);
}
