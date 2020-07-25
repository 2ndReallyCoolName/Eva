#include "wave.h"

Wave::Wave(const char* fname) {
	load(fname);

}

Wave::~Wave() {
	delete[] data;
}

void Wave::load(const char* fname) {
	FILE* fp;
	fopen_s(&fp, fname, "rb");
	if (fp) {
		fread(id, sizeof(char), 4, fp);
		id[4] = '\0';

		if (!strcmp(id, "RIFF")) {
			fread(&chunkSize, sizeof(unsigned int), 1, fp);
			fread(format, sizeof(char), 4, fp);
			format[4] = '\0';

			if (!strcmp(format, "WAVE")) {
				fread(subchunk1ID, sizeof(char), 4, fp);
				fread(&subchunk1Size, sizeof(unsigned int), 1, fp);
				fread(&audioFormat, sizeof(short), 1, fp);
				fread(&numChannels, sizeof(short), 1, fp);
				fread(&sampleRate, sizeof(unsigned int), 1, fp);
				fread(&byteRate, sizeof(unsigned int), 1, fp);
				fread(&blockAlign, sizeof(short), 1, fp);
				fread(&bitsPerSample, sizeof(short), 1, fp);

				fread(subchunk2ID, sizeof(char), 4, fp);
				fread(&subchunk2Size, sizeof(unsigned int), 1, fp);

				count = subchunk2Size / sizeof(short);
				data = new short[subchunk2Size];
				fread(data, sizeof(short), count, fp);
			}
			else {
				std::cout << "Error: RIFF file but not a wave file\n" << std::endl;
			}
		}
		else {
			std::cout << "Error: not a RIFF file\n" << std::endl;

		}
		fclose(fp);
	}
}