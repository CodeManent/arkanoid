#ifndef Texture_H
#define Texture_H

#include <string>
#include <vector>

class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

	void loadFromFile(const std::string &path);
	void uploadToGraphicsCard();
	void bind() const;

private:
	/*GLuint*/unsigned int texturePointer;
	/*GLuint*/unsigned int format;
	unsigned int width;
	unsigned int height;
	std::string path;
	std::vector<unsigned char> imageData;
};

#endif
