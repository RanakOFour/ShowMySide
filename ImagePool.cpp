#include "ImagePool.h"
#include "FL/Fl_PNG_Image.H"

//Another weird singletony thing
static ImagePool* m_self;

ImagePool::ImagePool()
{
	m_images[0] = new Fl_PNG_Image("./images/triangle.png");
	m_images[1] = new Fl_PNG_Image("./images/square.png");
	m_images[2] = new Fl_PNG_Image("./images/pentagon.png");
	m_images[3] = new Fl_PNG_Image("./images/hexagon.png");
	m_images[4] = new Fl_PNG_Image("./images/textbox.png");
	m_images[5] = new Fl_PNG_Image("./images/icon.png");
	m_images[6] = new Fl_PNG_Image("./images/splash.png");

	for (int i = 0; i < 4; i++)
	{
		m_images[i] = (Fl_PNG_Image*)m_images[i]->copy(200, 200);
	}

	m_images[4] = (Fl_PNG_Image*)m_images[4]->copy(200, 75);

	m_images[6] = (Fl_PNG_Image*)m_images[6]->copy(440, 160);
}

ImagePool::~ImagePool()
{
	delete m_images;
}

void ImagePool::Initialise()
{
	if (m_self == nullptr)
	{
		m_self = new ImagePool();
	}
}

Fl_PNG_Image* ImagePool::GetImage(ImageType _index)
{
	return m_self->m_images[_index];
}