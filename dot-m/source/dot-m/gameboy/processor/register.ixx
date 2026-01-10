export module dotm.dmg.processor.register_;

import std;
import dotm.types;
import dotm.dmg.processor.cpsr;
import dotm.bitwise;

export namespace dtm::dmg
{
    template<bit::endian_e endian_v>
    struct register_;
    template<>
    struct register_<bit::endian_e::little>
    {
        register_()
            : af{}, bc{}, de{}, hl{}, sp{}, pc{} {}

        union
        {
            struct
            {
                dmg::cpsr<bit::endian_e::little> f;
                dtm::uint8_t                     a;
            };
            dtm::uint16_t af;
        };
        union
        {
            struct
            {
                dtm::uint8_t c;
                dtm::uint8_t b;
            };
            dtm::uint16_t bc;
        };
        union
        {
            struct
            {
                dtm::uint8_t e;
                dtm::uint8_t d;
            };
            dtm::uint16_t de;
        };
        union
        {
            struct
            {
                dtm::uint8_t l;
                dtm::uint8_t h;
            };
            dtm::uint16_t hl;
        };

        dtm::uint16_t sp;
        dtm::uint16_t pc;

        template<std::unsigned_integral index_t>
        auto operator[](index_t index) -> index_t&
        {
                 if constexpr (std::is_same_v<index_t, dtm::uint8_t >)
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
            else if constexpr (std::is_same_v<index_t, dtm::uint16_t>)
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
            else static_assert(dtm::false_, "invalid index type");
        }
    };
    template<>
    struct register_<bit::endian_e::big   >
    {
        register_()
            : af{}, bc{}, de{}, hl{}, sp{}, pc{} {}

        union
        {
            struct
            {
                dtm::uint8_t                  a;
                dmg::cpsr<bit::endian_e::big> f;
            };
            dtm::uint16_t af;
        };
        union
        {
            struct
            {
                dtm::uint8_t b;
                dtm::uint8_t c;
            };
            dtm::uint16_t bc;
        };
        union
        {
            struct
            {
                dtm::uint8_t d;
                dtm::uint8_t e;
            };
            dtm::uint16_t de;
        };
        union
        {
            struct
            {
                dtm::uint8_t h;
                dtm::uint8_t l;
            };
            dtm::uint16_t hl;
        };

        dtm::uint16_t sp;
        dtm::uint16_t pc;

        template<std::unsigned_integral index_t>
        auto operator[](index_t index) -> index_t&
        {
                 if constexpr (std::is_same_v<index_t, dtm::uint8_t >)
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
            else if constexpr (std::is_same_v<index_t, dtm::uint16_t>)
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
            else static_assert(dtm::false_, "invalid index type");
        }
    };
}
