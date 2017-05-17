//TODO: error checking for everything
//http://devmaster.net/posts/openal-lesson-8-oggvorbis-streaming-using-the-source-queue

#include "Sound.h"
//#include "Console.h"

#include <sstream>
#include <iostream>

#include <stdexcept>

const size_t Sound::bufferSize = 4096*8;

Sound::Sound(const std::string &path)
:oggFile(NULL)
,vorbisInfo(NULL)
,vorbisComment(NULL)
,loop(false)
{
	open(path);
}

Sound::~Sound()
{
	//equivalent of release
	alSourceStop(source);
	alDeleteSources(1, &source);
	alDeleteBuffers(2, buffers);
	alCheck();

	ov_clear(&oggStream);
}


/*
	Info: όταν το gain είναι στην default τιμή (1.0f???) δημιουργούνται παράσιτα στον ήχο!
	      Με ορισμένο στο 0.5f, φαίνεται να δουλεύει καλά.
*/
void Sound::open(const std::string &path)
{
	int result;

	if( !( oggFile = fopen(path.c_str(), "rb") ) )
	{
		throw std::runtime_error( std::string("Couldn't open sound file: ") + path );
	}

	//if( ( result = ov_open(oggFile, &oggStream, NULL, 0) ) < 0 )
	if( ( result = ov_open_callbacks(oggFile, &oggStream, NULL, 0, OV_CALLBACKS_DEFAULT) ) < 0 )
	{
		fclose(oggFile);
		throw std::runtime_error( std::string("Couldn't open ogg stream: ") + vorbisErorrString(result) );
	}

	vorbisInfo = ov_info(&oggStream, -1);
	vorbisComment = ov_comment(&oggStream, -1);

	if(vorbisInfo->channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else if(vorbisInfo->channels == 2)
	{
		format = AL_FORMAT_STEREO16;
	}
	else
	{
		throw std::runtime_error( std::string("Bad number of chamnnels on the sound stream for the file ") + path );
	}


	alGenBuffers(2, buffers);
	alCheck();

	alGenSources(1, &source);
	alCheck();

	alSourcef ( source, AL_GAIN, 0.5 );
	alSource3f( source, AL_POSITION, 0.0f, 0.0f, 0.0f );
	alSource3f( source, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alSource3f( source, AL_DIRECTION, 0.0f, 0.0f, 0.0f );
	alSourcef ( source, AL_ROLLOFF_FACTOR, 0.0f );
	alSourcei ( source, AL_SOURCE_RELATIVE, AL_TRUE );//true because streaming and changing buffers

	alCheck();
}


bool Sound::play(bool loop)
{
	this->loop = loop;

	resetStream();

	if( playing() )
	{
		stop();
		//return true;
	}

	clear();

	if( !stream(buffers[0]) )
	{
		return false;
	}

	if( !stream(buffers[1]) )
	{
		return false;
	}

	alSourceQueueBuffers(source, 2, buffers);
	alSourcePlay(source);

	alCheck();

	return true;
}




void Sound::stop()
{
	if( !playing() )
	{
		//the sound is not playing
		return;
	}

	alSourceStop(source);
}





bool Sound::playing()
{
	ALenum state;

	alGetSourcei(source, AL_SOURCE_STATE, &state);
	alCheck();

	return state == AL_PLAYING;
}







bool Sound::update()
{
	ALint processed;
	bool active = true;

	alGetSourcei( source, AL_BUFFERS_PROCESSED, &processed );
	alCheck();

	while(processed--)
	{
		ALuint buffer;

		//pop a buffer from the front
		alSourceUnqueueBuffers( source, 1, &buffer );
		alCheck();

		active = stream(buffer);
		if(!active)
			return false;

		//push a buffer to the back
		alSourceQueueBuffers(source, 1, &buffer );
		alCheck();
	}

	return true;
}





std::string Sound::toString() const
{
	std::stringstream ss;

	ss
		<< "version " << vorbisInfo->version << '\n'
		<< "channels " << vorbisInfo->channels << '\n'
		<< "rate (hz) " << vorbisInfo->rate << '\n'
		<< "bitrate upper " << vorbisInfo->bitrate_upper << '\n'
		<< "bitrate nominal " << vorbisInfo->bitrate_nominal << '\n'
		<< "bitrate lower " << vorbisInfo->bitrate_lower << '\n'
		<< "bitrate window " << vorbisInfo->bitrate_window << '\n'
		<< '\n'
		<< "vendor " << vorbisComment->vendor << '\n';

	for(int i = 0; i < vorbisComment->comments; ++i)
	{
		ss << "   " << vorbisComment->user_comments[i] << '\n';
	}

	ss << std::endl;

	return ss.str();
}



bool Sound::resetStream()
{
	int ret = ov_pcm_seek(&oggStream, 0);
	if(ret != 0)
	{
		std::string errMsg("Sound::resetStream: ");
		errMsg += vorbisErorrString(ret);
		throw std::runtime_error( errMsg );
	}
	return true;
}



/* returns true if the stream finished */
bool Sound::stream(ALuint buffer)
{
	char data[bufferSize];
	int size = 0;
	int section;
	int result;

	while(size < int(bufferSize))
	{
		result = ov_read(&oggStream, data+size, int(bufferSize) - size, 0, 2, 1, &section);

		if(result  > 0)
		{
			size += result;
		}
		else if(result < 0)
		{
			throw std::runtime_error("Couldn't read from the ogg stream");
		}
		else // result == 0
		{
			break;
		}
	}

//	std::cout << size << std::endl;


	if(size == 0)
	{
		//end of stream reached
		if(loop)
		{
			resetStream();
			return stream(buffer);
		}
		else
		{
			return false;
		}
	}

	alBufferData(buffer, format, data, size, vorbisInfo->rate);
	alCheck();

	return true;
}

void Sound::clear()
{
	int queued;

	alGetSourcei( source, AL_BUFFERS_QUEUED, &queued );
	alCheck();

	while(queued--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers( source, 1, &buffer );
		alCheck();
	}
}

void Sound::alCheck()
{
	ALenum err;
	bool errorFound = false;
	std::stringstream ss;
	std::string errMsg("Sound::alCheck: ");

	while( ( err = alGetError() ) != AL_NO_ERROR )
	{
		switch(err)
		{
		case AL_INVALID_NAME:
			errMsg += "A bad ID was passed to an OpenAL function";
			break;

		case AL_INVALID_ENUM:
			errMsg += "An invalid enum value was passed to an OpenAL function";
			break;

		case AL_INVALID_VALUE:
			errMsg += "An invalid value was passed to an OpenAL function";
			break;

		case AL_INVALID_OPERATION:
			errMsg += "The rewuested operation is not valid";
			break;

		case AL_OUT_OF_MEMORY:
			errMsg += "The requested operation resulted in OpenAL out of memory";
			break;

		default:
			errMsg += "Unspecified OpenAL error";
		}
		errorFound = true;
	}



	if(errorFound)
	{
		errMsg += ss.get();
		throw std::runtime_error( errMsg );
	}
}


std::string Sound::vorbisErorrString(int errorCode)
{
	switch(errorCode)
	{
	case OV_EREAD:		/*-128*/	return "Reading from media.";
	case OV_EFAULT:		/*-129*/	return "Internal logic fault (bug or heap/stack corruption.";
	case OV_EIMPL:		/*-130*/	return "OV_EIMPL";
	case OV_EINVAL:		/*-131*/	return "OV_EINVAL";
	case OV_ENOTVORBIS:	/*-132*/	return "Not vorbis data.";
	case OV_EBADHEADER:	/*-133*/	return "Bad vorbis header";
	case OV_EVERSION:	/*-134*/	return "Vorbis version mismatch";
	case OV_ENOTAUDIO:	/*-135*/	return "OV_ENOTAUDIO";
	case OV_EBADPACKET:	/*-136*/	return "OV_EBADPACKET";
	case OV_EBADLINK:	/*-137*/	return "OV_EBADLINK";
	case OV_ENOSEEK:	/*-138*/	return "OV_ENOSEEK";
	
	default:
		return "Unknown ogg error";
	}
}