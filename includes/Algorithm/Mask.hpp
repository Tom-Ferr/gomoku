#ifndef MASK_HPP
# define MASK_HPP

# include <gomoku.hpp>
# include <vector>

enum e_masktype
{
	HORIZONTAL,
	VERTICAL,
	CRESCENDO,
	DECRESCENDO,
	SUPERPOSITION
};

enum e_maskinnertype
{
	FULL,
	MIDDLE,
	EDGE,
	OUTER,
    SUBMASK,
    VECTOR
};


class Mask
{
public:
    typedef std::vector<BigInt> mask_vector;
    typedef std::vector<mask_vector> variations_vector;
    typedef std::vector<variations_vector> inner_map;
    typedef std::vector<inner_map> outer_map;
    static mask_vector _targets;

private:
    outer_map _masks;
    unsigned int _mask_size;
    unsigned int _board_sqrt;
    unsigned int _board_size;
    bool _submask;
    bool _mirror;
    const char _modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};

public:
    Mask();
    Mask(const int mask_size, unsigned int board_sqrt, bool submask=false, bool mirror=false);
    Mask(const Mask& other);
	Mask &operator=(const Mask& other);
    ~Mask();

    inner_map operator[](char);
	inner_map &at(char c);
    void print_mask(variations_vector &m) const;
    //BigInt targets(size_t pos) const;
	static BigInt &targets(size_t pos);

private:
    inner_map horizontal_mask() const;
    inner_map vertical_mask() const;
    inner_map crescendo_mask() const;
    inner_map decrescendo_mask() const;
    inner_map create_superpositions();
    mask_vector build_targets();
    void vectorize(mask_vector &dest, const int src, const char mode) const;
};
#endif