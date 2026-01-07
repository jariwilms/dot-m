export module dotm.sm83_register;

import std;
import dotm.types;
import dotm.processor;
import dotm.bitwise;

export namespace dotm
{
    template<bit::endian_e endian_v>
    struct sm83_register;
    template<>
    struct sm83_register<bit::endian_e::little>
    {
        sm83_register()
            : af{}, bc{}, de{}, hl{}, sp{}, pc{} {}

        union
        {
            struct
            {
                dotm::cpsr<bit::endian_e::little> f;
                dotm::uint8_t                     a;
            };
            dotm::uint16_t af;
        };
        union
        {
            struct
            {
                dotm::uint8_t c;
                dotm::uint8_t b;
            };
            dotm::uint16_t bc;
        };
        union
        {
            struct
            {
                dotm::uint8_t e;
                dotm::uint8_t d;
            };
            dotm::uint16_t de;
        };
        union
        {
            struct
            {
                dotm::uint8_t l;
                dotm::uint8_t h;
            };
            dotm::uint16_t hl;
        };

        dotm::uint16_t sp;
        dotm::uint16_t pc;

        template<std::unsigned_integral index_t>
        auto operator[](index_t index) -> index_t&
        {
                 if constexpr (std::is_same_v<index_t, dotm::uint8_t >)
            {
                switch (index)
                {
                    case 0u: return b;
                    case 1u: return c;
                    case 2u: return d;
                    case 3u: return e;
                    case 4u: return h;
                    case 5u: return l;
                    case 7u: return a;

                    default: std::unreachable();
                }
            }
            else if constexpr (std::is_same_v<index_t, dotm::uint16_t>)
            {
                switch (index)
                {
                    case 0u: return bc;
                    case 1u: return de;
                    case 2u: return hl;
                    case 3u: return af;

                    default: std::unreachable();
                }
            }
            else static_assert(dotm::false_, "invalid index type");
        }
    };
    template<>
    struct sm83_register<bit::endian_e::big   >
    {
        sm83_register()
            : af{}, bc{}, de{}, hl{}, sp{}, pc{} {}

        union
        {
            struct
            {
                dotm::uint8_t                  a;
                dotm::cpsr<bit::endian_e::big> f;
            };
            dotm::uint16_t af;
        };
        union
        {
            struct
            {
                dotm::uint8_t b;
                dotm::uint8_t c;
            };
            dotm::uint16_t bc;
        };
        union
        {
            struct
            {
                dotm::uint8_t d;
                dotm::uint8_t e;
            };
            dotm::uint16_t de;
        };
        union
        {
            struct
            {
                dotm::uint8_t h;
                dotm::uint8_t l;
            };
            dotm::uint16_t hl;
        };

        dotm::uint16_t sp;
        dotm::uint16_t pc;

        template<std::unsigned_integral index_t>
        auto operator[](index_t index) -> index_t&
        {
                 if constexpr (std::is_same_v<index_t, dotm::uint8_t >)
            {
                switch (index)
                {
                    case 0u: return b;
                    case 1u: return c;
                    case 2u: return d;
                    case 3u: return e;
                    case 4u: return h;
                    case 5u: return l;
                    case 7u: return a;

                    default: std::unreachable();
                }
            }
            else if constexpr (std::is_same_v<index_t, dotm::uint16_t>)
            {
                switch (index)
                {
                    case 0u: return bc;
                    case 1u: return bc;
                    case 2u: return de;
                    case 3u: return de;
                    case 4u: return hl;
                    case 5u: return hl;
                    case 6u: return sp;
                    case 7u: return sp;

                    default: std::unreachable();
                }
            }
            else static_assert(dotm::false_, "invalid index type");
        }
    };
}
