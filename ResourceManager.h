#ifndef ResourceManager_H
#define ResourceManager_H

#include "Texture.h"
#include "Sound.h"
#include "OpenALContext.h"

#include <string>
#include <vector>
#include <map>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void bindTexture(const std::string &path);
	void loadTexture(const std::string &path);
	void preloadTextures(const std::vector<std::string> &paths);

	void loadSound(const std::string &path);
	void playSound(const std::string &path, bool loop = true);
	void stopSound(const std::string &path);
	void updatePlayingSounds();

private:
	OpenALContext soundContext;
	std::string resourcesRoot;

	std::map<std::string, Texture *> textures;
	std::string bindedTexture;
	std::map<std::string, Sound *> sounds;
};

extern ResourceManager &resourceManager;

#endif
