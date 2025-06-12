#include "soundSystem.h"
#include <cmath>

// Define static member variables
float SoundSystem::m_exponent = 1.0f;
float SoundSystem::m_averageVolume[400] = { 0.0f };
float SoundSystem::m_volume = 0.5f;

SoundSystem::SoundSystem() {
   // empty 
}

SoundSystem::~SoundSystem(){
    //empty
}

void SoundSystem::ProcessAudio(void *buffer, unsigned int frames){
    float *samples = (float *)buffer;
    float average = 0.0f;

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float *left = &samples[frame * 2 + 0];
        float *right = &samples[frame * 2 + 1];

        // Apply distortion
        *left = powf(fabsf(*left), m_exponent) * ((*left < 0.0f) ? -1.0f : 1.0f);
        *right = powf(fabsf(*right), m_exponent) * ((*right < 0.0f) ? -1.0f : 1.0f);

        // Apply fixed volume scaling
        *left *= m_volume;
        *right *= m_volume;

        average += (fabsf(*left) + fabsf(*right)) / frames;
    }

    for (int i = 0; i < 399; i++) m_averageVolume[i] = m_averageVolume[i + 1];
    m_averageVolume[399] = average;
}