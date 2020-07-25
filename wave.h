#pragma once
#include <fstream>
#include <iostream>

class Wave {
	char id[5];
	unsigned int chunkSize;
	char format[5];
	//fmt subchunk
	char subchunk1ID[5];
	unsigned int subchunk1Size;
	short audioFormat;
	short numChannels;
	unsigned int sampleRate;
	unsigned int byteRate;
	short blockAlign;
	short bitsPerSample;
	//data subchunk
	char subchunk2ID[5];
	unsigned int subchunk2Size;
	short* data;
	unsigned int count;

public:
	Wave(const char* fname);
	~Wave();
	void load(const char* fname);

	void print_header() {
		std::cout << "id: " << id << std::endl;
		std::cout << "chunkSize: " << chunkSize << std::endl;
		std::cout << "format: " << format << std::endl;
		std::cout << "subchunk1Id: " << subchunk1ID << std::endl;
		std::cout << "subchunk1Size: " << subchunk1Size << std::endl;
		std::cout << "audio format: " << audioFormat << std::endl;
		std::cout << "channels: " << numChannels << std::endl;
		std::cout << "sample rate: " << sampleRate << std::endl;
		std::cout << "byte rate: " << byteRate << std::endl;
		std::cout << "block align: " << blockAlign << std::endl;
		std::cout << "bits per sample: " << bitsPerSample << std::endl;
		std::cout << "subchunk2ID: " << subchunk2ID << std::endl;
		std::cout << "subchunk2Size: " << subchunk2Size << std::endl;


		for (int i = 0; i < 200; i++) {
			std::cout << data[i] << ", ";
		}

	}

	inline unsigned int get_size() { return count; }
	inline short* get_data() { return data; }

};