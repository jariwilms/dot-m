export module dotm.bitwise;

import std;
import dotm.types;

export namespace dotm::bit
{
    enum class direction_e
    {
        left , 
        right, 
    };
    enum class endian_e
    {
        little = std::endian::little, 
        big    = std::endian::big   , 
        native = std::endian::native, 
    };
    enum class operation_e
    {
        add     , 
        subtract, 
        multiply, 
        divide  , 
    };
    enum class shift_e
    {
        arithmetic, 
        logical   , 
    };
    
    auto constexpr byte_width = 8u;
    
    template<std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr set              (value_t value, index_t index, dotm::bool_t state) -> value_t
    {
        return static_cast<value_t>((value & ~(1u << index)) | state << index);
    }
    template<std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr reset            (value_t value, index_t index) -> value_t
    {
        return static_cast<value_t>(value & ~(1u << index));
    }
    template<std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr test             (value_t value, index_t index) -> dotm::bool_t
    {
        return (value >> index) & 1u;
    }
    template<std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr toggle           (value_t value, index_t index) -> value_t
    {
        return static_cast<value_t>(value ^ (1u << index));
    }
    
    template<std::unsigned_integral value_t>
    auto constexpr pop_count        (value_t value) -> value_t
    {
        return std::popcount(value);
    }
    template<bit::direction_e direction_v, std::unsigned_integral value_t>
    auto constexpr count_zero       (value_t value) -> value_t
    {
             if constexpr (direction_v == bit::direction_e::left ) return std::countl_zero(value);
        else if constexpr (direction_v == bit::direction_e::right) return std::countr_zero(value);
        else static_assert(dotm::false_, "invalid direction");
    }
    template<bit::direction_e direction_v, std::unsigned_integral value_t>
    auto constexpr count_one        (value_t value) -> value_t
    {
             if constexpr (direction_v == bit::direction_e::left ) return std::countl_one(value);
        else if constexpr (direction_v == bit::direction_e::right) return std::countr_one(value);
        else static_assert(dotm::false_, "invalid direction");
    }
    template<std::unsigned_integral value_t>
    auto constexpr is_power_of_2    (value_t value) -> dotm::bool_t
    {
        return std::has_single_bit(value);
    }
    
    template<std::unsigned_integral value_t>
    auto constexpr length           () -> value_t
    {
        return static_cast<value_t>(sizeof(value_t) * bit::byte_width);
    }
    template<std::unsigned_integral value_t>
    auto constexpr width            (value_t value = {}) -> value_t
    {
        return static_cast<value_t>(bit::length<value_t>() - bit::count_zero<bit::direction_e::left>(value));
    }
    
    template<std::unsigned_integral value_t>
    auto constexpr swap             (value_t value) -> value_t
    {
        return std::byteswap(value);
    }
    template<bit::shift_e shift_v, bit::direction_e direction_v, std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr shift            (value_t value, index_t index) -> value_t
    {
        using signed_t = std::make_signed_t<value_t>;

             if constexpr (direction_v == bit::direction_e::left ) return static_cast<value_t>(value << index);
        else if constexpr (direction_v == bit::direction_e::right)
        {
                 if constexpr (shift_v == bit::shift_e::arithmetic) return static_cast<value_t>(static_cast<signed_t>(value) >> index);
            else if constexpr (shift_v == bit::shift_e::logical   ) return static_cast<value_t>(                      value  >> index);
            else static_assert(dotm::false_, "invalid shift");
        }
        else static_assert(dotm::false_, "invalid direction");
    }
    template<bit::direction_e direction_v, std::unsigned_integral value_t, std::unsigned_integral index_t>
    auto constexpr rotate           (value_t value, index_t rotation) -> value_t
    {
             if constexpr (direction_v == bit::direction_e::left ) return std::rotl(value, rotation);
        else if constexpr (direction_v == bit::direction_e::right) return std::rotr(value, rotation);
        else static_assert(dotm::false_, "invalid direction");
    }
    template<bit::direction_e direction_v, std::unsigned_integral value_t>
    auto constexpr rotate_carry     (value_t value, dotm::flag_t carry) -> std::tuple<value_t, dotm::flag_t>
    {
        auto const index = static_cast<value_t>(bit::length<value_t>() - 1u);

             if constexpr (direction_v == bit::direction_e::left ) return std::make_tuple(bit::set(bit::shift<bit::shift_e::logical, direction_v>(value, 1u), 0u   , carry), bit::test(value, index));
        else if constexpr (direction_v == bit::direction_e::right) return std::make_tuple(bit::set(bit::shift<bit::shift_e::logical, direction_v>(value, 1u), index, carry), bit::test(value, 0u   ));
        else static_assert(dotm::false_, "invalid direction");
    }

    template<std::unsigned_integral value_t, std::unsigned_integral index_t, std::unsigned_integral count_t>
    auto constexpr insert           (value_t value, value_t insert_value, index_t index, count_t count = bit::length<value_t>()) -> value_t
    {
        auto const mask = static_cast<value_t>((1u << count) - 1u);
        return static_cast<value_t>((value & ~(mask << index)) | (insert_value & mask) << index);
    }
    template<std::unsigned_integral value_t, std::unsigned_integral index_t, std::unsigned_integral count_t>
    auto constexpr extract          (value_t value,                       index_t index, count_t count = bit::length<value_t>()) -> value_t
    {
        auto const mask = static_cast<value_t>((1u << count) - 1u);
        return static_cast<value_t>((value >> index) & mask);
    }

    template<bit::endian_e endian_v, std::unsigned_integral to_t, std::unsigned_integral from_t> requires (sizeof(from_t) < sizeof(to_t))
    auto constexpr pack             (from_t alpha, from_t beta) -> to_t
    {
             if constexpr (endian_v == bit::endian_e::little) return (static_cast<to_t>(beta ) << bit::length<from_t>()) | alpha;
        else if constexpr (endian_v == bit::endian_e::big   ) return (static_cast<to_t>(alpha) << bit::length<from_t>()) | beta ;
        else static_assert(dotm::false_, "invalid endian");
    }
    template<bit::endian_e endian_v, std::unsigned_integral to_t, std::unsigned_integral from_t> requires (sizeof(from_t) > sizeof(to_t))
    auto constexpr unpack           (from_t value) -> std::tuple<to_t, to_t>
    {
        auto const alpha = static_cast<to_t>((value                       ) & std::numeric_limits<to_t>::max());
        auto const beta  = static_cast<to_t>((value >> bit::length<to_t>()) & std::numeric_limits<to_t>::max());

             if constexpr (endian_v == bit::endian_e::little) return std::make_tuple(alpha, beta );
        else if constexpr (endian_v == bit::endian_e::big   ) return std::make_tuple(beta , alpha);
        else static_assert(dotm::false_, "invalid endian");
    }
    
    template<bit::endian_e endian_v, dotm::size_t group_size_v, std::unsigned_integral value_t, std::unsigned_integral element_t = value_t> requires (group_size_v <= bit::length<value_t>())
    auto constexpr expand           (value_t value) -> std::array<element_t, (bit::length<value_t>() + group_size_v - 1u) / group_size_v>
    {
        auto constexpr value_count = (bit::length<value_t>() + group_size_v - 1u) / group_size_v;
        auto constexpr mask        = (group_size_v == bit::length<value_t>()) ? ~value_t{ 0 } : (value_t{ 1 } << group_size_v) - 1u;
        auto           values      = std::array<element_t, value_count>{};

        for (auto index = dotm::size_t{ 0u }; index < value_count; ++index)
        {
                 if constexpr (endian_v == bit::endian_e::little) values[                    index ] = (value >> (index * group_size_v)) & mask;
            else if constexpr (endian_v == bit::endian_e::big   ) values[(value_count - 1u - index)] = (value >> (index * group_size_v)) & mask;
            else static_assert(dotm::false_, "invalid endian");
        }

        return values;
    }
    template<bit::endian_e endian_v, dotm::size_t group_size_v, std::unsigned_integral value_t, std::unsigned_integral element_t, std::size_t count_v> requires (group_size_v * count_v <= bit::length<value_t>())
    auto constexpr compress         (std::array<element_t, count_v> const& values) -> value_t
    {
        auto constexpr mask   = (group_size_v == bit::length<element_t>()) ? ~element_t{ 0u } : (element_t{ 1u } << group_size_v) - 1u;
        auto           result = value_t{ 0u };

        for (auto index = dotm::size_t{ 0u }; index < count_v; ++index)
        {
            auto const chunk = static_cast<value_t>(values[index] & mask);

                 if constexpr (endian_v == bit::endian_e::little) result |= static_cast<value_t>(chunk << (                index  * group_size_v));
            else if constexpr (endian_v == bit::endian_e::big   ) result |= static_cast<value_t>(chunk << ((count_v - 1u - index) * group_size_v));
            else static_assert(dotm::false_, "invalid endian");
        }

        return result;
    }
    
    template<bit::operation_e operation_v, std::unsigned_integral value_t>
    auto constexpr test_carry       (value_t alpha, value_t beta, dotm::flag_t carry = dotm::false_) -> dotm::bool_t
    {
             if constexpr (operation_v == bit::operation_e::add     ) return alpha + beta + carry > std::numeric_limits<value_t>::max();
        else if constexpr (operation_v == bit::operation_e::subtract) return alpha - beta - carry < std::numeric_limits<value_t>::min();
        else static_assert(dotm::false_, "invalid operation");
    }
    template<bit::operation_e operation_v, std::unsigned_integral value_t>
    auto constexpr test_half_carry  (value_t alpha, value_t beta, dotm::flag_t carry = dotm::false_) -> dotm::bool_t
    {
        auto constexpr mask = static_cast<value_t>(std::numeric_limits<value_t>::max() >> 4u);

             if constexpr (operation_v == bit::operation_e::add     ) return ((alpha & mask) + (beta & mask) + carry) & (mask + 1u);
        else if constexpr (operation_v == bit::operation_e::subtract) return ((alpha & mask) - (beta & mask) - carry) & (mask + 1u);
        else static_assert(dotm::false_, "invalid operation");
    }
}
