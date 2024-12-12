#include "FL/Fl_PNG_Image.H"
#include <memory>
#pragma once

// Contains all the images used to save laoding them at runtime
// Singleton design to avoid this aswell

enum class ImageType
{
	TRIANGLE = 0,
	SQUARE = 1,
	PENTAGON = 2,
	HEXAGON = 3,
	TEXTBOX = 4,
	ICON = 5,
	SPLASH = 6
};

class ImagePool
{
private:
	std::shared_ptr<Fl_PNG_Image> m_images[7];
	ImagePool();

public:

	~ImagePool();
	static void Initialise();
	static std::shared_ptr<Fl_PNG_Image> GetImage(ImageType _imageIndex);

};