#ifndef MASK_HPP
# define MASK_HPP
# include <gomoku.hpp>
# include <map>
# include <vector>

class Mask
{
private:
typedef std::vector<BigInt> mask_vector;
typedef std::vector<mask_vector> variations_vector;
typedef std::map<std::string, variations_vector> inner_map;
typedef std::map<char, inner_map> outer_map;

outer_map _masks;
const unsigned int _mask_size;
const unsigned int _board_sqrt;
const unsigned int _board_size;
//bool _submask;

public:
    Mask(const int mask_size, unsigned int board_sqrt);
    ~Mask();

    inner_map operator[](char);

    inner_map horizontal_mask() const;
    inner_map vertical_mask() const;
    inner_map crescendo_mask() const;
    inner_map decrescendo_mask() const;
    void print_mask(variations_vector &m) const;
};
#endif