#include <Mask.hpp>

Mask::Mask(const int mask_size, const unsigned int board_sqrt)
: 
    _mask_size(mask_size),
    _board_sqrt(board_sqrt),
    _board_size(board_sqrt * board_sqrt)
{
        _masks = outer_map();
        _masks['h'] = horizontal_mask();
        _masks['v'] = vertical_mask();
        _masks['c'] = crescendo_mask();
        _masks['d'] = decrescendo_mask();
}
Mask::~Mask(){}

Mask::Mask::inner_map Mask::horizontal_mask() const
{
    Mask::inner_map map;
    BigInt base = BigInt(1);
    base = base << _mask_size;
    base--;
    BigInt full_base = base;
    BigInt mid_base = (base >> 2) << 1;
    BigInt edge_base = ~mid_base & full_base;

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        if (pos % _board_sqrt >= _board_sqrt - (_mask_size -2) || pos % _board_sqrt == 0)
        {
            BigInt zero = BigInt(0);
            mask_vector empty_vector;
            empty_vector.push_back(zero);
            full_vec.push_back(empty_vector);
            mid_vec.push_back(empty_vector);
            edge_vec.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - 1;
        BigInt mid_mask = mid_base << pos - 1;
        BigInt edge_mask = edge_base << pos - 1;

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        for (size_t j = 0; j < _mask_size - 2; j++)
        {
            if( (pos % _board_sqrt) - j <= 0)
                break;
            f_vec.push_back(full_mask >> j);

            m_vec.push_back(mid_mask >> j);

            e_vec.push_back(edge_mask >> j);
        }
        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;

    return map;
}

Mask::inner_map Mask::vertical_mask() const
{
    Mask::inner_map map;
    BigInt base = BigInt(1);
    for (size_t i = 1; i < _mask_size; i++)
    {
        base = (base << _board_sqrt) | 1;
    }
    BigInt full_base = base;
    BigInt mid_base = (base >> (2 * _board_sqrt)) << (1* _board_sqrt);
    BigInt edge_base = ~mid_base & full_base;

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        if (pos < _board_sqrt || pos >= _board_sqrt * (_board_sqrt - (_mask_size - 2)))
        {
            BigInt zero = BigInt(0);
            mask_vector empty_vector;
            empty_vector.push_back(zero);
            full_vec.push_back(empty_vector);
            mid_vec.push_back(empty_vector);
            edge_vec.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - _board_sqrt;
        BigInt mid_mask = mid_base << pos - _board_sqrt;
        BigInt edge_mask = edge_base << pos - _board_sqrt;

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        for (size_t j = 0; j < _mask_size - 2; j++)
        {
            int shift = j * _board_sqrt;
            if( pos - shift < _board_sqrt)
                break;
            f_vec.push_back(full_mask >> shift);

            m_vec.push_back(mid_mask >> shift);

            e_vec.push_back(edge_mask >> shift);
        }
        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;

    return map;
}

Mask::Mask::inner_map Mask::crescendo_mask() const
{
    Mask::inner_map map;
    BigInt base = BigInt(1);
    for (size_t i = 1; i < _mask_size; i++)
    {
        base = base << _board_sqrt | 1 << i;
    }
    BigInt full_base = base;
    BigInt mid_base = (base >> 2 *_board_sqrt) << (1 + _board_sqrt);
    BigInt edge_base = ~mid_base & full_base;

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        if ( pos < _board_sqrt + _mask_size - 2 ||pos % _board_sqrt < _mask_size - 2 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -2)) -2) ||pos % _board_sqrt == _board_sqrt -1) 
        {
            BigInt zero = BigInt(0);
            mask_vector empty_vector;
            empty_vector.push_back(zero);
            full_vec.push_back(empty_vector);
            mid_vec.push_back(empty_vector);
            edge_vec.push_back(empty_vector);
            continue;
        }
        if(pos % _board_sqrt == _mask_size - 2)
        {
            base = base << _board_sqrt;
        }
        BigInt full_mask = full_base << pos - (_board_sqrt + _mask_size - 2);
        BigInt mid_mask = mid_base << pos - (_board_sqrt + _mask_size - 2);
        BigInt edge_mask = edge_base << pos - (_board_sqrt + _mask_size - 2);

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        for (size_t j = 0; j < _mask_size - 2; j++)
        {
            int shift = (j * _board_sqrt) - j;
            if( (pos - shift) % _board_sqrt == _board_sqrt - 1 || pos - shift < _board_sqrt)
                break;
            f_vec.push_back(full_mask >> shift);

            m_vec.push_back(mid_mask >> shift);

            e_vec.push_back(edge_mask >> shift);
        }
        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;

    return map;
}

Mask::Mask::inner_map Mask::decrescendo_mask() const
{
    Mask::inner_map map;
    BigInt base = BigInt(1);
    for (size_t i = 1; i < _mask_size; i++)
    {
        base = (base << _board_sqrt + 1) | 1;
    }
    BigInt full_base = base;
    BigInt mid_base = (base >> 2 *_board_sqrt) << (_board_sqrt - 1);
    BigInt edge_base = ~mid_base & full_base;

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        if (pos % _board_sqrt > _board_sqrt - (_mask_size - 1) || pos < _board_sqrt + 1 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -2))) - (_mask_size -1))
        {
            
            BigInt zero = BigInt(0);
            mask_vector empty_vector;
            empty_vector.push_back(zero);
            full_vec.push_back(empty_vector);
            mid_vec.push_back(empty_vector);
            edge_vec.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - ( _board_sqrt + 1);
        BigInt mid_mask = mid_base << pos - ( _board_sqrt + 1);
        BigInt edge_mask = edge_base << pos - ( _board_sqrt + 1);

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        for (size_t j = 0; j < _mask_size - 2; j++)
        {
            int shift = (j * _board_sqrt) + j;
            if( (pos - shift) % _board_sqrt == 0 || pos - shift < _board_sqrt)
                break;
            f_vec.push_back(full_mask >> shift);

            m_vec.push_back(mid_mask >> shift);

            e_vec.push_back(edge_mask >> shift);
        }
        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;

    return map;
}
Mask::inner_map Mask::operator[](char c)
{
    return _masks[c];
}
void Mask::print_mask(Mask::variations_vector &m) const
{
    BigInt last = BigInt(1);
    last = last << _board_size - 1;
    BigInt zero = BigInt(0);
    for (size_t pos = 0; pos < _board_size; pos++)
    {
        char version = 'A';
        mask_vector::iterator var = m[pos].begin();
        for (; var != m[pos].end(); var++)
        {
            std::cout << "pos = " << pos << '.' << version <<  std::endl;
            for (size_t offset = 0; offset < _board_size; offset++)
            {
                if (offset % _board_sqrt == 0 && offset != 0) 
                    std::cout << std::endl;
                BigInt current = last >> offset;
                BigInt t = *var;
                if ((t & current) != zero)
                    std::cout << " X ";
                else
                    std::cout << " . ";
            }
            std::cout << std::endl;
            version++;
        }
        
    }
    
}