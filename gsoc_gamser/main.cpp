#include "gamser.h"
#define TEST_TIME 1000000
#ifdef _WIN32
#define USE_QPC // Use QueryPerformance In Windows
#endif
#include <stdio.h>
#ifdef USE_QPC
#include <windows.h>
#endif
#ifdef __unix__
#define USE_GTOD // Use GetTimeOfDay
#include <sys/time.h>
#endif
#include <stdlib.h>

int main() {
	Double_t p[100], p2[100];
	Double_t d1, d2;
#ifdef USE_QPC
	_LARGE_INTEGER time_start, timeEnd;
	double dqFreq;
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);
#endif
#ifdef USE_GTOD
	struct timeval begin, end;
	gettimeofday(&begin, 0);
#endif
	for (size_t i = 0; i < TEST_TIME; i++)
	{
		d1 = TMath::GamSer(77, 13.0, p);
	}
#ifdef USE_QPC
	QueryPerformanceCounter(&timeEnd);
	printf("Before:%fs\n", (timeEnd.QuadPart - time_start.QuadPart) / dqFreq);
	QueryPerformanceCounter(&time_start);
#endif
#ifdef USE_GTOD
	gettimeofday(&end, 0);
	printf("Before:%fs\n", end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec) / 1000000.0);
	gettimeofday(&begin, 0);
#endif
	for (size_t i = 0; i < TEST_TIME; i++)
	{
		d2 = TMath::GamSer2(77, 13.0, p2);
	}
#ifdef USE_QPC
	QueryPerformanceCounter(&timeEnd);
	printf("After:%fs\n", (timeEnd.QuadPart - time_start.QuadPart) / dqFreq);
#undef USE_QPC
#endif
#ifdef USE_GTOD
	gettimeofday(&end, 0);
	printf("After:%fs\n", end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec) / 1000000.0);
	gettimeofday(&begin, 0);
#undef USE_GTOD
#endif
	//for (size_t i = 0; i < 100; i++)
	//{
	//	if (p[i] == p2[i])continue;
	//	printf("%g %d\n", p[i]- p2[i], (int) p[i] == p2[i]);
	//}
	printf("%g %g\n", d1, d2);
#ifdef _WIN32
	printf("It's running under Windows.\n");
	system("pause");
#endif
#ifdef __unix__
	printf("Running on unix or unix-like system.");
#endif

	return 0;
}
#undef TEST_TIME
