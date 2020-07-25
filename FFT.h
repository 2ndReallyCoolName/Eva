#pragma once
#pragma once
#include <complex>
#include <math.h>
#include <omp.h>
#include <future>
#include <vector>
#include <iostream>
#include <bitset>

constexpr int char_bit = 8;
constexpr double pi = 3.14159265359;
constexpr double e = 2.7182818284590452353602874;

constexpr std::complex<double> i(0.0, 1.0);


namespace eva {

	template<typename T>
	std::vector<double> fft(const T* const buffer, int size) {
		std::vector<double> v(size, 0);
		std::copy(buffer, buffer + size, v.begin());

		const int N = (int)log2(size);

		int end = (int)pow(2, N);

		//		std::cout << "partition" << std::endl;
		eva::partition(v, end);

		//eva:: print_vect(v);


		std::complex<double> w = pow(e, -(2 * pi / size) * i);
		std::complex<double> w_in = pow(e, -(2 * pi / (2 * (double)size)) * i);

		//	std::cout << "x transformation" << std::endl;
		std::vector<std::complex< double>> x = eva::Xtransform(v, end);

		//eva::print_vect(v);

		//std::cout << "combine" << std::endl;
		eva::combine(x, N, end);

		//eva::print_vect(x);

		std::cout << "real" << std::endl;
		eva::real(x, v, end);

		//	eva::print_vect(v);


		if (size - end > 0) {
			std::cout << "recursion" << std::endl;
			std::vector<double> v2 = eva::fft(&(buffer[end]), size - end);
			std::copy(&(v2[0]), size - end + &(v2[0]), &(v[end]));
			std::cout << size - end << std::endl;
		}

		return v;
	}

	std::complex<double> sum(short* buffer, int size, char offset, std::complex<double> w);

	void partition(std::vector<std::vector<double>>&& v);

	void combine(std::vector<std::complex<double>>&, const int, const int);

	std::vector<std::complex< double>> Xtransform(std::vector<double>, const int);

	void real(std::vector<std::complex< double>>, std::vector<double>&, const int);

	unsigned int bit_reverse(const unsigned int b, const unsigned int n);


	template<typename t>
	void print_vect(std::vector<t> v) {
		for (auto d : v) {
			std::cout << d << " ";
		}
		std::cout << std::endl << "-------" << std::endl;
	}

	template<typename T>
	void swap(std::vector<T>& v, const int j, const int k) {
		T temp = v[j];
		v[j] = v[k]; v[k] = temp;
	}


	template<typename T>
	int partition_k(std::vector<T>& v, std::vector<T>& f, const int low, const int high) {
		int j = low, k = high;
		int piv_index = (int)((k + j) / 2);
		T pivot = v[piv_index];
		eva::swap(v, piv_index, k);
		eva::swap(f, piv_index, k);
		k--;

		/*std::cout << "start" << std::endl;
		print_vect(v);
		std::cout << std::endl;
*/
		while (j < k) {
			if (v[j] < pivot) {
				eva::swap(v, j, k);
				eva::swap(f, j, k);
				k--;
				continue;
			}/*
			std::cout << "loop" << std::endl;
			std::cout << "low: " << low << " high: " << high << " j: " << j << " k: " << k << " v[j]: " << v[j] << " pivot: " << pivot << std::endl;
			print_vect(v);*/
			std::cout << std::endl;
			j++;
		}

		if (v[j] > v[high]) {
			eva::swap(v, j + 1, high);
			eva::swap(f, j + 1, high);
			piv_index = j + 1;
		}
		else {
			eva::swap(v, j, high);
			eva::swap(f, j, high);
			piv_index = j;
		}

		/*std::cout << "end" << std::endl;
		print_vect(v);
		std::cout << std::endl;
*/
		return piv_index;
	}



	template<typename T>
	void partition(std::vector<T>& v, const int end) {
		const int n = (int)log2(end);
		int jr;
		for (int j = 0; j < end; j++) {
			jr = eva::bit_reverse(j, n);
			if (jr > j) {
				eva::swap(v, j, jr);
			}
		}
	}


	template<typename T>
	std::vector<T> generate_frequency_spectrum(int N, int fs) {
		std::vector<T> v(N, 0);
		for (unsigned int j = 1; j < v.size(); j++) {
			v[j] = (T)j * fs;
		}
		return v;
	}

	template<typename T>
	std::vector<std::vector<T>> k_largest(const std::vector<T> v, T k, int sampling_frequency) {
		std::vector<T> vc = v;
		std::vector<T> fs = eva::generate_frequency_spectrum<T>(v.size(), sampling_frequency);
		//eva::print_vect(vc);

		int high = vc.size() - 1, low = 0, p = 0;

		while (p != k) {
			p = eva::partition_k(vc, fs, low, high);
			if (p > k) {
				high = p;
			}
			if (p < k) {
				low = p;
			}
		}
		eva::print_vect(vc);
		return { vc, fs };
	}



	template<typename T>
	std::vector<T> generate_signal(float* amplitudes, int* frequencies, float time, int sampling_rate, int sz) {
		std::vector<T> v((int)(time * sampling_rate), 0);
		float dt = 1.0f / sampling_rate;
		float t = 0;
#pragma omp parallel
		for (unsigned int k = 0; k < v.size(); k += 1) {
			T val = 0;
			for (int j = 0; j < sz; j++) {
				val += amplitudes[j] * (T)sin(2 * pi * frequencies[j] * t);
			}
			v[k] = val;
			t += dt;
		}
		return v;
	}


	template<typename t>
	void print_vect(std::vector<std::vector<t>> v) {
		for (auto s : v) {
			for (auto d : s) {
				std::cout << d << " ";
			}
			std::cout << std::endl << std::endl;
		}
		std::cout << "-------" << std::endl << std::endl;
	}

}