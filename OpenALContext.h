#ifndef OpenALContext_H
#define OpenALContext_H

#include <alc.h>


class OpenALContext{
public:
	OpenALContext();
	~OpenALContext();

private:
	ALCdevice* device;
	ALCcontext *context;
};


#endif
