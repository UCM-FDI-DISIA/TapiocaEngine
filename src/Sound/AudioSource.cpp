#include "Sound.h"
#include "SoundEngine.h"

#include "AudioSource.h"


Tapioca::AudioSource::AudioSource(Sound sound, Vector3 pos , bool ispaused, bool islooped) {

	
	paused = ispaused;
     looped = islooped;
     position = pos;
     irrklang ::vec3df p(pos.x, pos.y, pos.z);
     //tienes que crearlo pausado si luego quieres acceder al el
     mysound = Tapioca::SoundEngine::instance()->soundEngine->play3D(sound.mysource, p, looped, true);
     setPaused(ispaused);

}

Tapioca::AudioSource::AudioSource(Sound sound, bool ispaused, bool islooped) { 

    paused = ispaused;
     looped = islooped;
    //tienes que crearlo pausado si luego quieres acceder al el
     mysound = Tapioca::SoundEngine::instance()->soundEngine->play2D(sound.mysource, looped, true);
     setPaused(ispaused);
}

void Tapioca::AudioSource::setPaused(bool p) { 
    mysound->setIsPaused(p);
     paused = p;
}

void Tapioca::AudioSource::setAtenuation(float minDist, float maxDist) {
     mysound->setMinDistance(minDist);
     minDistance = minDist;
     mysound->setMaxDistance(maxDist);
     maxDistance = maxDist;
}

void Tapioca::AudioSource::setPan(float p) { 
    mysound->setPan(p);
     pan = p;
}

void Tapioca::AudioSource::setSpeed(float s) { 
    mysound->setPlaybackSpeed(s);
     playbackSpeed = s;

}

void Tapioca::AudioSource::setLooped(bool l) { 
    mysound->setIsLooped(l);
     looped = l;

}

void Tapioca::AudioSource::setVolume(float v) { 
    mysound->setVolume(v);
     volume = v;

}