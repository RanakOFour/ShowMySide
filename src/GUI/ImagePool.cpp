#include "GUI/ImagePool.h"
#include "FL/Fl_PNG_Image.H"
#include <memory>
#include <cstdio>


// Weird singletony thing, the compiler doesn't like it being a smart pointer
static ImagePool* m_self;

void ImagePool::Initialise()
{
	if (m_self == nullptr)
	{
		m_self = new ImagePool();
		printf("Initialised ImagePool\n");
	}
}

ImagePool::ImagePool()
{
	m_images[0] = std::make_shared<Fl_PNG_Image>("./images/triangle.png");
	m_images[1] = std::make_shared<Fl_PNG_Image>("./images/square.png");
	m_images[2] = std::make_shared<Fl_PNG_Image>("./images/pentagon.png");
	m_images[3] = std::make_shared<Fl_PNG_Image>("./images/hexagon.png");
	m_images[4] = std::make_shared<Fl_PNG_Image>("./images/textbox.png");
	m_images[5] = std::make_shared<Fl_PNG_Image>("./images/icon.png");
	m_images[6] = std::make_shared<Fl_PNG_Image>("./images/splash.png");


	// Swap the contents of shared ptrs with resized images so the original ones are deleted from memory

	for (int i = 0; i < 4; i++)
	{
		Fl_PNG_Image* sizedImage = (Fl_PNG_Image*)m_images[i].get()->copy(200, 200);
		std::shared_ptr<Fl_PNG_Image> sharedSizedImage = std::shared_ptr<Fl_PNG_Image>(sizedImage);
		m_images[i].swap(sharedSizedImage);

	}

	Fl_PNG_Image* sizedTextbox = (Fl_PNG_Image*)m_images[4].get()->copy(200, 75);
	std::shared_ptr<Fl_PNG_Image> sharedSizedTextbox = std::shared_ptr<Fl_PNG_Image>(sizedTextbox);
	m_images[4].swap(sharedSizedTextbox);

	Fl_PNG_Image* sizedSplash = (Fl_PNG_Image*)m_images[6].get()->copy(440, 160);
	std::shared_ptr<Fl_PNG_Image> sharedSizedSplash = std::shared_ptr<Fl_PNG_Image>(sizedSplash);
	m_images[6].swap(sharedSizedSplash);
}

ImagePool::~ImagePool()
{
}

std::shared_ptr<Fl_PNG_Image> ImagePool::GetImage(ImageType _index)
{
	return m_self->m_images[(int)_index];
}