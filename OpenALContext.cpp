#include "OpenALContext.h"

#include <stdexcept>
#include <string>
#include <memory>

#include <al.h>
#include <alc.h>

OpenALContext::OpenALContext()
:device(NULL)
,context(NULL)
{
	std::string errMsg("OpenALContext: ");

	//open a sound device
	device = alcOpenDevice(NULL);
	if(device == NULL)
	{
		errMsg += "Coundn't open a sound device";
		throw std::runtime_error( errMsg );
	}

	//create a context
	context = alcCreateContext(device, NULL);
	if(context == NULL)
	{
		ALCenum err = alcGetError(device);

		errMsg += "Context creation error: ";

		switch(err)
		{
		case ALC_INVALID_VALUE:
			errMsg += "A contet can't be created for the selected device";
			break;

		case ALC_INVALID_DEVICE:
			errMsg += "The selected device is not a valid output device";
				break;

		default:
			errMsg += "Unspecified  error";
			break;
		}

		//close the opened device before throwing
		alcCloseDevice(device);

		throw std::runtime_error( errMsg );
	}

	ALCboolean retval = alcMakeContextCurrent(context);
	if(retval != ALC_TRUE)
	{
		ALCenum err = alcGetError(device);

		//cleanup
		alcDestroyContext(context);
		alcCloseDevice(device);

		errMsg += "Error at making context current: ";


		if(err == ALC_INVALID_CONTEXT)
		{
			errMsg += "The specified context is onvalid";
			throw std::runtime_error( errMsg);
		}
		else
		{
			errMsg += "Unspecified error";
			throw std::runtime_error( errMsg );
		}
	}

	//set listener parameters
	{
		//position of the listener
		ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f };
		alListenerfv( AL_POSITION, listenerPos );
	}
	{
		//velocity of the listener
		ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
		alListenerfv( AL_VELOCITY, listenerVel );
	}
	{
		// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
		ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
		alListenerfv( AL_ORIENTATION, listenerOri );
	}

	if(alGetError() != AL_NO_ERROR)
	{
		errMsg += "Error setting listener parameters";
		throw std::runtime_error( errMsg );
	}
}

OpenALContext::~OpenALContext()
{
	if(context != NULL)
		alcDestroyContext(context);

	if(device != NULL)
		alcCloseDevice(device);
}
