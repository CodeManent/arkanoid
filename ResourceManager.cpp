#include "ResourceManager.h"

ResourceManager &resourceManager = ResourceManager();



ResourceManager::ResourceManager()
:soundContext()
,resourcesRoot("..\\assets\\")
,textures()
,bindedTexture("")
,sounds()
{
}






ResourceManager::~ResourceManager()
{
	std::map<std::string, Texture *>::iterator texture;
	for(texture = textures.begin(); texture != textures.end(); ++texture)
	{
		delete texture->second;
	}
	textures.clear();
	bindedTexture = "";
}






void ResourceManager::bindTexture(const std::string &path)
{
	if(path == "")
		throw std::runtime_error("ResourceManager::bindTexture: empty path");


	if(bindedTexture == path)
	{
		//texture is already binded
		return;
	}

	std::map<std::string, Texture *>::const_iterator texture;
	texture = textures.find(path);
	if(texture == textures.end())
	{
		//requested texture is not loaded, load it now!
		loadTexture(path);
	}
	else
	{
		texture->second->bind();
	}

	bindedTexture = path;
}






void ResourceManager::loadTexture(const std::string &path)
{
	if(path == "")
		throw std::runtime_error("ResourceManager::loadTexture: empty path");

	std::map<std::string, Texture *>::const_iterator texture;
	texture = textures.find(path);
	if(texture != textures.end())
	{
		//texture already loaded
		return;
	}

	textures[path] = new Texture(resourcesRoot + path);
	bindedTexture = path;
}






void ResourceManager::preloadTextures(const std::vector<std::string> &paths)
{
	std::vector<std::string>::const_iterator path;
	for(path = paths.begin(); path != paths.end(); ++path)
	{
		loadTexture(*path);
		if(path == paths.end()-1)
		{
			bindedTexture = *path;
		}
	}
}





void ResourceManager::loadSound(const std::string &path)
{
	if(path == "")
		throw std::runtime_error( "ResourceManager::loadSound: empty path" );

	std::map<std::string, Sound *>::const_iterator sound;
	sound = sounds.find(path);
	if( sound != sounds.end() )
	{
		return;
	}

	sounds[path] = new Sound(resourcesRoot + path);
}





void ResourceManager::playSound(const std::string &path, bool loop)
{
	if(path == "")
		throw std::runtime_error( "ResourceManager::loadSound: empty path" );

	std::map<std::string, Sound *>::const_iterator sound;
	sound = sounds.find(path);
	if( sound == sounds.end() )
	{
		//sound not loaded
		loadSound(path);
	}

	if( sounds[path]->play() == false )
	{
		throw std::runtime_error( path + ": refused to play" );

	}
}

void ResourceManager::stopSound(const std::string &path)
{
	if(path == "")
		throw std::runtime_error( "ResourceManager::loadSound: empty path" );

	std::map<std::string, Sound *>::const_iterator sound;
	sound = sounds.find(path);
	if( sound == sounds.end() )
	{
		//sound not found
		return;
	}

	sounds[path]->stop();
}




//update all sounds
void ResourceManager::updatePlayingSounds()
{
	std::map<std::string, Sound*>::iterator sound;

	for( sound = sounds.begin(); sound != sounds.end(); ++sound )
	{
		if( sound->second->playing() )
		{
			sound->second->update();
		}
	}
}
