#ifndef MASK_HPP
# define MASK_HPP
# include <gomoku.hpp>
# include <map>
# include <vector>

class Mask
{
public:
    typedef std::vector<BigInt> mask_vector;
    typedef std::vector<mask_vector> variations_vector;
    typedef std::map<std::string, variations_vector> inner_map;
    typedef std::map<char, inner_map> outer_map;
private:
    outer_map _masks;
    unsigned int _mask_size;
    unsigned int _board_sqrt;
    unsigned int _board_size;
    bool _submask;
    mask_vector _targets;
    
public:
    Mask(const int mask_size, unsigned int board_sqrt, bool submask=false);
    Mask(const Mask& other);
	Mask &operator=(const Mask& other);
    ~Mask();

    inner_map operator[](char);

    void print_mask(variations_vector &m) const;
    BigInt targets(size_t pos) const;

private:
    inner_map horizontal_mask() const;
    inner_map vertical_mask() const;
    inner_map crescendo_mask() const;
    inner_map decrescendo_mask() const;
    void build_targets();
    
public:
};
#endif