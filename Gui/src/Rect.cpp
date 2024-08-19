#include <Rect.hpp>

Rect::Rect()
: x(0), y(0), width(0), height(0)
{}

Rect::Rect(int x, int y, int width, int height)
: x(x), y(y), width(width), height(height)
{}

Rect::Rect(const Rect& other)
: x(other.x), y(other.y), width(other.width), height(other.height)
{}

Rect& Rect::operator=(const Rect& other)
{
	x = other.x;
	y = other.y;
	width = other.width;
	height = other.height;
	return *this;
}

Rect::~Rect()
{}

bool Rect::operator==(const Rect& other)
{
	return (x == other.x && y == other.y
			&& width == other.width
			&& height == other.height);
}

bool Rect::operator!=(const Rect& other)
{
	return (!(*this == other));
}

bool Rect::contains(size_t x, size_t y)
{
	return x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height;
}

/*
** creates a subrect of the parent rect
*/
Rect Rect::subrect(const Rect& parent, size_t width, size_t index)
{
	Rect r;

	r.width = width;
	r.height = width;
	size_t parent_xlen = parent.width / width;
	r.x = parent.x + (index % parent_xlen) * width;
	r.y = parent.y + (index / parent_xlen) * width;
	if (r.x + r.width > parent.x + parent.width)
		r.width = parent.x + parent.width - r.x;
	if (r.y + r.height > parent.y + parent.height)
		r.height = parent.y + parent.height - r.y;
	return r;
}

/*
creates a centralized squared subrect with the ratio of the parent
*/
Rect Rect::subrect(const Rect &parent, float ratio)
{
	Rect r;

	r.width = std::min(parent.width, parent.height) * ratio;
	r.height = r.width;
	r.x = parent.x + (parent.width - r.width) / 2;
	r.y = parent.y + (parent.height - r.height) / 2;
	return r;
}

/*
** horizontal align is always center
** vertical align: 0 = top, 1 = center, 2 = bottom
*/
Rect Rect::subrect(const Rect &parent, float wratio, float hratio, int valign)
{
	Rect r;

	r.width = parent.width * wratio;
	r.height = parent.height * hratio;
	r.x = parent.x + (parent.width - r.width) / 2;
	r.y = parent.y;
	if (valign == 1)
		r.y += (parent.height - r.height) / 2;
	else if (valign == 2)
		r.y += parent.height - r.height;
	return r;
}

std::ostream& operator<<(std::ostream& os, const Rect& rect) {
	os << "Rect: " << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ")";
	return os;
}
