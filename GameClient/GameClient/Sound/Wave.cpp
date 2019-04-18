#include "stdafx.h"

Wave::Wave(const char* fileName)
{
	memset(&descriptor, 0, sizeof(_WAVEDESCR));
	memset(&format, 0, sizeof(_WAVEFORMAT));
	data = NULL;
	size = 0;

	// Load .WAV file
	FILE* file;
	errno_t err = fopen_s(&file, fileName, "rb");
	if (err == 0)
	{
		// Read descriptor
		fread(&descriptor, sizeof(_WAVEDESCR), 1, file);

		// Check for valid wave file
		if (strncmp((LPCSTR)descriptor.wave, "WAVE", 4) == 0)
		{
			// Read format
			fread(&format, sizeof(_WAVEFORMAT), 1, file);

			// Check for valid .WAV file
			if ((strncmp((LPCSTR)format.id, "fmt", 3) == 0) && (format.format == 1))
			{
				// Read next chunk
				BYTE id[4];
				DWORD _size;
				fread(id, sizeof(BYTE), 4, file);
				fread(&_size, sizeof(DWORD), 1, file);
				DWORD offset = ftell(file);

				// Read data
				while (offset < descriptor.size)
				{
					// Check for .WAV data chunk
					if (strncmp((LPCSTR)id, "data", 4) == 0)
					{
						if (data == NULL)
							data = (LPBYTE)malloc(_size * sizeof(BYTE));
						else
							data = (LPBYTE)realloc(data, (size + _size) * sizeof(BYTE));
						fread(data + size, sizeof(BYTE), _size, file);
						size += _size;
					}
					else
					{
						// Skip chunk
						fseek(file, size, SEEK_CUR);
					}

					// Read next chunk
					fread(id, sizeof(BYTE), 4, file);
					fread(&_size, sizeof(DWORD), 1, file);
					offset = ftell(file);
				}
			}
		}

		fclose(file);
	}
}

Wave::~Wave()
{
	if (data != NULL)
		delete data;
}
