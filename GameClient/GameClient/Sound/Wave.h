#pragma once

// -------------------------------------------------------------------------------------
// Backend class used to by the Sound class to play sounds.
// Not meant to be used directly. Use Sound class instead.
// -------------------------------------------------------------------------------------

class Wave
{
public:
	Wave(const char* fileName);
	~Wave();

	DWORD GetSize() { return size; }
	SHORT GetChannels() { return format.channels; }
	DWORD GetSampleRate() { return format.sampleRate; }
	SHORT GetBitsPerSample() { return format.bitsPerSample; }
	LPBYTE GetData() { return data; }

private:

#pragma pack(1)
	typedef struct __WAVEDESCR
	{
		BYTE riff[4];
		DWORD size;
		BYTE wave[4];

	} _WAVEDESCR, *_LPWAVEDESCR;

	typedef struct __WAVEFORMAT
	{
		BYTE id[4];
		DWORD size;
		SHORT format;
		SHORT channels;
		DWORD sampleRate;
		DWORD byteRate;
		SHORT blockAlign;
		SHORT bitsPerSample;

	} _WAVEFORMAT, *_LPWAVEFORMAT;
#pragma pack()

	_WAVEDESCR descriptor;
	_WAVEFORMAT format;
	LPBYTE data;
	DWORD size;
};
