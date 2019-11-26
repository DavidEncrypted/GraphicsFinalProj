#pragma once


struct Texture
{
	std::string name;
	GLuint texid;
};


class TextureManager
{
public:
	GLuint loadTexture(const std::string & filename, bool adjustgamma);
	GLuint loadCubemap(std::vector<std::string> faces);
private:
	std::list<Texture> textures;
	GLuint skyboxtexture;
	Texture * getTexture(const std::string & filename);
	

	bool loadTextureData(const Texture & texture, bool adjustgamma) const;
	bool readPNGdata(const std::string filename, bool adjustgamma, unsigned char *&data, png_uint_32 & width, png_uint_32 & height, GLint & format) const;

};
