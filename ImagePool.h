#include "FL/Fl_PNG_Image.H"

#pragma once

// Contains all the images used to save laoding them at runtime
// Singleton design to avoid this aswell

class ImagePool
{
private:
	Fl_PNG_Image* m_images[6];
	ImagePool();

public:
	enum ImageType
	{
		TRIANGLE = 0,
		SQUARE = 1,
		PENTAGON = 2,
		HEXAGON = 3,
		TEXTBOX = 4,
		ICON = 5
	};

	~ImagePool();
	static void Initialise();
	static Fl_PNG_Image* GetImage(ImageType _imageIndex);

};

