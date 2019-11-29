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
	std::cerr << "Created texid: " << newtexture.texid << std::endl;
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
    	int numrots;
    	if (i == 0) numrots = 1;
    	if (i == 1) numrots = 3;
    	if (i == 2) numrots = 0;
    	if (i == 3) numrots = 2;
    	if (i == 4) numrots = 3;
    	if (i == 5) numrots = 3;
    	



    	unsigned char * data;
		png_uint_32 width, height;
		GLint format;
		if (readPNGdata("Models/Textures/Skybox/" + faces[i], true, data, width, height, format, numrots))
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
	if (!readPNGdata(texture.name, adjustgamma, data, width, height, format, 0))
		return false;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	delete [] data;

	return true;
}

void TextureManager::rotateonce(unsigned char *&data, png_uint_32 height,  png_uint_32 width, png_byte channels, int rowsize) const{
	int stepsize = rowsize / width;
	std::cerr << "height: " << height << std::endl;
	std::cerr << "width: " << width << std::endl;
	std::cerr << "stepsize: " << stepsize << std::endl;
	std::cerr << "rowsize: " << rowsize << std::endl;

	int N = height;


	// mat[x][y] === data[y * (x*stepsize)]
	//	
	// data[y * (rowsize) + (x*stepsize)] = data[y * (x*stepsize)]
	// data[y * (x*stepsize) + 1] = data[y * (x*stepsize) + 1]
	// data[y * (x*stepsize) + 2] = data[y * (x*stepsize) + 2]
	// data[y * (x*stepsize) + 3] = data[y * (x*stepsize) + 3]

	// Credit:
	// https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/
	// Consider all squares one by one 
    for (int x = 0; x < N / 2; x++) 
    { 
        // Consider elements in group of 4 in  
        // current square 
        for (int y = x; y < N-x-1; y++) 
        { 
            // store current cell in temp variable 
            //unsigned char temp = mat[x][y]; 
        	unsigned char temp0 = data[y * (rowsize) + (x*stepsize)];
			unsigned char temp1 = data[y * (rowsize) + (x*stepsize) + 1];
			unsigned char temp2 = data[y * (rowsize) + (x*stepsize) + 2];
			//std::cerr << (int)temp0 << " ";
			//unsigned char temp3 = data[y * (x*stepsize) + 3];
  
            // move values from right to top 
            //mat[x][y] = mat[y][N-1-x]; 
        	data[y * (rowsize) + (x*stepsize)]     = data[(N-1-x) * (rowsize) + ((y)*stepsize)];
			data[y * (rowsize) + (x*stepsize) + 1] = data[(N-1-x) * (rowsize) + ((y)*stepsize) + 1];
			data[y * (rowsize) + (x*stepsize) + 2] = data[(N-1-x) * (rowsize) + ((y)*stepsize) + 2];
			//data[y * (x*stepsize) + 3] = data[(N-1-x) * ((y)*stepsize) + 3];
  
            // move values from bottom to right 
            //mat[y][N-1-x] = mat[N-1-x][N-1-y]; 
        	data[(N-1-x) * (rowsize) + ((y)*stepsize)]     = data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize)];
			data[(N-1-x) * (rowsize) + ((y)*stepsize) + 1] = data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize) + 1];
			data[(N-1-x) * (rowsize) + ((y)*stepsize) + 2] = data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize) + 2];
			//data[(N-1-x) * ((y)*stepsize) + 3] = data[(N-1-y) * ((N-1-x)*stepsize) + 3];
 
            // move values from left to bottom 
            //mat[N-1-x][N-1-y] = mat[N-1-y][x]; 
  			data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize)]     = data[(x) * (rowsize) + ((N-1-y)*stepsize)];
			data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize) + 1] = data[(x) * (rowsize) + ((N-1-y)*stepsize) + 1];
			data[(N-1-y) * (rowsize) + ((N-1-x)*stepsize) + 2] = data[(x) * (rowsize) + ((N-1-y)*stepsize) + 2];
			//data[(N-1-y) * ((N-1-x)*stepsize) + 3] = data[(x) * ((N-1-y)*stepsize) + 3];
            // assign temp to left 
            //mat[N-1-y][x] = temp; 
  			data[(x) * (rowsize) + ((N-1-y)*stepsize)]     = temp0;
			data[(x) * (rowsize) + ((N-1-y)*stepsize) + 1] = temp1;
			data[(x) * (rowsize) + ((N-1-y)*stepsize) + 2] = temp2;
			//data[(x) * ((N-1-y)*stepsize) + 3] = temp3;
        } 
    } 
}

bool TextureManager::readPNGdata(const std::string filename, bool adjustgamma, unsigned char *&data, png_uint_32 & width, png_uint_32 & height, GLint & format, int rotatenum) const
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

	if (rotatenum > 0)
		for (int i = 0; i < rotatenum; i++)
			rotateonce(data, height, width, channels, rowsize);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	format = (colortype == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
	if (format == GL_RGB) std::cerr << filename <<  " Texture RGB" << std::endl;
	else if (format == GL_RGBA) std::cerr << filename << "Texture RGBA" << std::endl;
	//for (int i = 0; i < 100; i++) std::cerr << data[i];
	return true;
}