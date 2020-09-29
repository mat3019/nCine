#include "Colorf.h"
#include "Color.h"
#include "ColorHdr.h"
#include <nctl/algorithms.h>

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

const Colorf Colorf::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Colorf Colorf::White(1.0f, 1.0f, 1.0f, 1.0f);
const Colorf Colorf::Red(1.0f, 0.0f, 0.0f, 1.0f);
const Colorf Colorf::Green(0.0f, 1.0f, 0.0f, 1.0f);
const Colorf Colorf::Blue(0.0f, 0.0f, 1.0f, 1.0f);
const Colorf Colorf::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
const Colorf Colorf::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
const Colorf Colorf::Cyan(0.0f, 1.0f, 1.0f, 1.0f);

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

Colorf::Colorf()
    : Colorf(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Colorf::Colorf(float red, float green, float blue)
    : Colorf(red, green, blue, 1.0f)
{
}

Colorf::Colorf(float red, float green, float blue, float alpha)
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	set(red, green, blue, alpha);
}

Colorf::Colorf(const float channels[NumChannels])
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	setVec(channels);
}

Colorf::Colorf(const Color &color)
    : channels_(nctl::StaticArrayMode::EXTEND_SIZE)
{
	const float inv = 1.0f / 255.0f;
	channels_[0] = static_cast<float>(color.r() * inv);
	channels_[1] = static_cast<float>(color.g() * inv);
	channels_[2] = static_cast<float>(color.b() * inv);
	channels_[3] = static_cast<float>(color.a() * inv);
}

Colorf::Colorf(const ColorHdr &color)
    : Colorf(color.r(), color.g(), color.b(), 1.0f)
{
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void Colorf::set(float red, float green, float blue, float alpha)
{
	channels_[0] = nctl::clamp(red, 0.0f, 1.0f);
	channels_[1] = nctl::clamp(green, 0.0f, 1.0f);
	channels_[2] = nctl::clamp(blue, 0.0f, 1.0f);
	channels_[3] = nctl::clamp(alpha, 0.0f, 1.0f);
}

void Colorf::set(float red, float green, float blue)
{
	channels_[0] = nctl::clamp(red, 0.0f, 1.0f);
	channels_[1] = nctl::clamp(green, 0.0f, 1.0f);
	channels_[2] = nctl::clamp(blue, 0.0f, 1.0f);
}

void Colorf::setVec(const float channels[NumChannels])
{
	set(channels[0], channels[1], channels[2], channels[3]);
}

void Colorf::setAlpha(float alpha)
{
	channels_[3] = nctl::clamp(alpha, 0.0f, 1.0f);
}

Colorf &Colorf::operator=(const Color &color)
{
	const float inv = 1.0f / 255.0f;
	channels_[0] = static_cast<float>(color.r() * inv);
	channels_[1] = static_cast<float>(color.g() * inv);
	channels_[2] = static_cast<float>(color.b() * inv);
	channels_[3] = static_cast<float>(color.a() * inv);

	return *this;
}

Colorf &Colorf::operator+=(const Colorf &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] + color.channels_[i];
		channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return *this;
}

Colorf &Colorf::operator-=(const Colorf &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] - color.channels_[i];
		channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return *this;
}

Colorf &Colorf::operator*=(const Colorf &color)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] * color.channels_[i];
		channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return *this;
}

Colorf &Colorf::operator*=(float scalar)
{
	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] * scalar;
		channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return *this;
}

Colorf Colorf::operator+(const Colorf &color) const
{
	Colorf result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] + color.channels_[i];
		result.channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return result;
}

Colorf Colorf::operator-(const Colorf &color) const
{
	Colorf result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] - color.channels_[i];
		result.channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return result;
}

Colorf Colorf::operator*(const Colorf &color) const
{
	Colorf result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] * color.channels_[i];
		result.channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return result;
}

Colorf Colorf::operator*(float scalar) const
{
	Colorf result;

	for (unsigned int i = 0; i < NumChannels; i++)
	{
		const float channelValue = channels_[i] * scalar;
		result.channels_[i] = nctl::clamp(channelValue, 0.0f, 1.0f);
	}

	return result;
}

}
