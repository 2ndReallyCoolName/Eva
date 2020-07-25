#include "FFT.h"


void eva::real(std::vector<std::complex<double>> X, std::vector<double>& v, const int end) {
	double real, im;
	for (int j = 0; j < end; j++) {
		real = X[j].real(); im = X[j].imag();
		v[j] = sqrt(pow(real, 2) + pow(im, 2));
	}
}

std::vector<std::complex< double>> eva::Xtransform(std::vector<double> v, const int end) {
	std::vector<std::complex<double>> x(v.size(), 0);

	for (int j = 0; j < end; j++) {
		x[j] = std::complex<double>(v[j], 0);
	}
	return x;
}


void eva::combine(std::vector<std::complex<double>>& v, const int N, const int end) {
	/*print_vect(v);
	std::cout << "---------------------" << std::endl;
*/

	std::complex<double> even, odd;
	int m, pw;
	std::complex<double> w;
	w = pow(e, -(2 * pi / end) * i);
	//std::cout << "w: " << w << std::endl;

	for (int n = 0; n < N; n++) {
		m = (int)pow(2, n);
		pw = (int)(end / (m * 2));

		for (int j = 0; j < end; j += 2 * m) {

#pragma omp parallel
			for (int k = j; k < j + m; k++) {
				//std::cout << k << " " << k + m << std::endl;
				even = v[k]; odd = v[k + m];
				/*std::cout << even << " + " << "w^" << (k - j) * pw << "." << odd << std::endl;
				std::cout << even + odd * std::pow(w, (k - j) * pw) << std::endl;

				std::cout << even << " + " << "w^" << (k + m - j) * pw << "." << odd << std::endl;
				std::cout << even + odd * std::pow(w, (k + m - j) * pw) << std::endl;*/

				v[k] = even + odd * std::pow(w, (k - j) * pw);
				v[k + m] = even + odd * std::pow(w, (k + m - j) * pw);
				eva::print_vect(v);
			}

		}
	}
}


void eva::partition(std::vector<std::vector<double>>&& v) {
	std::vector<std::vector<double>> res(2 * v.size());
	double N = pow(2, (int)log2(v[0].size()));
	int index = 0;
	for (auto s : v) {
		res[index] = std::vector<double>(s.size() / 2);
		res[index + 1] = std::vector<double>(s.size() / 2);
		int j = 0;
		while (j < N) {
			res[index][(int)j / 2] = s[j];
			res[index + 1][(int)j / 2] = s[j + 1];
			j += 2;
		}
		index += 2;
	}
	v = res;
}


std::complex<double> eva::sum(short* buffer, int size, char offset, std::complex<double> w) {
	std::complex<double> total;
#pragma omp parallel reduction(+ : total)
	for (int n = offset; n < size; n += 2) {
		total += (double)buffer[n] * pow(exp(1), w * (double)n);
	}
	return total;
}


unsigned int eva::bit_reverse(const unsigned int v, const unsigned int N) {
	unsigned int mask = 1;
	unsigned int val = 0;
	for (unsigned int j = 0; j < N; j++) {
		if ((mask & v) > 0) {
			val = val | (1 << (N - j - 1));
		}
		mask = mask << 1;
	}

	return val;
}