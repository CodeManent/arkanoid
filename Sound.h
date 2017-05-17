#ifndef Sound_H
#define Sound_H

#include <string>
#include <iostream>

#include <al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


class Sound
{
public:
	Sound(const std::string &path);
	~Sound();							// releases the handles

	virtual void open(const std::string &path);
	virtual bool play(bool loop = false);// play the ogg stream
	virtual void stop();				// stops the playing sound
	virtual bool playing();				// check if source is playing
	virtual bool update();				// update the stream is necessary
	virtual std::string toString() const;

private:
	bool resetStream();					// set the sound at the beginning
	bool stream(ALuint buffer);			// reloads the buffer
	void clear();						// clears the queue
	void alCheck();						// checks OpenAL error state
	std::string vorbisErorrString(int errorCode);// stringify an error code

	FILE          *oggFile;				// file handle
	OggVorbis_File oggStream;			// stream handle
	vorbis_info   *vorbisInfo;			// some informatino data
	vorbis_comment *vorbisComment;		// user comments


	ALuint buffers[2];					// front abd back buffers
	ALuint source;						// audio source
	ALenum format;						// internal format
	bool loop;							// looping sound
	static const size_t bufferSize;			// buffer size used
};


#endif
