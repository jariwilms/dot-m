export module dotm.processor;

import std;
import dotm.types;
import dotm.bitwise;

export namespace dotm
{
    template<bit::endian_e endian_v> struct cpsr;
    template<> struct cpsr<bit::endian_e::little>
    {
        constexpr cpsr(dotm::uint8_t value = {})
            : _{ value } {}

        union
        {
            struct
            {
                dotm::uint8_t x : 4u;
                dotm::bool_t  c : 1u;
                dotm::bool_t  h : 1u;
                dotm::bool_t  n : 1u;
                dotm::bool_t  z : 1u;
            };
            dotm::uint8_t _;
        };

        operator dotm::uint8_t() const
        {
            return _;
        }
        auto operator=(dotm::uint8_t value) -> cpsr&
        {
            _ = value;
            return *this;
        }
    };
    template<> struct cpsr<bit::endian_e::big   >
    {
        cpsr(dotm::uint8_t value = {})
            : _{ value } {}

        union
        {
            struct
            {
                dotm::bool_t  z : 1u;
                dotm::bool_t  n : 1u;
                dotm::bool_t  h : 1u;
                dotm::bool_t  c : 1u;
                dotm::uint8_t x : 4u;
            };
            dotm::uint8_t _;
        };

        operator dotm::uint8_t() const
        {
            return _;
        }
        auto operator=(dotm::uint8_t value) -> cpsr&
        {
            _ = value;
            return *this;
        }
    };

    namespace _
    {
        template<std::endian endian_v>
        struct generic_register;
        template<> struct generic_register<std::endian::little>
        {
            generic_register()
                : value{} {}

            union
            {
                struct
                {
                    dotm::uint8_t low;
                    dotm::uint8_t high;
                };
                dotm::uint16_t value;
            };
        };
        template<> struct generic_register<std::endian::big   >
        {
            generic_register()
                : value{} {}

            union
            {
                struct
                {
                    dotm::uint8_t high;
                    dotm::uint8_t low;
                };
                dotm::uint16_t value;
            };
        };
    }
}