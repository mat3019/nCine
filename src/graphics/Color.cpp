#include "Color.h"
#include "Colorf.h"
#include <nctl/algorithms.h>

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

const Color Color::Black(0, 0, 0, 255);
const Color Color::White(255, 255, 255, 255);
const Color Color::Red(255, 0, 0, 255);
const Color Color::Green(0, 255, 0, 255);
const Color Color::Blue(0, 0, 255, 255);
const Color Color::Yellow(255, 255, 0, 255);
const Color Color::Magenta(255, 0, 255, 255);
const Color Color::Cyan(0, 255, 255, 255);

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

Color::Color()
    : Color(255, 255, 255, 255)
{
}

Color::Color(unsigned int red, unsigned int green, unsigned int blue)
    : Color(red, green, blue, 255)
{
}

Color::Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	set(red, green, blue, alpha);
}

Color::Color(unsigned int hex)
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	set(hex);
	setAlpha(255);
}

Color::Color(const unsigned int channels[NumChannels])
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	setVec(channels);
}

Color::Color(const Colorf &color)
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	channels_[0] = static_cast<unsigned char>(color.r() * 255);
	channels_[1] = static_cast<unsigned char>(color.g() * 255);
	channels_[2] = static_cast<unsigned char>(color.b() * 255);
	channels_[3] = static_cast<unsigned char>(color.a() * 255);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void Color::set(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	channels_[0] = static_cast<unsigned char>(red);
	channels_[1] = static_cast<unsigned char>(green);
	channels_[2] = static_cast<unsigned char>(blue);
	channels_[3] = static_cast<unsigned char>(alpha);
}

void Color::set(unsigned int red, unsigned int green, unsigned int blue)
{
	channels_[0] = static_cast<unsigned char>(red);
	channels_[1] = static_cast<unsigned char>(green);
	channels_[2] = static_cast<unsigned char>(blue);
}

void Color::set(unsigned int hex)
{
	channels_[0] = static_cast<unsigned char>((hex & 0xFF0000) >> 16);
	channels_[1] = static_cast<unsigned char>((hex & 0xFF00) >> 8);
	channels_[2] = static_cast<unsigned char>(hex & 0xFF);
}

void Color::setVec(const unsigned int channels[NumChannels])
{
	set(channels[0], channels[1], channels[2], channels[3]);
}

void Color::setAlpha(unsigned int alpha)
{
	channels_[3] = static_cast<unsigned char>(alpha);
}

Color &Color::operator=(const Colorf &color)
{
	channels_[0] = static_cast<unsigned char>(color.r() * 255.0f);
	channels_[1] = static_cast<unsigned char>(color.g() * 255.0f);
	channels_[2] = static_cast<unsigned char>(color.b() * 255.0f);
	channels_[3] = static_cast<unsigned char>(color.a() * 255.0f);

	return *this;
}

bool Color::operator==(const Color &color) const
{
	return (r() == color.r() && g() == color.g() &&
	        b() == color.b() && a() == color.a());
}

Color &Color::operator+=(const Color &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		unsigned int channelValue = channels_[i] + color.channels_[i];
		channelValue = nctl::clamp(channelValue, 0U, 255U);
		channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return *this;
}

Color &Color::operator-=(const Color &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		unsigned int channelValue = channels_[i] - color.channels_[i];
		channelValue = nctl::clamp(channelValue, 0U, 255U);
		channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return *this;
}

Color &Color::operator*=(const Color &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		float channelValue = channels_[i] * (color.channels_[i] / 255.0f);
		channelValue = nctl::clamp(channelValue, 0.0f, 255.0f);
		channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return *this;
}

Color &Color::operator*=(float scalar)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		float channelValue = channels_[i] * scalar;
		channelValue = nctl::clamp(channelValue, 0.0f, 255.0f);
		channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return *this;
}

Color Color::operator+(const Color &color) const
{
	Color result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		unsigned int channelValue = channels_[i] + color.channels_[i];
		channelValue = nctl::clamp(channelValue, 0U, 255U);
		result.channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return result;
}

Color Color::operator-(const Color &color) const
{
	Color result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		unsigned int channelValue = channels_[i] - color.channels_[i];
		channelValue = nctl::clamp(channelValue, 0U, 255U);
		result.channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return result;
}

Color Color::operator*(const Color &color) const
{
	Color result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		float channelValue = channels_[i] * (color.channels_[i] / 255.0f);
		channelValue = nctl::clamp(channelValue, 0.0f, 255.0f);
		result.channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return result;
}

Color Color::operator*(float scalar) const
{
	Color result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		float channelValue = channels_[i] * scalar;
		channelValue = nctl::clamp(channelValue, 0.0f, 255.0f);
		result.channels_[i] = static_cast<unsigned char>(channelValue);
	}

	return result;
}

}
