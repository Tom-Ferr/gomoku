#include <Mask.hpp>

Mask::mask_vector Mask::_targets = mask_vector();

Mask::Mask(){}

Mask::Mask(const int mask_size, const unsigned int board_sqrt, bool submask, bool mirror)
:
    _mask_size(mask_size),
    _board_sqrt(board_sqrt),
    _board_size(board_sqrt * board_sqrt),
    _submask(submask),
    _mirror(mirror)
{
    if (_targets.empty())
        _targets = build_targets();
		_masks.resize(5);
        _masks[HORIZONTAL] = horizontal_mask();
        _masks[VERTICAL] = vertical_mask();
        _masks[CRESCENDO] = crescendo_mask();
        _masks[DECRESCENDO] = decrescendo_mask();
        if(submask == false)
		{
            _masks[SUPERPOSITION] = create_superpositions();
		}
}

Mask::Mask(const Mask& other)
{
	_mask_size = other._mask_size;
	_board_sqrt = other._board_sqrt;
	_board_size = other._board_size;
	_submask = other._submask;
	_mirror = other._mirror;
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
	    _mirror = other._mirror;
        _masks = other._masks;
    }
	return *this;
}

Mask::~Mask(){}

Mask::mask_vector Mask::build_targets()
{
    mask_vector vec;
    BigInt n = BigInt(1);
    for (size_t i = 0; i < _board_size; i++)
    {
        BigInt s = (n << i);
        vec.push_back(s);
    }
    return vec;
}

BigInt &Mask::targets(size_t pos)
{
	return Mask::_targets[pos];
}

void Mask::vectorize(Mask::mask_vector &dest, int src, char mode) const
{
    int inc;
    int shift;
    int pos;
    switch (mode)
    {
    case HORIZONTAL:
        inc = 0;
        shift = 1;
        break;
    case VERTICAL:
        inc = 0;
        shift = _board_sqrt;
        break;
    case CRESCENDO:
        inc = -1;
        shift = _board_sqrt;
        break;
    case DECRESCENDO:
        inc = 1;
        shift = _board_sqrt;
        break;
    default:
        return ;
    }

    pos = src - shift - inc;
    bool s = (src + shift + inc) % _board_sqrt > src % _board_sqrt;
    bool f = pos % _board_sqrt >= src % _board_sqrt;
    bool l = (pos + ((_mask_size + 1) * shift) + ((_mask_size + 1) * inc)) % _board_sqrt > src % _board_sqrt;
    for (size_t i = 0; i < _mask_size + 2; i++)
    {
        BigInt t;
        size_t t_pos = pos;
        if(s == f && i == 0)
            t_pos = (pos + shift + inc);
        else if(s != l && i == _mask_size + 1)
            t_pos = (pos - shift - inc);
        else if (pos < 0 )
            t_pos = (pos + shift + inc);
        else if (pos >= (int)_board_size)
            t_pos = (pos - shift - inc);

        t = targets(t_pos);
        t.set_maskpos(t_pos);

        dest.push_back(t);
        pos = pos + shift + inc;
    }
    return ;
}

Mask::inner_map Mask::create_superpositions()
{
    inner_map super_positions;
	super_positions.resize(4);
    variations_vector full_vars;
    variations_vector mid_vars;
    variations_vector other_vars;
    variations_vector edge_vars;
    std::vector<char> ot;
    ot.assign(_modes, _modes+4);
    for (size_t pos = 0; pos < _board_size; pos++)
    {
        mask_vector full_masks;
        mask_vector mid_masks;
        mask_vector other_masks;
        mask_vector edge_masks;
        std::vector<char>::iterator it = ot.begin();
        for (; it+1 != ot.end(); it++)
        {
            mask_vector::iterator main_full = _masks[*it][FULL][pos].begin();
            mask_vector::iterator main_mid = _masks[*it][MIDDLE][pos].begin();
            mask_vector::iterator main_edge = _masks[*it][EDGE][pos].begin();
            for (; main_full != _masks[*it][FULL][pos].end(); main_full++, main_mid++, main_edge++)
            {
                if(*main_full == 0)
                    break;
                mask_vector::iterator others_full = _masks[*(it+1)][FULL][pos].begin();
                mask_vector::iterator others_mid = _masks[*(it+1)][MIDDLE][pos].begin();
                mask_vector::iterator others_edge = _masks[*(it+1)][EDGE][pos].begin();
                for (; others_full != _masks[*(it+1)][FULL][pos].end(); others_full++, others_mid++, others_edge++)
                {
                    if(*others_full == 0)
                        break;
                    BigInt f = *main_full | *others_full;
                    full_masks.push_back(f);

                    BigInt m = *main_mid ^ targets(pos);
                    mid_masks.push_back(m);

                    BigInt o = *others_mid ^ targets(pos);
                    other_masks.push_back(o);

                    BigInt e = *main_edge | *others_edge;
                    edge_masks.push_back(e);
                }
            }
        }
        full_vars.push_back(full_masks);
        mid_vars.push_back(mid_masks);
        other_vars.push_back(other_masks);
        edge_vars.push_back(edge_masks);
    }
    super_positions[FULL] = full_vars;
    super_positions[MIDDLE] = mid_vars;
    super_positions[OUTER] = other_vars;
    super_positions[EDGE] = edge_vars;
    return super_positions;
}

void Mask::print_mask(Mask::variations_vector &m) const
{
    BigInt last = BigInt(1);
    last = last << _board_size - 1;
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
                if ((t & current) != 0)
                    std::cout << " X ";
                else
                    std::cout << " . ";
            }
            std::cout << std::endl;
            version++;
        }

    }

}

Mask::Mask::inner_map Mask::horizontal_mask() const
{
    Mask::inner_map map(7);
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
    variations_vector mirror;

    for (size_t pos = 0; pos < _board_size; pos++)
    {
        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;

        if (_submask == false && _mirror == false)
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

            for (size_t j = 0; j < _mask_size - 2; j++)
            {
                if( (pos % _board_sqrt) - j <= 0)
                    break;
                f_vec.push_back(full_mask >> j);

                m_vec.push_back(mid_mask >> j);

                e_vec.push_back(edge_mask >> j);
            }
        }
        else if (_mirror == true)
        {
            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;

            for (size_t j = 0; j <= _mask_size - 1; j += _mask_size - 1)
            {
                if (pos % _board_sqrt >= _board_sqrt - (_mask_size -1))
                {
                    BigInt zero = BigInt(0);
                    f_vec.push_back(zero);
                    m_vec.push_back(zero);
                    e_vec.push_back(zero);
                    j += _mask_size - 1;
                }
                if( (int)((pos % _board_sqrt) - j) < 0)
                    break;
                f_vec.push_back(full_mask >> j);

                m_vec.push_back(mid_mask >> j);

                e_vec.push_back(edge_mask >> j);
            }
        }
        else
        {
            if (pos % _board_sqrt >= _board_sqrt - (_mask_size -1))
            {
                BigInt zero = BigInt(0);
                mask_vector empty_vector;
                empty_vector.push_back(zero);
                full_vec.push_back(empty_vector);
                mid_vec.push_back(empty_vector);
                edge_vec.push_back(empty_vector);
                sub_vec.push_back(empty_vector);
                vectorized.push_back(empty_vector);
                mirror.push_back(empty_vector);
                continue;
            }
            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;

            if (pos % _board_sqrt == 0)
                edge_mask = full_mask ^ (full_mask << 1);
            else if (pos % _board_sqrt >= _board_sqrt - (_mask_size))
                edge_mask = full_mask ^ (full_mask >> 1);
            else
                edge_mask = edge_mask >> 1;
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask >> 1);
            m_vec.push_back(mid_mask);
            m_vec.push_back(mid_mask << 1);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, HORIZONTAL);
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
    map[FULL] = full_vec;
    map[MIDDLE] = mid_vec;
    map[EDGE] = edge_vec;
    if (_submask == true)
    {
        map[SUBMASK] = sub_vec;
        map[VECTOR] = vectorized;
    }

    return map;
}

Mask::inner_map Mask::vertical_mask() const
{
    Mask::inner_map map(6);
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

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;

        if (_submask == false && _mirror == false)
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
        else if (_mirror == true)
        {

            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;

            for (size_t j = 0; j <= _mask_size - 1; j += _mask_size - 1)
            {
                if (pos >= _board_sqrt * (_board_sqrt - (_mask_size - 1)))
                {
                    BigInt zero = BigInt(0);
                    f_vec.push_back(zero);
                    m_vec.push_back(zero);
                    e_vec.push_back(zero);
                    j += _mask_size - 1;
                }
                int shift = j * _board_sqrt;
                if( (int)(pos - shift) < 0)
                    break;
                f_vec.push_back(full_mask >> shift);

                m_vec.push_back(mid_mask >> shift);

                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {

            if (pos >= _board_sqrt * (_board_sqrt - (_mask_size - 1)))
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

            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;

            int shift = _board_sqrt;

            if (pos < _board_sqrt)
                edge_mask = full_mask ^ (full_mask << shift);
            else if (pos >= _board_sqrt * (_board_sqrt - (_mask_size)))
                edge_mask = full_mask ^ (full_mask >> shift);
            else
                edge_mask = edge_mask >> shift;
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask >> shift);
            m_vec.push_back(mid_mask);
            m_vec.push_back(mid_mask << shift);

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, VERTICAL);
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
    map[FULL] = full_vec;
    map[MIDDLE] = mid_vec;
    map[EDGE] = edge_vec;
    if (_submask == true)
    {
        map[SUBMASK] = sub_vec;
        map[VECTOR] = vectorized;
    }

    return map;
}

Mask::Mask::inner_map Mask::crescendo_mask() const
{
    Mask::inner_map map(6);
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

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;

        if (_submask == false && _mirror == false)
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
        else if (_mirror == true)
        {
            if(pos % _board_sqrt == _mask_size - 2)
            {
                base = base << _board_sqrt;
            }
            BigInt full_mask = full_base << pos - (_mask_size - 1);
            BigInt mid_mask = mid_base << pos - (_mask_size - 1);
            BigInt edge_mask = edge_base << pos - (_mask_size - 1);
            for (size_t j = 0; j <= _mask_size - 1; j += _mask_size - 1)
            {
                if (  pos % _board_sqrt < _mask_size - 1 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -1))))
                {
                    BigInt zero = BigInt(0);
                    f_vec.push_back(zero);
                    m_vec.push_back(zero);
                    e_vec.push_back(zero);
                    j += _mask_size - 1;
                }
                int shift = (j * _board_sqrt) - j;
                if( (pos - shift) % _board_sqrt < _mask_size - 1 || (int)(pos - shift) < 0)
                    break;
                f_vec.push_back(full_mask >> shift);

                m_vec.push_back(mid_mask >> shift);

                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {
            if ( pos % _board_sqrt < _mask_size - 1 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -1))))
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
            if(pos % _board_sqrt == _mask_size - 1)
            {
                base = base << _board_sqrt;
            }
            BigInt full_mask = full_base << pos - (_mask_size - 1);
            BigInt mid_mask = mid_base << pos - (_mask_size - 1);
            BigInt edge_mask = edge_base << pos - (_mask_size - 1);
            int shift = _board_sqrt;

            if ( pos == (_mask_size - 1) || pos == _board_size - (_board_sqrt * 4 + _mask_size - 4))
                edge_mask = full_mask ^ mid_mask;
            else if ( pos < _board_sqrt || pos % _board_sqrt == _board_sqrt - 1)
                edge_mask = full_mask ^ (full_mask << (shift - 1));
            else if (pos >= _board_size * (_board_sqrt - (_mask_size)) || pos % _board_sqrt == _mask_size - 1 || pos >= _board_size - (_board_sqrt * 5) + _mask_size - 1)
                edge_mask = full_mask ^ (full_mask >> (shift - 1));
            else
                edge_mask = edge_mask >> (shift + 1);
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask >> (shift - 1));
            m_vec.push_back(mid_mask);
            m_vec.push_back(mid_mask << (shift - 1));

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, CRESCENDO);
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
    map[FULL] = full_vec;
    map[MIDDLE] = mid_vec;
    map[EDGE] = edge_vec;
    if (_submask == true)
    {
        map[SUBMASK] = sub_vec;
        map[VECTOR] = vectorized;
    }

    return map;
}

Mask::Mask::inner_map Mask::decrescendo_mask() const
{
    Mask::inner_map map(6);
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

        mask_vector f_vec;
        mask_vector m_vec;
        mask_vector e_vec;
        
        if (_submask == false && _mirror == false)
        {
            if (pos % _board_sqrt > _board_sqrt - (_mask_size - 1) || pos < _board_sqrt + 1 || pos > (_board_sqrt * (_board_sqrt - (_mask_size -2))) - (_mask_size -1) || pos % _board_sqrt == 0)
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
        else if (_mirror == true)
        {
            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;
            for (size_t j = 0; j <= _mask_size - 1; j += _mask_size - 1)
            {
                
                if (pos % _board_sqrt > _board_sqrt - (_mask_size) || pos > (_board_sqrt * (_board_sqrt - (_mask_size -1))) - (_mask_size -1))
                {
                    BigInt zero = BigInt(0);
                    f_vec.push_back(zero);
                    m_vec.push_back(zero);
                    e_vec.push_back(zero);
                    j += _mask_size - 1;
                }
                int shift = (j * _board_sqrt) + j;
                if( (pos - shift) % _board_sqrt >= _board_sqrt - ( _mask_size - 1) || (int)(pos - shift) < 0)
                    break;
                f_vec.push_back(full_mask >> shift);

                m_vec.push_back(mid_mask >> shift);

                e_vec.push_back(edge_mask >> shift);
            }
        }

        else
        {
            if (pos % _board_sqrt > _board_sqrt - (_mask_size) || pos > (_board_sqrt * (_board_sqrt - (_mask_size -1))) - (_mask_size -1))
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
            BigInt full_mask = full_base << pos;
            BigInt mid_mask = mid_base << pos;
            BigInt edge_mask = edge_base << pos;
            int shift = _board_sqrt;

            if ( pos == _board_sqrt - (_mask_size) || pos == (_board_sqrt * (_board_sqrt - (_mask_size))))
                edge_mask = full_mask ^ mid_mask;
            else if ( pos < _board_sqrt || pos % _board_sqrt == 0)
                edge_mask = full_mask ^ (full_mask << (shift + 1));
            else if (pos >= _board_size * (_board_sqrt - (_mask_size)) || pos % _board_sqrt == _board_sqrt - (_mask_size) || pos > (_board_sqrt * (_board_sqrt - (_mask_size))))
                edge_mask = full_mask ^ (full_mask >> (shift + 1));
            else
                edge_mask = edge_mask >> (shift + 1);
            f_vec.push_back(full_mask );

            m_vec.push_back(mid_mask >> (shift + 1));
            m_vec.push_back(mid_mask);
            m_vec.push_back(mid_mask << (shift + 1));

            e_vec.push_back(edge_mask);

            mask_vector vec;
            vectorize(vec, pos, DECRESCENDO);
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
    map[FULL] = full_vec;
    map[MIDDLE] = mid_vec;
    map[EDGE] = edge_vec;
    if (_submask == true)
    {
        map[SUBMASK] = sub_vec;
        map[VECTOR] = vectorized;
    }

    return map;
}
Mask::inner_map Mask::operator[](char c)
{
    return _masks[c];
}

Mask::inner_map &Mask::at(char c)
{
    return _masks[c];
}
