#pragma once
#ifdef __unix__
#include <x86intrin.h>
#endif
#ifdef _WIN32
#include <intrin.h>
#endif
#include <math.h>
#include <stdio.h> //Debug

typedef double Double_t;
typedef int Int_t;
namespace TMath {
	inline Double_t Abs(Double_t x);
	inline Double_t Exp(Double_t x);
	inline Double_t Log(Double_t x);
	Double_t GamSer(Double_t a, Double_t x, Double_t*);
	Double_t GamSer2(Double_t a, Double_t x, Double_t*);
}
Double_t LnGamma(Double_t a) {
	return 1.111;
}
inline Double_t TMath::Exp(Double_t x)
{
	return exp(x);
}
inline Double_t TMath::Log(Double_t x)
{
	return log(x);
}
inline Double_t TMath::Abs(Double_t x)
{
	if (x < 0)return -x;
	return x;
}
Double_t TMath::GamSer(Double_t a, Double_t x, Double_t* p)
{
	Int_t itmax = 100;    // Maximum number of iterations
	Double_t eps = 3.e-14; // Relative accuracy

	if (a <= 0 || x <= 0) return 0;

	Double_t gln = LnGamma(a);
	Double_t ap = a;
	Double_t sum = 1 / a;
	Double_t del = sum;
	for (Int_t n = 1; n <= itmax; n++) {
		ap += 1;
		del = del*x / ap;
		sum += del;
		p[n - 1] = del;
		if (TMath::Abs(del) < Abs(sum*eps)) {
			//printf("Break at %d times, del = %g\n", n, del);
			break;
		}
		//if (n==itmax) std::cout << "*GamSer(a,x)* a too large or itmax too small" << std::endl;
	}
	Double_t v = sum*Exp(-x + a*Log(x) - gln);
	return sum;
}
Double_t TMath::GamSer2(Double_t a, Double_t x, Double_t* p)
{
	Int_t itmax = 100;    // Maximum number of iterations
	Int_t real_it = itmax / 4;
	Double_t eps = 3.e-14; // Relative accuracy
	Double_t sums[4];

	if (a <= 0 || x <= 0) return 0;

	Double_t gln = LnGamma(a);
	Double_t ap = a;
	Double_t sum = 1 / a;
	Double_t del = sum;
	Double_t del_test[4];
	Double_t apf[4] = { a + 1 };
	apf[1] = apf[0] * (a + 2);
	apf[2] = apf[1] * (a + 3);
	apf[3] = apf[2] * (a + 4);
	//for (Int_t n = 1; n <= itmax; n++) {
	//	ap += 1;
	//	del = del / ap;
	//	sum += del;
	//	if (TMath::Abs(del) < Abs(sum*eps)) break;
	//	//if (n==itmax) std::cout << "*GamSer(a,x)* a too large or itmax too small" << std::endl;
	// * x at last
	//}
	// 
	Double_t delArray[4] = { del*x };
	delArray[1] = delArray[0] * x;
	delArray[2] = delArray[1] * x;
	delArray[3] = delArray[2] * x;
	Double_t x4 = x*x*x*x;
	register __m256d d_vec = _mm256_loadu_pd(delArray);
	register __m256d add_vec = _mm256_set_pd(ap + 4, ap + 3, ap + 2, ap + 1);
	register __m256d static_one = _mm256_set_pd(1, 1, 1, 1);
	register __m256d static_x = _mm256_set_pd(x4, x4, x4, x4);
	register __m256d apf_factor = _mm256_loadu_pd(apf);
	register __m256d del_sum = _mm256_setzero_pd();
	for (Int_t i = 0; i < real_it; i++)
	{
		register __m256d toAdd = _mm256_div_pd(d_vec, apf_factor);
		_mm256_storeu_pd(p + 4 * i, toAdd); //Debug
		del_sum = _mm256_add_pd(del_sum, toAdd);
		_mm256_storeu_pd(del_test, toAdd);
		_mm256_storeu_pd(sums, del_sum);
		sum = sums[0] + sums[1] + sums[2] + sums[3] + 1 / a;
		// Test if Breakable
		if (Abs(del_test[2] < Abs(sum*eps)))
		{
			//printf("AVX iterations:%d. del = %g sum*eps = %g\n", i, del_test[2], sum*eps);
			break;
		}
		d_vec = _mm256_mul_pd(d_vec, static_x);
		//add_vec = _mm256_set_pd(ap++, ap++, ap++, ap++);
		add_vec = _mm256_add_pd(add_vec, static_one);
		apf_factor = _mm256_mul_pd(apf_factor, add_vec);
		add_vec = _mm256_add_pd(add_vec, static_one);
		apf_factor = _mm256_mul_pd(apf_factor, add_vec);
		add_vec = _mm256_add_pd(add_vec, static_one);
		apf_factor = _mm256_mul_pd(apf_factor, add_vec);
		add_vec = _mm256_add_pd(add_vec, static_one);
		apf_factor = _mm256_mul_pd(apf_factor, add_vec);
	}
	//sum /= (a + 100);
	Double_t v = sum*Exp(-x + a*Log(x) - gln);
	return sum;
}
