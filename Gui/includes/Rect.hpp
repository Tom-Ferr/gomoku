#ifndef RECT_HPP
# define RECT_HPP
# include <algorithm>
# include <ostream>
class Rect
{
	public:
		size_t x;
		size_t y;
		size_t width;
		size_t height;
		Rect();
		Rect(int x, int y, int width, int height);
		Rect(const Rect& other);
		Rect& operator=(const Rect& other);
		~Rect();
		bool operator==(const Rect& other);
		bool operator!=(const Rect& other);
		bool contains(size_t x, size_t y) const;
		static Rect subrect(const Rect& parent, size_t width, size_t index);
		static Rect subrect(const Rect &parent, float ratio);
		static Rect subrect(const Rect &parent, float wratio, float hratio, int valign);
};

std::ostream& operator<<(std::ostream& os, const Rect& rect);

#endif