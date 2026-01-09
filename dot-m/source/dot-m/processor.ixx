export module dotm.processor;

import std;
import dotm.types;
import dotm.bitwise;

export namespace dotm
{
    template<bit::endian_e endian_v> struct cpsr;
    template<> struct cpsr<bit::endian_e::little>
    {
        cpsr(dotm::uint8_t value = {})
            : value{ value } {}
        cpsr(dotm::bool_t zero, dotm::bool_t negative, dotm::bool_t half_carry, dotm::bool_t carry)
            : zero{ zero }, negative{ negative }, half_carry{ half_carry }, carry{ carry } {}

        union
        {
            struct
            {
                dotm::uint8_t _          : 4u;
                dotm::bool_t  carry      : 1u;
                dotm::bool_t  half_carry : 1u;
                dotm::bool_t  negative   : 1u;
                dotm::bool_t  zero       : 1u;
            };
            dotm::uint8_t value;
        };
    };
    template<> struct cpsr<bit::endian_e::big   >
    {
        cpsr(dotm::uint8_t value = {})
            : value{ value } {}
        cpsr(dotm::bool_t zero, dotm::bool_t negative, dotm::bool_t half_carry, dotm::bool_t carry)
            : zero{ zero }, negative{ negative }, half_carry{ half_carry }, carry{ carry } {}

        union
        {
            struct
            {
                dotm::bool_t  zero       : 1u;
                dotm::bool_t  negative   : 1u;
                dotm::bool_t  half_carry : 1u;
                dotm::bool_t  carry      : 1u;
                dotm::uint8_t _          : 4u;
            };
            dotm::uint8_t value;
        };
    };
}