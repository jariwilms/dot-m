export module dotm.dmg.processor.cpsr;

import std;
import dotm.types;
import dotm.bitwise;

export namespace dtm::dmg
{
    template<bit::endian_e endian_v> struct cpsr;
    template<> struct cpsr<bit::endian_e::little>
    {
        cpsr(dtm::uint8_t value = {})
            : value{ value } {}
        cpsr(dtm::bool_t zero, dtm::bool_t negative, dtm::bool_t half_carry, dtm::bool_t carry)
            : zero{ zero }, negative{ negative }, half_carry{ half_carry }, carry{ carry } {}

        union
        {
            struct
            {
                dtm::uint8_t _          : 4u;
                dtm::bool_t  carry      : 1u;
                dtm::bool_t  half_carry : 1u;
                dtm::bool_t  negative   : 1u;
                dtm::bool_t  zero       : 1u;
            };
            dtm::uint8_t value;
        };
    };
    template<> struct cpsr<bit::endian_e::big   >
    {
        cpsr(dtm::uint8_t value = {})
            : value{ value } {}
        cpsr(dtm::bool_t zero, dtm::bool_t negative, dtm::bool_t half_carry, dtm::bool_t carry)
            : zero{ zero }, negative{ negative }, half_carry{ half_carry }, carry{ carry } {}

        union
        {
            struct
            {
                dtm::bool_t  zero       : 1u;
                dtm::bool_t  negative   : 1u;
                dtm::bool_t  half_carry : 1u;
                dtm::bool_t  carry      : 1u;
                dtm::uint8_t _          : 4u;
            };
            dtm::uint8_t value;
        };
    };
}