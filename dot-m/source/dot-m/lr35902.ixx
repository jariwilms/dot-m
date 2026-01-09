export module dotm.architecture.sm83;

import std;
import dotm.bitwise;
import dotm.processor;
import dotm.sm83_memory;
import dotm.sm83_register;
import dotm.types;

export namespace dotm
{
    auto constexpr instruction_identifiers = std::array<dotm::char_t const*, 0x100>
    {
        //       x0",          x1",          x2",          x3",          x4",          x5",          x6",          x7", 
        "nop       ", "ld_rr_nn  ", "ld_drr_a  ", "inc_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "rlca      ", //00x
        "ld_dnn_sp ", "add_hl_rr ", "ld_a_drr  ", "dec_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "rrca      ", //01x
        "stop      ", "ld_rr_nn  ", "ld_drr_a  ", "inc_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "rla       ", //02x
        "jr_e      ", "add_hl_rr ", "ld_a_drr  ", "dec_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "rra       ", //03x
        "jr_cond_e ", "ld_rr_nn  ", "ld_dhli_a ", "inc_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "daa       ", //04x
        "jr_cond_e ", "add_hl_rr ", "ld_a_dhli ", "dec_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "cpl       ", //05x
        "jr_cond_e ", "ld_rr_nn  ", "ld_dhld_a ", "inc_rr    ", "inc_dhl   ", "dec_dhl   ", "ld_dhl_n  ", "scf       ", //06x
        "jr_cond_e ", "add_hl_rr ", "ld_a_dhld ", "dec_rr    ", "inc_r     ", "dec_r     ", "ld_r_n    ", "ccf       ", //07x

        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //10x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //11x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //12x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //13x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //14x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //15x
        "ld_dhl_r  ", "ld_dhl_r  ", "ld_dhl_r  ", "ld_dhl_r  ", "ld_dhl_r  ", "ld_dhl_r  ", "halt      ", "ld_dhl_r  ", //16x
        "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_r    ", "ld_r_dhl  ", "ld_r_r    ", //17x

        "add_a_r   ", "add_a_r   ", "add_a_r   ", "add_a_r   ", "add_a_r   ", "add_a_r   ", "add_a_dhl ", "add_a_r   ", //20x
        "adc_a_r   ", "adc_a_r   ", "adc_a_r   ", "adc_a_r   ", "adc_a_r   ", "adc_a_r   ", "adc_a_dhl ", "adc_a_r   ", //21x
        "sub_a_r   ", "sub_a_r   ", "sub_a_r   ", "sub_a_r   ", "sub_a_r   ", "sub_a_r   ", "sub_a_dhl ", "sub_a_r   ", //22x
        "sbc_a_r   ", "sbc_a_r   ", "sbc_a_r   ", "sbc_a_r   ", "sbc_a_r   ", "sbc_a_r   ", "sbc_a_dhl ", "sbc_a_r   ", //23x
        "and_a_r   ", "and_a_r   ", "and_a_r   ", "and_a_r   ", "and_a_r   ", "and_a_r   ", "and_a_dhl ", "and_a_r   ", //24x
        "xor_a_r   ", "xor_a_r   ", "xor_a_r   ", "xor_a_r   ", "xor_a_r   ", "xor_a_r   ", "xor_a_dhl ", "xor_a_r   ", //25x
        "or_a_r    ", "or_a_r    ", "or_a_r    ", "or_a_r    ", "or_a_r    ", "or_a_r    ", "or_a_dhl  ", "or_a_r    ", //26x
        "cp_a_r    ", "cp_a_r    ", "cp_a_r    ", "cp_a_r    ", "cp_a_r    ", "cp_a_r    ", "cp_a_dhl  ", "cp_a_r    ", //27x

        "ret_cc    ", "pop_rr    ", "jp_cc_nn  ", "jp_nn     ", "call_cc_nn", "push_rr   ", "add_a_n   ", "rst       ", //30x
        "ret_cc    ", "ret       ", "jp_cc_nn  ", "prefix    ", "call_cc_nn", "call_nn   ", "adc_a_n   ", "rst       ", //31x
        "ret_cc    ", "pop_rr    ", "jp_cc_nn  ", "_         ", "call_cc_nn", "push_rr   ", "sub_a_n   ", "rst       ", //32x
        "ret_cc    ", "reti      ", "jp_cc_nn  ", "_         ", "call_cc_nn", "_         ", "sbc_a_n   ", "rst       ", //33x
        "ldh_dn_a  ", "pop_rr    ", "ldh_dc_a  ", "_         ", "_         ", "push_rr   ", "and_a_n   ", "rst       ", //34x
        "add_sp_e  ", "jp_hl     ", "ld_dnn_sp ", "_         ", "_         ", "_         ", "xor_a_n   ", "rst       ", //35x
        "ldh_a_dn  ", "pop_rr    ", "ldh_a_dc  ", "di        ", "_         ", "push_rr   ", "or_a_n    ", "rst       ", //36x
        "ld_hl_sp_e", "ld_sp_hl  ", "ld_a_dnn  ", "ei        ", "_         ", "_         ", "cp_a_n    ", "rst       ", //37x
    };

    class sm83
    {
    public:
        using memory_t              = dotm::sm83_memory;
        using register_t            = dotm::sm83_register<bit::endian_e::little>;
        using instruction_pointer_t = dotm::void_t(sm83::*)();

        sm83()
            : memory_{}
        {
            register_.a   = 0x01;
            register_.f._ = 0xB0;
            register_.b   = 0x00;
            register_.c   = 0x13;
            register_.d   = 0x00;
            register_.e   = 0xD8;
            register_.h   = 0x01;
            register_.l   = 0x4D;
            register_.sp  = 0xFFFE;
            register_.pc  = 0x0100;

            interrupt_master_enable_     = dotm::false_;
            interrupt_master_enable_delay = 0u;
            is_running_                  = dotm::true_;
        }

        void cycle()
        {
            if (interrupt_master_enable_delay > 0u && (--interrupt_master_enable_delay == 0u)) interrupt_master_enable_ = dotm::true_;

            auto       interrupt_flag   = memory_.read(0xFF0F);
            auto const interrupt_enable = memory_.read(0xFFFF);
            auto const interrupts       = static_cast<dotm::uint8_t>(interrupt_enable & interrupt_flag & 0x1F);
            auto const jumps            = std::array<dotm::uint16_t, 5u>{ 0x40, 0x48, 0x50, 0x58, 0x60 };

            if (interrupt_master_enable_ && interrupts)
            {
                interrupt_master_enable_ = dotm::false_;

                for (auto i = 0u; i < jumps.size(); ++i)
                {
                    if (bit::test(interrupts, i))
                    {
                        memory_.write(0xFF0F, bit::set(interrupt_flag, i, dotm::false_));

                        auto const [low_pc, high_pc] = bit::unpack<bit::endian_e::little, dotm::uint8_t>(register_.pc);
                        memory_.write(--register_.sp, high_pc);
                        memory_.write(--register_.sp, low_pc );

                        register_.pc = jumps[i];
                        break;
                    }
                }
            }

            instruction_       = fetch  ();
            auto const pointer = decode (instruction_);
                                 execute(pointer);
        }

    //private:
        auto fetch     () -> dotm::instruction8_t
        {
            return memory_.read(register_.pc++);
        }
        auto decode    (dotm::instruction8_t instruction) -> instruction_pointer_t
        {
            auto constexpr jump_table = std::array<instruction_pointer_t, 0x100>
            {
                //             _0,                _1,                _2,                _3,                _4,                _5,                _6,                _7, 
                &sm83::nop       , &sm83::ld_rr_nn  , &sm83::ld_drr_a  , &sm83::inc_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::rlca      , //000_
                &sm83::ld_dnn_sp , &sm83::add_hl_rr , &sm83::ld_a_drr  , &sm83::dec_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::rrca      , //001_
                &sm83::stop      , &sm83::ld_rr_nn  , &sm83::ld_drr_a  , &sm83::inc_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::rla       , //002_
                &sm83::jr_e      , &sm83::add_hl_rr , &sm83::ld_a_drr  , &sm83::dec_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::rra       , //003_
                &sm83::jr_cc_e   , &sm83::ld_rr_nn  , &sm83::ld_dhli_a , &sm83::inc_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::daa       , //004_
                &sm83::jr_cc_e   , &sm83::add_hl_rr , &sm83::ld_a_dhli , &sm83::dec_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::cpl       , //005_
                &sm83::jr_cc_e   , &sm83::ld_rr_nn  , &sm83::ld_dhld_a , &sm83::inc_rr    , &sm83::inc_dhl   , &sm83::dec_dhl   , &sm83::ld_dhl_n  , &sm83::scf       , //006_
                &sm83::jr_cc_e   , &sm83::add_hl_rr , &sm83::ld_a_dhld , &sm83::dec_rr    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::ccf       , //007_
                                                                                                                                                                      
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //010_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //011_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //012_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //013_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //014_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //015_
                &sm83::ld_dhl_r  , &sm83::ld_dhl_r  , &sm83::ld_dhl_r  , &sm83::ld_dhl_r  , &sm83::ld_dhl_r  , &sm83::ld_dhl_r  , &sm83::halt      , &sm83::ld_dhl_r  , //016_
                &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_r    , &sm83::ld_r_dhl  , &sm83::ld_r_r    , //017_
                                                                                                                                                                      
                &sm83::add_a_r   , &sm83::add_a_r   , &sm83::add_a_r   , &sm83::add_a_r   , &sm83::add_a_r   , &sm83::add_a_r   , &sm83::add_a_dhl , &sm83::add_a_r   , //020_
                &sm83::adc_a_r   , &sm83::adc_a_r   , &sm83::adc_a_r   , &sm83::adc_a_r   , &sm83::adc_a_r   , &sm83::adc_a_r   , &sm83::adc_a_dhl , &sm83::adc_a_r   , //021_
                &sm83::sub_a_r   , &sm83::sub_a_r   , &sm83::sub_a_r   , &sm83::sub_a_r   , &sm83::sub_a_r   , &sm83::sub_a_r   , &sm83::sub_a_dhl , &sm83::sub_a_r   , //022_
                &sm83::sbc_a_r   , &sm83::sbc_a_r   , &sm83::sbc_a_r   , &sm83::sbc_a_r   , &sm83::sbc_a_r   , &sm83::sbc_a_r   , &sm83::sbc_a_dhl , &sm83::sbc_a_r   , //023_
                &sm83::and_a_r   , &sm83::and_a_r   , &sm83::and_a_r   , &sm83::and_a_r   , &sm83::and_a_r   , &sm83::and_a_r   , &sm83::and_a_dhl , &sm83::and_a_r   , //024_
                &sm83::xor_a_r   , &sm83::xor_a_r   , &sm83::xor_a_r   , &sm83::xor_a_r   , &sm83::xor_a_r   , &sm83::xor_a_r   , &sm83::xor_a_dhl , &sm83::xor_a_r   , //025_
                &sm83::or_a_r    , &sm83::or_a_r    , &sm83::or_a_r    , &sm83::or_a_r    , &sm83::or_a_r    , &sm83::or_a_r    , &sm83::or_a_dhl  , &sm83::or_a_r    , //026_
                &sm83::cp_a_r    , &sm83::cp_a_r    , &sm83::cp_a_r    , &sm83::cp_a_r    , &sm83::cp_a_r    , &sm83::cp_a_r    , &sm83::cp_a_dhl  , &sm83::cp_a_r    , //027_
                
                &sm83::ret_cc    , &sm83::pop_rr    , &sm83::jp_cc_nn  , &sm83::jp_nn     , &sm83::call_cc_nn, &sm83::push_rr   , &sm83::add_a_n   , &sm83::rst       , //030_
                &sm83::ret_cc    , &sm83::ret       , &sm83::jp_cc_nn  , &sm83::prefix    , &sm83::call_cc_nn, &sm83::call_nn   , &sm83::adc_a_n   , &sm83::rst       , //031_
                &sm83::ret_cc    , &sm83::pop_rr    , &sm83::jp_cc_nn  , &sm83::_         , &sm83::call_cc_nn, &sm83::push_rr   , &sm83::sub_a_n   , &sm83::rst       , //032_
                &sm83::ret_cc    , &sm83::reti      , &sm83::jp_cc_nn  , &sm83::_         , &sm83::call_cc_nn, &sm83::_         , &sm83::sbc_a_n   , &sm83::rst       , //033_
                &sm83::ldh_dn_a  , &sm83::pop_rr    , &sm83::ldh_dc_a  , &sm83::_         , &sm83::_         , &sm83::push_rr   , &sm83::and_a_n   , &sm83::rst       , //034_
                &sm83::add_sp_e  , &sm83::jp_hl     , &sm83::ld_dnn_a  , &sm83::_         , &sm83::_         , &sm83::_         , &sm83::xor_a_n   , &sm83::rst       , //035_
                &sm83::ldh_a_dn  , &sm83::pop_rr    , &sm83::ldh_a_dc  , &sm83::di        , &sm83::_         , &sm83::push_rr   , &sm83::or_a_n    , &sm83::rst       , //036_
                &sm83::ld_hl_sp_e, &sm83::ld_sp_hl  , &sm83::ld_a_dnn  , &sm83::ei        , &sm83::_         , &sm83::_         , &sm83::cp_a_n    , &sm83::rst       , //037_
            };

            return jump_table[instruction];
        }
        void execute   (instruction_pointer_t pointer)
        {
            (this->*pointer)();
        }

        void nop       ()
        {

        }
        void ld_rr_nn  ()
        {
            auto const index     = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);
            auto const immediate = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            
            if (instruction_ == 0x31)
            {
                register_.sp = immediate;
            }
            else
            {
                register_[index]     = immediate;
            }
        }
        void ld_drr_a  ()
        {
            auto const index = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
            
            memory_.write(register_[index], register_.a);
        }
        void ld_a_drr  ()
        {
            auto const index = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
            
            register_.a      = memory_.read(register_[index]);
        }
        void ld_dnn_sp ()
        {
            auto const low_byte          = memory_.read(register_.pc++);
            auto const high_byte         = memory_.read(register_.pc++);
            auto const immediate         = bit::pack  <bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            auto const [low_sp, high_sp] = bit::unpack<bit::endian_e::little, dotm::uint8_t >(register_.sp);
            
            memory_.write(immediate     , low_sp );
            memory_.write(immediate + 1u, high_sp);
        }
        void inc_rr    ()
        {
            if (instruction_ == 0x33)
            {
                ++register_.sp;
            }
            else
            {
                auto const index = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
                register_[index] = register_[index] + dotm::uint16_t{ 1u };
            }
        }
        void dec_rr    ()
        {
            if (instruction_ == 0x3B)
            {
                --register_.sp;
            }
            else
            {
                auto const index = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
                register_[index] = register_[index] - dotm::uint16_t{ 1u };
            }
        }
        void add_hl_rr ()
        {
            if (instruction_ == 0x39)
            {
                auto const addend  = register_.sp;
                auto const operand = register_.hl;
            
                register_.hl       = register_.hl + addend;
                register_.f.n      = dotm::false_;
                register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, addend);
                register_.f.c      = bit::test_carry     <bit::operation_e::add>(operand, addend);
            }
            else
            {
                auto const index   = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
                auto const addend  = register_[index];
                auto const operand = register_.hl;
            
                register_.hl       = register_.hl + addend;
                register_.f.n      = dotm::false_;
                register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, addend);
                register_.f.c      = bit::test_carry     <bit::operation_e::add>(operand, addend);
            }
        }
        void inc_r     ()
        {
            auto const index   = bit::extract<dotm::uint8_t>(instruction_, 3u, 3u);
            auto const operand = register_[index];

            register_[index]   = register_[index] + dotm::uint8_t{ 1u };
            register_.f.z      = register_[index] == 0u;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, dotm::uint8_t{ 1u });
        }
        void dec_r     ()
        {
            auto const index   = bit::extract<dotm::uint8_t>(instruction_, 3u, 3u);
            auto const operand = register_[index];
            
            register_[index]   = register_[index] - dotm::uint8_t{ 1u };
            register_.f.z      = register_[index] == 0u;
            register_.f.n      = dotm::true_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::subtract>(operand, dotm::uint8_t{ 1u });
        }
        void ld_r_n    ()
        {
            auto const index     = bit::extract<dotm::uint8_t>(instruction_, 3u, 3u);
            auto const immediate = memory_.read(register_.pc++);

            register_[index]     = immediate;
        }
        void rlca      ()
        {
            auto const operand = register_.a;

            register_.a        = bit::rotate<bit::direction_e::left>(register_.a, 1u);
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = dotm::false_;
            register_.f.c      = bit::test(operand, 7u);
        }
        void rrca      ()
        {
            auto const operand = register_.a;

            register_.a        = bit::rotate<bit::direction_e::right>(register_.a, 1u);
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = dotm::false_;
            register_.f.c      = bit::test(operand, 0u);
        }
        void rla       ()
        {
            auto const operand = register_.a;

            register_.a        = bit::rotate<bit::direction_e::left>(register_.a, 1u);
            register_.a        = bit::set(register_.a, 0u, register_.f.c);
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = dotm::false_;
            register_.f.c      = bit::test(operand, 7u);
        }
        void rra       ()
        {
            auto const operand = register_.a;

            register_.a        = bit::rotate<bit::direction_e::right>(register_.a, 1u);
            register_.a        = bit::set(register_.a, 7u, register_.f.c);
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = dotm::false_;
            register_.f.c      = bit::test(operand, 0u);
        }
        void daa       ()
        {
            auto correction = dotm::uint8_t{ 0u };

            if (!register_.f.n)
            {
                if (register_.f.h || (register_.a & 0x0F) > 0x09) correction  = 0x06;
                if (register_.f.c ||  register_.a         > 0x99) correction |= 0x60;
                
                register_.a += correction;
            }
            else
            {
                if (register_.f.h) correction  = 0x06;
                if (register_.f.c) correction |= 0x60;
                
                register_.a -= correction;
            }

            register_.f.z   = register_.a == 0u;
            register_.f.h   = dotm::false_;
            register_.f.c   = correction >= 0x60;
        }
        void cpl       ()
        {
            register_.a   = ~register_.a;
            register_.f.n = dotm::true_;
            register_.f.h = dotm::true_;
        }
        void scf       ()
        {
            register_.f.n = dotm::false_;
            register_.f.h = dotm::false_;
            register_.f.c = dotm::true_;
        }
        void ccf       ()
        {
            register_.f.n = dotm::false_;
            register_.f.h = dotm::false_;
            register_.f.c = !register_.f.c;
        }
        void jr_e      ()
        {
            auto const offset = static_cast<dotm::int8_t>(memory_.read(register_.pc++));
            register_.pc      = register_.pc + offset;

            //if (offset == static_cast<dotm::int8_t>(0xFE))
            //{
            //    is_running_ = dotm::false_;
            //}
        }
        void jr_cc_e   ()
        {
            auto const immediate = static_cast<dotm::int8_t>(memory_.read(register_.pc++));
            auto const condition = bit::extract(instruction_, 3u, 2u);

            switch (condition)
            {
                case 0u: if (!register_.f.z) register_.pc = register_.pc + immediate; break;
                case 1u: if ( register_.f.z) register_.pc = register_.pc + immediate; break;
                case 2u: if (!register_.f.c) register_.pc = register_.pc + immediate; break;
                case 3u: if ( register_.f.c) register_.pc = register_.pc + immediate; break;

                default: std::unreachable();
            }
        }
        void stop      ()
        {
            //auto const immediate     = memory_.read(register_.pc++);
            //interrupt_master_enable_ = dotm::false_;

            throw;
        }
        void ld_r_r    ()
        {
            auto const index        = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const target_index = bit::extract<dotm::uint8_t>(instruction_, 3u, 3u);
            
            register_[target_index] = register_[index];
        }
        void halt      ()
        {
            memory_.write(0xFFFF, dotm::false_);
            throw;
        }
        void add_a_r   ()
        {
            auto const index   = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const addend  = register_[index];
            auto const operand = register_.a;

            register_.a        = register_.a + addend;
            register_.f.z      = register_.a == 0u;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, addend);
            register_.f.c      = bit::test_carry     <bit::operation_e::add>(operand, addend);
        }
        void adc_a_r   ()
        {
            auto const index      = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const addend     = register_[index];
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a + addend + carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::false_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::add>(operand, addend, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::add>(operand, addend, carry_flag);
        }
        void sub_a_r   ()
        {
            auto const index   = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const operand = register_.a;

            register_.a        = register_.a - register_[index];
            register_.f.z      = register_.a == 0u;
            register_.f.n      = dotm::true_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::subtract>(operand, register_[index]);
            register_.f.c      = bit::test_carry     <bit::operation_e::subtract>(operand, register_[index]);
        }
        void sbc_a_r   ()
        {
            auto const index      = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const subtrahend = register_[index];
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a - subtrahend - carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::true_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::subtract>(operand, subtrahend, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::subtract>(operand, subtrahend, carry_flag);
        }
        void and_a_r   ()
        {
            auto const index = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);

            register_.a      = register_.a & register_[index];
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::true_;
            register_.f.c    = dotm::false_;
        }
        void xor_a_r   ()
        {
            auto const index = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);

            register_.a      = register_.a ^ register_[index];
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::false_;
            register_.f.c    = dotm::false_;
        }
        void or_a_r    ()
        {
            auto const index = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);

            register_.a      = register_.a | register_[index];
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::false_;
            register_.f.c    = dotm::false_;
        }
        void cp_a_r    ()
        {
            auto const index  = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            auto const result = register_.a - register_[index];

            register_.f.z     = result == 0u;
            register_.f.n     = dotm::true_;
            register_.f.h     = bit::test_half_carry<bit::operation_e::subtract>(register_.a, register_[index]);
            register_.f.c     = bit::test_carry     <bit::operation_e::subtract>(register_.a, register_[index]);
        }
        void add_a_n   ()
        {
            auto const addend  = memory_.read(register_.pc++);
            auto const operand = register_.a;

            register_.a        = register_.a + addend;
            register_.f.z      = register_.a == 0u;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, addend);
            register_.f.c      = bit::test_carry     <bit::operation_e::add>(operand, addend);
        }
        void adc_a_n   ()
        {
            auto const immediate  = memory_.read(register_.pc++);
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a + immediate + carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::false_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::add>(operand, immediate, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::add>(operand, immediate, carry_flag);
        }
        void sub_a_n   ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const operand   = register_.a;

            register_.a          = register_.a - immediate;
            register_.f.z        = register_.a == 0u;
            register_.f.n        = dotm::true_;
            register_.f.h        = bit::test_half_carry<bit::operation_e::subtract>(operand, immediate);
            register_.f.c        = bit::test_carry     <bit::operation_e::subtract>(operand, immediate);
        }
        void sbc_a_n   ()
        {
            auto const immediate  = memory_.read(register_.pc++);
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a - immediate - carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::true_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::subtract>(operand, immediate, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::subtract>(operand, immediate, carry_flag);
        }
        void and_a_n   ()
        {
            auto const immediate = memory_.read(register_.pc++);

            register_.a          = register_.a & immediate;
            register_.f.z        = register_.a == 0u;
            register_.f.n        = dotm::false_;
            register_.f.h        = dotm::true_;
            register_.f.c        = dotm::false_;
        }
        void xor_a_n   ()
        {
            auto const immediate = memory_.read(register_.pc++);

            register_.a          = register_.a ^ immediate;
            register_.f.z        = register_.a == 0u;
            register_.f.n        = dotm::false_;
            register_.f.h        = dotm::false_;
            register_.f.c        = dotm::false_;
        }
        void or_a_n    ()
        {
            auto const immediate = memory_.read(register_.pc++);

            register_.a          = register_.a | immediate;
            register_.f.z        = register_.a == 0u;
            register_.f.n        = dotm::false_;
            register_.f.h        = dotm::false_;
            register_.f.c        = dotm::false_;
        }
        void cp_a_n    ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const result    = static_cast<dotm::uint8_t>(register_.a - immediate);

            register_.f.z        = result == 0u;
            register_.f.n        = dotm::true_;
            register_.f.h        = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate);
            register_.f.c        = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate);
        }
        void ret_cc    ()
        {
            auto const condition = bit::extract(instruction_, 3u, 2u);

            switch (condition)
            {
                case 0u:
                {
                    if (!register_.f.z)
                    {
                        auto const low_byte  = memory_.read(register_.sp++);
                        auto const high_byte = memory_.read(register_.sp++);
                        register_.pc         = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                    }

                    break;
                }
                case 1u:
                {
                    if (register_.f.z)
                    {
                        auto const low_byte  = memory_.read(register_.sp++);
                        auto const high_byte = memory_.read(register_.sp++);
                        register_.pc         = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                    }

                    break;
                }
                case 2u:
                {
                    if (!register_.f.c)
                    {
                        auto const low_byte  = memory_.read(register_.sp++);
                        auto const high_byte = memory_.read(register_.sp++);
                        register_.pc         = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                    }

                    break;
                }
                case 3u:
                {
                    if (register_.f.c)
                    {
                        auto const low_byte  = memory_.read(register_.sp++);
                        auto const high_byte = memory_.read(register_.sp++);
                        register_.pc         = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                    }

                    break;
                }

                default: std::unreachable();
            }
        }
        void ret       ()
        {
            auto const low_byte  = memory_.read(register_.sp++);
            auto const high_byte = memory_.read(register_.sp++);
            register_.pc         = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
        }
        void reti      ()
        {
            auto const low_byte      = memory_.read(register_.sp++);
            auto const high_byte     = memory_.read(register_.sp++);
            register_.pc             = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            interrupt_master_enable_ = dotm::true_;
        }
        void jp_cc_nn  ()
        {
            auto const condition = bit::extract(instruction_, 3u, 2u);
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);

            if ((condition == 0u && register_.f.z == dotm::false_) ||
                (condition == 1u && register_.f.z == dotm::true_ ) ||
                (condition == 2u && register_.f.c == dotm::false_) ||
                (condition == 3u && register_.f.c == dotm::true_ )  )
            {
                auto const immediate = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                register_.pc         = immediate;
            }
        }
        void jp_nn     ()
        {
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);
            auto const immediate = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            register_.pc         = immediate;
        }
        void jp_hl     ()
        {
            register_.pc = register_.hl;
        }
        void call_cc_nn()
        {
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);
            auto const condition = bit::extract(instruction_, 3u, 2u);

            if ((condition == 0u && register_.f.z == dotm::false_) ||
                (condition == 1u && register_.f.z == dotm::true_ ) ||
                (condition == 2u && register_.f.c == dotm::false_) ||
                (condition == 3u && register_.f.c == dotm::true_ )  )
            {
                auto const [low, high] = bit::unpack<bit::endian_e::little, dotm::uint8_t>(register_.pc);

                memory_.write(--register_.sp, high);
                memory_.write(--register_.sp, low );

                auto const immediate   = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
                register_.pc           = immediate;
            }
        }
        void call_nn   ()
        {
            auto const low_byte          = memory_.read(register_.pc++);
            auto const high_byte         = memory_.read(register_.pc++);
            auto const immediate         = bit::pack  <bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            auto const [low_pc, high_pc] = bit::unpack<bit::endian_e::little, dotm::uint8_t >(register_.pc);
                
            memory_.write(--register_.sp, high_pc);
            memory_.write(--register_.sp, low_pc );
                
            register_.pc                 = immediate;
        }
        void rst       ()
        {
            auto const address           = static_cast<dotm::uint8_t>(instruction_ ^ 0xC7u);
            auto const [low_pc, high_pc] = bit::unpack<bit::endian_e::little, dotm::uint8_t >(register_.pc);

            memory_.write(--register_.sp, high_pc);
            memory_.write(--register_.sp, low_pc );

            register_.pc                 = bit::pack<bit::endian_e::little, dotm::uint16_t>(address, dotm::uint8_t{ 0x00 });
        }
        void pop_rr    ()
        {
            auto const index     = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
            auto const low_byte  = memory_.read(register_.sp++);
            auto const high_byte = memory_.read(register_.sp++);
            
            register_[index]     = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);;

            if (instruction_ == 0xF1)
            {
                register_.f.x = 0u;
            }
        }
        void push_rr   ()
        {
            auto const index       = bit::extract<dotm::uint16_t>(instruction_, 4u, 2u);
            auto const [low, high] = bit::unpack<bit::endian_e::little, dotm::uint8_t>(register_[index]);

            memory_.write(--register_.sp, high);
            memory_.write(--register_.sp, low );
        }
        void prefix    ()
        {
            auto const instruction      = memory_.read(register_.pc++);
            auto const index            = bit::extract(instruction, 0u, 3u);
            auto const bit_index        = bit::extract(instruction, 3u, 3u);
            auto const prf              = (instruction & 0xF8) >> 3;
            auto const prf2             = (instruction & 0xC0) >> 6;
            auto const is_hl_indirect   = (index == 0x06) || (index == 0x0E);
            auto       operand          = is_hl_indirect ? memory_.read(register_.hl) : register_[index];
            auto const original_operand = operand;

            if (prf == 0u) //rlc
            {
                operand = bit::rotate<bit::direction_e::left>(operand, 1u);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 7u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 1u) //rrc
            {
                operand = bit::rotate<bit::direction_e::right>(operand, 1u);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 0u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 2u) //rl
            {
                operand = bit::rotate<bit::direction_e::left>(operand, 1u);
                operand = bit::set(operand, 0u, register_.f.c);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 7u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 3u) //rr
            {
                operand = bit::rotate<bit::direction_e::right>(operand, 1u);
                operand = bit::set(operand, 7u, register_.f.c);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 0u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 4u) //sla
            {
                operand = bit::shift<bit::shift_e::arithmetic, bit::direction_e::left>(operand, 1u);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 7u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 5u) //sra
            {
                operand = bit::shift<bit::shift_e::arithmetic, bit::direction_e::right>(operand, 1u);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 0u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 6u) //swap
            {
                operand = bit::rotate<bit::direction_e::left>(operand, 4u);
                register_.f.z = operand == 0u;
                register_.f.c = dotm::false_;
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf == 7u) //srl
            {
                operand = bit::shift<bit::shift_e::logical, bit::direction_e::right>(operand, 1u);
                register_.f.z = operand == 0u;
                register_.f.c = bit::test(original_operand, 0u);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::false_;
            }
            else if (prf2 == 1u) //bit
            {
                register_.f.z = !bit::test(operand, bit_index);
                register_.f.n = dotm::false_;
                register_.f.h = dotm::true_;
                return; // Don't write back for BIT instruction
            }
            else if (prf2 == 2u) //res
            {
                operand = bit::set(operand, bit_index, dotm::false_);
            }
            else if (prf2 == 3u) //set
            {
                operand = bit::set(operand, bit_index, dotm::true_);
            }
    
            if   (is_hl_indirect) memory_.write(register_.hl, operand);
            else                  register_[index] = operand;
        }
        void ldh_dc_a  ()
        {
            auto const address = bit::pack<bit::endian_e::little, dotm::uint16_t>(register_.c, dotm::uint8_t{ 0xFF });
            memory_.write(address, register_.a);
        }
        void ldh_dn_a  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dotm::uint16_t>(immediate, dotm::uint8_t{ 0xFF });

            memory_.write(address, register_.a);
        }
        void ld_dnn_a  ()
        {
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);

            memory_.write(address, register_.a);
        }
        void ldh_a_dc  ()
        {
            auto const address = bit::pack<bit::endian_e::little, dotm::uint16_t>(register_.c, dotm::uint8_t{ 0xFF });
            register_.a        = memory_.read(address);
        }
        void ldh_a_dn  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dotm::uint16_t>(immediate, dotm::uint8_t{ 0xFF });

            register_.a          = memory_.read(address);
        }
        void ld_a_dnn  ()
        {
            auto const low_byte  = memory_.read(register_.pc++);
            auto const high_byte = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dotm::uint16_t>(low_byte, high_byte);
            
            register_.a          = memory_.read(address);
        }
        void add_sp_e  ()
        {
            auto const offset  = static_cast<dotm::int8_t>(memory_.read(register_.pc++));
            auto const operand = register_.sp;
            
            register_.sp       = register_.sp + offset;
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(static_cast<dotm::uint8_t>(operand), static_cast<dotm::uint8_t>(offset));
            register_.f.c      = bit::test_carry     <bit::operation_e::add>(static_cast<dotm::uint8_t>(operand), static_cast<dotm::uint8_t>(offset));
        }
        void ld_hl_sp_e()
        {
            auto const offset  = memory_.read(register_.pc++);
            auto const operand = register_.sp;

            register_.hl       = register_.sp + static_cast<dotm::int8_t>(offset);
            register_.f.z      = dotm::false_;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(static_cast<dotm::uint8_t>(operand), offset);
            register_.f.c      = bit::test_carry     <bit::operation_e::add>(static_cast<dotm::uint8_t>(operand), offset);
        }
        void ld_sp_hl  ()
        {
            register_.sp = register_.hl;
        }
        void di        ()
        {
            interrupt_master_enable_ = dotm::false_;
        }
        void ei        ()
        {
            interrupt_master_enable_delay = 2u;
        }
        void ld_dhli_a ()
        {
            memory_.write(register_.hl++, register_.a);
        }
        void ld_a_dhli ()
        {
            register_.a = memory_.read(register_.hl++);
        }
        void ld_dhld_a ()
        {
            memory_.write(register_.hl--, register_.a);
        }
        void ld_a_dhld ()
        {
            register_.a = memory_.read(register_.hl--);
        }
        void inc_dhl   ()
        {
            auto const operand = memory_.read(register_.hl);
            auto const value   = static_cast<dotm::uint8_t>(operand + 1u);

            memory_.write(register_.hl, value);
            register_.f.z      = value == 0u;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, dotm::uint8_t{ 1u });
        }
        void dec_dhl   ()
        {
            auto const operand = memory_.read(register_.hl);
            auto const value   = static_cast<dotm::uint8_t>(operand - 1u);

            memory_.write(register_.hl, value);
            register_.f.z      = value == 0u;
            register_.f.n      = dotm::true_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::subtract>(operand, dotm::uint8_t{ 1u });
        }
        void ld_dhl_n  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            
            memory_.write(register_.hl, immediate);
        }
        void ld_r_dhl  ()
        {
            auto const index = bit::extract<dotm::uint8_t>(instruction_, 3u, 3u);
            register_[index] = memory_.read(register_.hl);
        }
        void ld_dhl_r  ()
        {
            auto const index = bit::extract<dotm::uint8_t>(instruction_, 0u, 3u);
            memory_.write(register_.hl, register_[index]);
        }
        void add_a_dhl ()
        {
            auto const addend  = memory_.read(register_.hl);
            auto const operand = register_.a;

            register_.a        = register_.a + addend;
            register_.f.z      = register_.a == 0u;
            register_.f.n      = dotm::false_;
            register_.f.h      = bit::test_half_carry<bit::operation_e::add>(operand, addend);
            register_.f.c      = bit::test_carry     <bit::operation_e::add>(operand, addend);
        }
        void adc_a_dhl ()
        {
            auto const immediate  = memory_.read(register_.hl);
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a + immediate + carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::false_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::add>(operand, immediate, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::add>(operand, immediate, carry_flag);
        }
        void sub_a_dhl ()
        {
            auto const subtrahend = memory_.read(register_.hl);
            auto const operand    = register_.a;

            register_.a           = register_.a - subtrahend;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::true_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::subtract>(operand, subtrahend);
            register_.f.c         = bit::test_carry     <bit::operation_e::subtract>(operand, subtrahend);
        }
        void sbc_a_dhl ()
        {
            auto const immediate  = memory_.read(register_.hl);
            auto const carry_flag = register_.f.c;
            auto const operand    = register_.a;

            register_.a           = register_.a - immediate - carry_flag;
            register_.f.z         = register_.a == 0u;
            register_.f.n         = dotm::true_;
            register_.f.h         = bit::test_half_carry<bit::operation_e::subtract>(operand, immediate, carry_flag);
            register_.f.c         = bit::test_carry     <bit::operation_e::subtract>(operand, immediate, carry_flag);
        }
        void and_a_dhl ()
        {
            auto const value = memory_.read(register_.hl);

            register_.a      = register_.a & value;
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::true_;
            register_.f.c    = dotm::false_;
        }
        void xor_a_dhl ()
        {
            auto const value = memory_.read(register_.hl);

            register_.a      = register_.a ^ value;
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::false_;
            register_.f.c    = dotm::false_;
        }
        void or_a_dhl  ()
        {
            auto const value = memory_.read(register_.hl);

            register_.a      = register_.a | value;
            register_.f.z    = register_.a == 0u;
            register_.f.n    = dotm::false_;
            register_.f.h    = dotm::false_;
            register_.f.c    = dotm::false_;
        }
        void cp_a_dhl  ()
        {
            auto const value  = memory_.read(register_.hl);
            auto const result = register_.a - value;

            register_.f.z     = result == 0u;
            register_.f.n     = dotm::true_;
            register_.f.h     = bit::test_half_carry<bit::operation_e::subtract>(register_.a, value);
            register_.f.c     = bit::test_carry     <bit::operation_e::subtract>(register_.a, value);
        }
        void _         ()
        {
            is_running_ = dotm::false_;
        }



        memory_t             memory_;
        register_t           register_;
        dotm::bool_t         is_running_;
        dotm::instruction8_t instruction_;
        dotm::bool_t         interrupt_master_enable_;
        dotm::uint8_t        interrupt_master_enable_delay;
    };
}
