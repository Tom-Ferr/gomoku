#ifndef COLOR_HPP
# define COLOR_HPP

/*
** Class for color representation.
*/
class Color
{
	private:
		typedef unsigned char uint8_t;

	public:
		static Color white;
		static Color black;
		static Color yellow;
		static Color white_alpha;
		static Color black_alpha;

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color();
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(const Color &other);
		~Color();
		Color &operator=(const Color &other);
		bool operator==(const Color &other) const;
		bool operator!=(const Color &other) const;
};

#endif
