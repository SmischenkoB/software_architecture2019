#include "stdafx.h"

#pragma comment(lib, "winmm.lib")

Sound::Sound(const char* fileName)
{
    wave = new Wave(fileName);

    m_hWaveout = NULL;
    memset(&m_WaveHeader, 0, sizeof(WAVEHDR));

    isPlaying = false;
    isLooping = false;

    Open(wave);
}

Sound::~Sound(void)
{
    Stop();
    Close();

    delete wave;
}

void Sound::Play(bool _loop)
{
    if (!isPlaying && m_hWaveout)
    {
        m_WaveHeader.lpData = (LPSTR)wave->GetData();
        m_WaveHeader.dwBufferLength = wave->GetSize();
        if (waveOutPrepareHeader(m_hWaveout, &m_WaveHeader, sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
        {
            if (waveOutWrite(m_hWaveout, &m_WaveHeader, sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
            {
                isPlaying = TRUE;
                isLooping = _loop;
            }
        }
    }
}

void Sound::Stop()
{
    if(isPlaying && m_hWaveout)
    {
        isPlaying = FALSE;
        isLooping = FALSE;

        if (waveOutReset(m_hWaveout) == MMSYSERR_NOERROR)
        {
            waveOutUnprepareHeader(m_hWaveout, &m_WaveHeader, sizeof(WAVEHDR));
        }
    }
}

void Sound::Open(Wave* _wave)
{
    if (_wave->GetSize() == 0)
        return;

    SHORT format = WAVE_FORMAT_PCM;
    SHORT blockAlign = _wave->GetChannels() << 1;
    WAVEFORMATEX wfex;
    wfex.wFormatTag = format;
    wfex.nChannels = _wave->GetChannels();
    wfex.nSamplesPerSec = _wave->GetSampleRate();
    wfex.nAvgBytesPerSec = blockAlign * _wave->GetSampleRate();
    wfex.nBlockAlign = blockAlign;
    wfex.wBitsPerSample = _wave->GetBitsPerSample();
    wfex.cbSize = 0;
    if (waveOutOpen(&m_hWaveout, WAVE_MAPPER, &wfex, (DWORD_PTR)&Sound::WaveOut_Proc, (DWORD_PTR)this, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
    {
        m_hWaveout = NULL;
    }
}

void Sound::Close()
{
    if (m_hWaveout)
    {
        waveOutClose(m_hWaveout);
        m_hWaveout = NULL;
    }
}

void Sound::WaveOut_Proc(HWAVEOUT hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    if (uMsg == WOM_DONE)
    { 
        Sound* sound = (Sound*)((DWORD_PTR)dwInstance);

        if (sound->isPlaying)
        {
            if (sound->isLooping)
            {
                // play again
                waveOutWrite(sound->m_hWaveout, &sound->m_WaveHeader, sizeof(WAVEHDR));
            }
            else
            {
                // stop
                waveOutUnprepareHeader(sound->m_hWaveout, &sound->m_WaveHeader, sizeof(WAVEHDR));
                sound->isPlaying = FALSE;
            }
        }
    }
}
