#include <stdio.h>
#include <stdlib.h>
#include "../libcpsl.h"
#include <signal.h>

#define RUNTEST(x) ((CurrentTest = #x), x(), printf("Test \"%s\" passed.\n", #x))

static const char *CurrentTest = NULL;

static void SigHandler(const int Signal);

static inline void InitTestUnit(void)
{
	CPSL_Configure(malloc, free, realloc);
	signal(SIGABRT, SigHandler);
	signal(SIGSEGV, SigHandler);
	signal(SIGTERM, SigHandler);
	signal(SIGILL, SigHandler);
	signal(SIGFPE, SigHandler);
	signal(SIGQUIT, SigHandler);
	signal(SIGBUS, SigHandler);
	
}

static void SigHandler(const int Signal)
{
	fprintf(stderr, "Received signal number %d while running test %s\n", Signal, CurrentTest);
	exit(1);
}
