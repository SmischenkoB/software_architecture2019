#pragma once

// -------------------------------------------------------------------------------------
// Class used to play sounds in the game.
// -------------------------------------------------------------------------------------

class Sound
{
public:
    Sound(const char* fileName);
    ~Sound();
    
    void Play(bool loop = false);
    void Stop();
    bool IsPlaying() { return isPlaying; }

private:
    void Open(Wave* _wave);
    void Close();
    void static CALLBACK WaveOut_Proc(HWAVEOUT hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

    bool isPlaying;
    bool isLooping;

    Wave* wave;

    HWAVEOUT m_hWaveout;
    WAVEHDR m_WaveHeader;
};
