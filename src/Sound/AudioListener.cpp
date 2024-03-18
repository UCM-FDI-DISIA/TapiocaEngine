#include "AudioListener.h"

Tapioca::AudioListener::AudioListener(Vector3 pos, Vector3 ldir, Vector3 vel, Vector3 upvec) { 
	position = pos;
    look = ldir;
    velocity = vel;
    up = upvec;
}
