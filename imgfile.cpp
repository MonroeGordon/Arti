/**********************************************************************************************************************/
// ArtiCore
// File: imgfile.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Static variable definitions
std::string ImageFile::PATH_NAME = "C:\\Synap\\Projects\\Arti\\Artilab\\resources\\images\\";

//ImageFile constructor function
ImageFile::ImageFile(GLuint tex, float x, float y) :
	w(x),
	h(y),
	t(tex)
{

}

//ImageFile destructor function
ImageFile::~ImageFile() {
	w = 0.0f;
	h = 0.0f;
	t = 0;
}

//ImageFile load file function
imgfile* ImageFile::loadFile(const std::string& filename) {
	int imgWidth = 0;
	int imgHeight = 0;
	unsigned char* imgData = stbi_load((PATH_NAME + filename).c_str(), &imgWidth, &imgHeight, NULL, 4);

	if (imgData == NULL)
		return NULL;

	for (int i = 0; i < imgWidth * imgHeight * 4; i += 4) {
		if (imgData[i] == 255 && imgData[i + 1] == 255 && imgData[i + 2] == 255)
			imgData[i + 3] = 0;
	}

	GLuint imgTexture;
	glGenTextures(1, &imgTexture);
	glBindTexture(GL_TEXTURE_2D, imgTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_image_free(imgData);

	imgfile* ret = (imgfile*)malloc(sizeof(imgfile));

	if (ret != NULL)
		*ret = ImageFile(imgTexture, (float)imgWidth, (float)imgHeight);

	return ret;
}

//ImageFile get width function
float ImageFile::width() {
	return w;
}

//ImageFile get height function
float ImageFile::height() {
	return h;
}

//ImageFile get tecture function
GLuint ImageFile::texture() {
	return t;
}