#include <Mask.hpp>

Mask::Mask(const int mask_size, const unsigned int board_sqrt, bool submask)
: 
    _mask_size(mask_size),
    _board_sqrt(board_sqrt),
    _board_size(board_sqrt * board_sqrt),
    _submask(submask)
{
        _masks = outer_map();
        _masks['h'] = horizontal_mask();
        _masks['v'] = vertical_mask();
        _masks['c'] = crescendo_mask();
        _masks['d'] = decrescendo_mask();
        build_targets();
}

Mask::Mask(const Mask& other)
{
	_mask_size = other._mask_size;
	_board_sqrt = other._board_sqrt;
	_board_size = other._board_size;
	_submask = other._submask;
    _masks = other._masks;
}

Mask &Mask::operator=(const Mask& other)
{
	if (this != &other)
    {
		_mask_size = other._mask_size;
	    _board_sqrt = other._board_sqrt;
	    _board_size = other._board_size;
	    _submask = other._submask;
        _masks = other._masks;
    }
	return *this;
}

Mask::~Mask(){}

void Mask::build_targets()
{
    mask_vector vec;
    for (size_t i = 0; i < _board_size; i++)
    {
        BigInt n = BigInt((1 << i));
        vec.push_back(n);
    }
    this->_targets = vec;
}

BigInt Mask::targets(size_t pos) const
{
    return _targets[pos];
}

void Mask::vectorize(Mask::mask_vector &dest, int src, char mode)
{
    int inc;
    int shift;
    int pos;
    switch (mode)
    {
    case 'h':
        inc = 0;
        shift = 1;
        break;
    case 'v':
        inc = 0;
        shift = _board_sqrt;
        break;
    case 'c':
        inc = -1;
        shift = _board_sqrt;
        break;
    case 'd':
        inc = 1;
        shift = _board_sqrt;
        break;
    default:
        return ;
    }

    pos = src - shift - inc;
    for (size_t i = 0; i < _mask_size + 2; pos = pos + shift + inc)
    {
        BigInt t;
        
        if (pos < 0 || pos >= _board_size)
            t = BigInt(0);
        else
            t = targets(pos);
        dest.push_back(t);
    }
    return ;
}

Mask::Mask::inner_map Mask::horizontal_mask()
{
    Mask::inner_map map;
    BigInt base = BigInt(1);
    base = base << _mask_size;
    base--;
    BigInt full_base = base;
    BigInt mid_base = (base >> 2) << 1;
    BigInt edge_base = ~mid_base & full_base;

    if (_submask == true)
    {
        edge_base = ((full_base << 2) | full_base) ^ (full_base << 1);
    }

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;
    variations_vector sub_vec;
    variations_vector vectorized;

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
            sub_vec.push_back(empty_vector);
            vectorized.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - 1;
        BigInt mid_mask = mid_base << pos - 1;
        BigInt edge_mask = edge_base << pos - 1;

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        

        if (_submask == false)
        {
            for (size_t j = 0; j < _mask_size - 2; j++)
            {
                if( (pos % _board_sqrt) - j <= 0)
                    break;
                f_vec.push_back(full_mask >> j);

                m_vec.push_back(mid_mask >> j);

                e_vec.push_back(edge_mask >> j);
            }
        }

        else
        {
            if (pos % _board_sqrt == 1)
                edge_mask = ((edge_mask >> 2) & full_mask) << 1;
            else if (pos % _board_sqrt > _board_sqrt - (_mask_size))
                edge_mask = (full_mask & edge_mask) >> 1;
            else
                edge_mask = edge_mask >> 1;
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, 'h');
            vectorized.push_back(vec);

            mask_vector s_vec;
            BigInt sub_mask = full_mask & (full_mask << 1);
            s_vec.push_back(sub_mask);
            s_vec.push_back((sub_mask >> 1));
            
            sub_vec.push_back(s_vec);
        }

        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;
    if (_submask == true)
    {
        map["submask"] = sub_vec;
        map["vectors"] = vectorized;
    }

    return map;
}

Mask::inner_map Mask::vertical_mask()
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

    if (_submask == true)
    {
        edge_base = ((full_base << (2 * _board_sqrt)) | full_base) ^ (full_base << (1* _board_sqrt));
    }

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;
    variations_vector sub_vec;
    variations_vector vectorized;

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
            sub_vec.push_back(empty_vector);
            vectorized.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - _board_sqrt;
        BigInt mid_mask = mid_base << pos - _board_sqrt;
        BigInt edge_mask = edge_base << pos - _board_sqrt;

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;

        if (_submask == false)
        {
            for (size_t j = 0; j < _mask_size - 2; j++)
            {
                int shift = j * _board_sqrt;
                if( pos - shift < _board_sqrt)
                    break;
                f_vec.push_back(full_mask >> shift);
    
                m_vec.push_back(mid_mask >> shift);
    
                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {
            int shift = _board_sqrt;

            if (pos < _board_sqrt * 2)
                edge_mask = ((edge_mask >> 2 * shift) & full_mask) << shift;
            else if (pos >= _board_sqrt * (_board_sqrt - (_mask_size - 1)))
                edge_mask = (full_mask & edge_mask) >> shift;
            else
                edge_mask = edge_mask >> shift;
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, 'v');
            vectorized.push_back(vec);

            mask_vector s_vec;
            BigInt sub_mask = full_mask & (full_mask << shift);
            s_vec.push_back(sub_mask);
            s_vec.push_back((sub_mask >> shift));
            
            sub_vec.push_back(s_vec);
        }

        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;
    if (_submask == true)
    {
        map["submask"] = sub_vec;
        map["vectors"] = vectorized;
    }

    return map;
}

Mask::Mask::inner_map Mask::crescendo_mask()
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

    if (_submask == true)
    {
        edge_base = (((full_base << 1) << ((2 * _board_sqrt) - 1)) | (full_base << 2)) ^ (full_base << _board_sqrt + 1);
    }

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;
    variations_vector sub_vec;
    variations_vector vectorized;

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
            sub_vec.push_back(empty_vector);
            vectorized.push_back(empty_vector);
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

        if (_submask == false)
        {
            for (size_t j = 0; j < _mask_size - 2; j++)
            {
                int shift = (j * _board_sqrt) - j;
                if( (pos - shift) % _board_sqrt == _board_sqrt - 1 || pos - shift < _board_sqrt)
                    break;
                f_vec.push_back(full_mask >> shift);

                m_vec.push_back(mid_mask >> shift);

                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {
            int shift = _board_sqrt;

            if ( pos == _board_sqrt + (_mask_size - 2) || pos == _board_size - (_board_sqrt + _mask_size - 1))
                edge_mask = BigInt(0);
            if ( pos < 2 * _board_sqrt || pos % _board_sqrt == _board_sqrt - 2)
                edge_mask = ((edge_mask >> 2) & (full_mask << ((2 * shift) - 2))) >> (shift - 1);
            else if (pos >= _board_size * (_board_sqrt - (_mask_size - 1)) || pos % _board_sqrt == _mask_size - 2)
                edge_mask = (edge_mask & (full_mask << 2)) >> (shift + 1);
            else
                edge_mask = edge_mask >> (shift + 1);
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, 'c');
            vectorized.push_back(vec);

            mask_vector s_vec;
            BigInt sub_mask = full_mask & (full_mask << (shift -1));
            s_vec.push_back(sub_mask);
            s_vec.push_back((sub_mask >> (shift-1)));
            
            sub_vec.push_back(s_vec);
        }

        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;
    if (_submask == true)
    {
        map["submask"] = sub_vec;
        map["vectors"] = vectorized;
    }

    return map;
}

Mask::Mask::inner_map Mask::decrescendo_mask()
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

    if (_submask == true)
    {
        edge_base = (full_base << ((2 * _board_sqrt) + 2) | full_base) ^ (full_base << (_board_sqrt + 1));
    }

    variations_vector full_vec;
    variations_vector mid_vec;
    variations_vector edge_vec;
    variations_vector sub_vec;
    variations_vector vectorized;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        if (pos % _board_sqrt > _board_sqrt - (_mask_size - 1) || pos < _board_sqrt + 1 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -2))) - (_mask_size -1) || pos % _board_sqrt == 0)
        {
            
            BigInt zero = BigInt(0);
            mask_vector empty_vector;
            empty_vector.push_back(zero);
            full_vec.push_back(empty_vector);
            mid_vec.push_back(empty_vector);
            edge_vec.push_back(empty_vector);
            sub_vec.push_back(empty_vector);
            vectorized.push_back(empty_vector);
            continue;
        }
        BigInt full_mask = full_base << pos - ( _board_sqrt + 1);
        BigInt mid_mask = mid_base << pos - ( _board_sqrt + 1);
        BigInt edge_mask = edge_base << pos - ( _board_sqrt + 1);

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        if (_submask == false)
        {
            for (size_t j = 0; j < _mask_size - 2; j++)
            {
                int shift = (j * _board_sqrt) + j;
                if( (pos - shift) % _board_sqrt == 0 || pos - shift < _board_sqrt)
                    break;
                f_vec.push_back(full_mask >> shift);

                m_vec.push_back(mid_mask >> shift);

                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {
            int shift = _board_sqrt;

            if ( pos == 2 * _board_sqrt - (_mask_size - 1) || pos == (_board_sqrt * (_board_sqrt - (_mask_size - 1))) + 1)
                edge_mask = BigInt(0);
            if ( pos < 2 * _board_sqrt || pos % _board_sqrt == 1)
                edge_mask = (edge_mask  & (full_mask << ((2 * shift) + 2))) >> (shift + 1);
            else if (pos >= _board_size * (_board_sqrt - (_mask_size - 1)) || pos % _board_sqrt == _board_sqrt - (_mask_size - 1))
                edge_mask = (full_mask & (edge_mask << (shift + 1))) >> ((2 * shift) + 2);
            else
                edge_mask = edge_mask >> (shift + 1);
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, 'd');
            vectorized.push_back(vec);

            mask_vector s_vec;
            BigInt sub_mask = full_mask & (full_mask << (shift +1));
            s_vec.push_back(sub_mask);
            s_vec.push_back((sub_mask >> (shift+1)));
            
            sub_vec.push_back(s_vec);
        }

        full_vec.push_back(f_vec);
        mid_vec.push_back(m_vec);
        edge_vec.push_back(e_vec);
    }
    map["full"] = full_vec;
    map["middle"] = mid_vec;
    map["edge"] = edge_vec;
    if (_submask == true)
    {
        map["submask"] = sub_vec;
        map["vectors"] = vectorized;
    }

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