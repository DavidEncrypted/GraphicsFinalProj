#include "global.h"
#include "TextureManager.h"



GLuint TextureManager::loadTexture(const std::string & filename, bool adjustgamma)
{
	Texture * existing = getTexture(filename);
	if (existing)
	{
		return existing->texid;
	}
	
	Texture newtexture;
	newtexture.name = filename;

	glGenTextures(1, &newtexture.texid);
	glBindTexture(GL_TEXTURE_2D, newtexture.texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (!loadTextureData(newtexture, adjustgamma))
	{
		std::cerr << "Failed to load texture " << filename << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &newtexture.texid);
		return 0;
	}
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	textures.push_back(newtexture);

	return newtexture.texid;
}

GLuint TextureManager::loadCubemap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    //int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {

    	unsigned char * data;
		png_uint_32 width, height;
		GLint format;
		if (readPNGdata("Models/Textures/" + faces[i], true, data, width, height, format))
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            //stbi_image_free(data);
            delete [] data;
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            delete [] data;
            //stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    skyboxtexture = textureID;
    return textureID;
}


Texture * TextureManager::getTexture(const std::string & filename)
{
	std::list<Texture>::iterator it;

	for (it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->name == filename)
		{
			return &(*it);
		}
	}

	return NULL;
}

// The ugly implementation of PNG loading...
bool TextureManager::loadTextureData(const Texture & texture, bool adjustgamma) const
{
	unsigned char * data;
	png_uint_32 width, height;
	GLint format;
	if (!readPNGdata(texture.name, adjustgamma, data, width, height, format))
		return false;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	delete [] data;

	return true;
}

bool TextureManager::readPNGdata(const std::string filename, bool adjustgamma, unsigned char *&data, png_uint_32 & width, png_uint_32 & height, GLint & format) const
{

	FILE * f = fopen(filename.c_str(), "rb");
	if (!f)
	{
		std::cout << "Failed to find texture " << filename.c_str() << "." << std::endl;
		return 0;
	}

	char sig[5];
	fread(sig, 1, 4, f);
	sig[4] = '\0';
	if (feof(f) || std::string("‰PNG") != std::string(sig))
	{
		std::cout << filename.c_str() << " is not a valid .png file." << std::endl;
		return 0;
	}


	png_structp png_ptr;
	png_infop info_ptr;

	png_bytepp row_pointers;
	png_uint_32 i;
	png_byte colortype;
	png_byte channels;

	if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0)))
	{
		return false;
	}
	if (!(info_ptr = png_create_info_struct(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false;
	}

	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 4);

	if (adjustgamma)
		png_set_gamma(png_ptr, 1.0, 1.0 / 2.2);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	colortype = png_get_color_type(png_ptr, info_ptr);

	row_pointers = png_get_rows(png_ptr, info_ptr);

	channels = png_get_channels(png_ptr, info_ptr);

	//ensure 4 byte aligned rows for opengl
	int rowsize = ((width * channels + 3) / 4) * 4;

	data = new unsigned char[height * rowsize];

	if (!data)
		return false;

	for (i = 0; i < height; ++i)
	{
		memcpy(data + i * rowsize, row_pointers[height - i - 1], width * channels);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	format = (colortype == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
	//for (int i = 0; i < 100; i++) std::cerr << data[i];
	return true;
}