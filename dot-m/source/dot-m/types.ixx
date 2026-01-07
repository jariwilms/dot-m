export module dotm.types;

import std;

export namespace dotm
{
    using void_t          = void         ;
    using bool_t          = bool         ;
    using char_t          = char         ;
    enum                  : dotm::bool_t
    {
        false_ = false, 
        true_  = true , 
    };
    
    using int8_t          = std::int8_t  ;
    using int16_t         = std::int16_t ;
    using int32_t         = std::int32_t ;
    using int64_t         = std::int64_t ;
    using uint8_t         = std::uint8_t ;
    using uint16_t        = std::uint16_t;
    using uint32_t        = std::uint32_t;
    using uint64_t        = std::uint64_t;

    using byte_t          = std::uint8_t ;
    using word_t          = std::uint16_t;
    using dword_t         = std::uint32_t;
    using qword_t         = std::uint64_t;

    using size_t          = std::size_t  ;
    using index_t         = std::size_t  ;

    using instruction8_t  = dotm::uint8_t ;
    using instruction16_t = dotm::uint16_t;
    using instruction32_t = dotm::uint32_t;
}
