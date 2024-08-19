#include <Color.hpp>

Color Color::white = Color(255, 255, 255);
Color Color::black = Color(100, 100, 100);
Color Color::yellow = Color(255, 255, 120, 150);
Color Color::white_alpha = Color(255, 255, 255, 150);
Color Color::black_alpha = Color(100, 100, 100, 150);

Color::Color()
: r(255), g(255), b(255), a(255)
{};

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
: r(r), g(g), b(b), a(a)
{};

Color::Color(uint8_t r, uint8_t g, uint8_t b)
: r(r), g(g), b(b), a(255)
{};

Color::Color(const Color &other)
: r(other.r), g(other.g), b(other.b), a(other.a)
{};
Color::~Color()
{};

Color &Color::operator=(const Color &other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    return *this;
};

bool Color::operator==(const Color &other) const
{
    return (r == other.r && g == other.g
            && b == other.b && a == other.a);
};

bool Color::operator!=(const Color &other) const
{
    return (!(*this == other));
};
