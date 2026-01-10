export module dotm.dmg.processor;

import std;
import dotm.bitwise;
import dotm.dmg.processor.cpsr;
import dotm.sm83_memory;
import dotm.dmg.processor.register_;
import dotm.types;

export namespace dtm::dmg
{
    class sm83
    {
    public:
        using memory_t              = dmg::memory;
        using register_t            = dmg::register_<bit::endian_e::little>;
        using instruction_pointer_t = dtm::void_t(sm83::*)();

        sm83()
            : memory_{}
        {
            register_.a                   = 0x01;
            register_.f.value             = 0xB0;
            register_.b                   = 0x00;
            register_.c                   = 0x13;
            register_.d                   = 0x00;
            register_.e                   = 0xD8;
            register_.h                   = 0x01;
            register_.l                   = 0x4D;
            register_.sp                  = 0xFFFE;
            register_.pc                  = 0x0100;

            interrupt_master_enable_      = dtm::false_;
            interrupt_master_enable_delay = 0u;
            is_running_                   = dtm::true_;
        }

        void cycle()
        {
            if (interrupt_master_enable_delay > 0u && (--interrupt_master_enable_delay == 0u)) interrupt_master_enable_ = dtm::true_;

            auto       interrupt_flag   = memory_.read(0xFF0F);
            auto const interrupt_enable = memory_.read(0xFFFF);
            auto const interrupts       = static_cast<dtm::uint8_t>(interrupt_enable & interrupt_flag & 0x1F);
            auto const jumps            = std::array<dtm::uint16_t, 5u>{ 0x40, 0x48, 0x50, 0x58, 0x60 };

            if (interrupt_master_enable_ && interrupts)
            {
                interrupt_master_enable_ = dtm::false_;

                for (auto i = 0u; i < jumps.size(); ++i)
                {
                    if (bit::test(interrupts, i))
                    {
                        memory_.write(0xFF0F, bit::set(interrupt_flag, i, dtm::false_));

                        auto const [low_pc, high_pc] = bit::unpack<bit::endian_e::little, dtm::uint8_t>(register_.pc);
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
        auto fetch     () -> dtm::instruction8_t
        {
            return memory_.read(register_.pc++);
        }
        auto decode    (dtm::instruction8_t instruction) -> instruction_pointer_t
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
                &sm83::jr_cc_e   , &sm83::ld_sp_nn  , &sm83::ld_dhld_a , &sm83::inc_sp    , &sm83::inc_dhl   , &sm83::dec_dhl   , &sm83::ld_dhl_n  , &sm83::scf       , //006_
                &sm83::jr_cc_e   , &sm83::add_hl_sp , &sm83::ld_a_dhld , &sm83::dec_sp    , &sm83::inc_r     , &sm83::dec_r     , &sm83::ld_r_n    , &sm83::ccf       , //007_
                                                                                                                                                                      
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
                &sm83::ldh_a_dn  , &sm83::pop_af    , &sm83::ldh_a_dc  , &sm83::di        , &sm83::_         , &sm83::push_rr   , &sm83::or_a_n    , &sm83::rst       , //036_
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
            auto const index     = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            auto const immediate = read_next<dtm::uint16_t>(register_.pc);
            register_[index]     = immediate;
        }
        void ld_sp_nn  ()
        {
            auto const immediate = read_next<dtm::uint16_t>(register_.pc);
            register_.sp  = immediate;
        }
        void ld_drr_a  ()
        {
            auto const index = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            memory_.write(register_[index], register_.a);
        }
        void ld_a_drr  ()
        {
            auto const index = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            register_.a      = memory_.read(register_[index]);
        }
        void ld_dnn_sp ()
        {
            auto const immediate  = read_next<dtm::uint16_t>(register_.pc);
            auto const [lsp, hsp] = bit::unpack<bit::endian_e::little, dtm::uint8_t>(register_.sp);
            
            memory_.write(immediate     , lsp);
            memory_.write(immediate + 1u, hsp);
        }
        void inc_rr    ()
        {
            auto const index = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            ++register_[index];
        }
        void inc_sp    ()
        {
            ++register_.sp;
        }
        void dec_rr    ()
        {
            auto const index = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            --register_[index];
        }
        void dec_sp    ()
        {
            --register_.sp;
        }
        void add_hl_rr ()
        {
            auto const index       = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            auto const result      = static_cast<dtm::uint16_t>(register_.hl + register_[index]);
            
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.hl, register_[index]);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.hl, register_[index]);
            register_.hl           = result;
        }
        void add_hl_sp ()
        {
            auto const result      = static_cast<dtm::uint16_t>(register_.hl + register_.sp);

            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.hl, register_.sp);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.hl, register_.sp);
            register_.hl           = result;
        }
        void inc_r     ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 3u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_[index] + 1u);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_[index], dtm::uint8_t{ 1u });
            register_[index]       = result;
        }
        void dec_r     ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 3u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_[index] - 1u);
            
            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_[index], dtm::uint8_t{ 1u });
            register_[index]       = result;
        }
        void ld_r_n    ()
        {
            auto const index     = bit::extract<dtm::uint8_t>(instruction_, 3u, 3u);
            auto const immediate = memory_.read(register_.pc++);

            register_[index]     = immediate;
        }
        void rlca      ()
        {
            auto const result      = bit::rotate<bit::direction_e::left>(register_.a, 1u);
            
            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = bit::test(register_.a, 7u);
            register_.a            = result;
        }
        void rrca      ()
        {
            auto const result      = bit::rotate<bit::direction_e::right>(register_.a, 1u);

            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = bit::test(register_.a, 0u);
            register_.a            = result;
        }
        void rla       ()
        {
            auto const result      = bit::set(bit::rotate<bit::direction_e::left>(register_.a, 1u), 0u, register_.f.carry);

            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = bit::test(register_.a, 7u);
            register_.a            = result;
        }
        void rra       ()
        {
            auto const result      = bit::set(bit::rotate<bit::direction_e::right>(register_.a, 1u), 7u, register_.f.carry);
            
            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = bit::test(register_.a, 0u);
            register_.a            = result;
        }
        void daa       ()
        {
            auto correction        = dtm::uint8_t{ 0u };

            if (register_.f.half_carry || (!register_.f.negative && (register_.a & 0x0F) > 0x09)) correction  = 0x06;
            if (register_.f.carry      || (!register_.f.negative &&  register_.a         > 0x99)) correction |= 0x60;

            register_.a           += register_.f.negative ? -correction : correction;

            register_.f.zero       = register_.a == 0u;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = correction >= 0x60;
        }
        void cpl       ()
        {
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = dtm::true_;
            register_.a            = ~register_.a;
        }
        void scf       ()
        {
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::true_;
        }
        void ccf       ()
        {
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = !register_.f.carry;
        }
        void jr_e      ()
        {
            auto const offset = static_cast<dtm::int8_t>(memory_.read(register_.pc++));
            register_.pc      = register_.pc + offset;
        }
        void jr_cc_e   ()
        {
            auto const offset      = static_cast<dtm::int8_t>(memory_.read(register_.pc++));
            auto const condition   = bit::extract(instruction_, 3u, 2u);
            auto const flag        = (condition & 0b10) ? register_.f.carry : register_.f.zero;
            auto const should_jump = flag == (condition & 0b01);

            if (should_jump) register_.pc += offset;
        }
        void stop      ()
        {
            interrupt_master_enable_ = dtm::false_;
            throw;
        }
        void ld_r_r    ()
        {
            auto const index        = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const target_index = bit::extract<dtm::uint8_t>(instruction_, 3u, 3u);
            
            register_[target_index] = register_[index];
        }
        void halt      ()
        {
            memory_.write(0xFFFF, dtm::false_);
            throw;
        }
        void add_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, register_[index]);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, register_[index]);
            register_.a            = result;
        }
        void adc_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + register_[index] + register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, register_[index], register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, register_[index], register_.f.carry);
            register_.a            = result;
        }
        void sub_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, register_[index]);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, register_[index]);
            register_.a            = result;
        }
        void sbc_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - register_[index] - register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, register_[index], register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, register_[index], register_.f.carry);
            register_.a            = result;
        }
        void and_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a & register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::true_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void xor_a_r   ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a ^ register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void or_a_r    ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a | register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void cp_a_r    ()
        {
            auto const index       = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - register_[index]);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, register_[index]);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, register_[index]);
        }
        void add_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, immediate);
            register_.a            = result;
        }
        void adc_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + immediate + register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, immediate, register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, immediate, register_.f.carry);
            register_.a            = result;
        }
        void sub_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate);
            register_.a            = result;
        }
        void sbc_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate - register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate, register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate, register_.f.carry);
            register_.a            = result;
        }
        void and_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a & immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::true_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void xor_a_n   ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a ^ immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void or_a_n    ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a | immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void cp_a_n    ()
        {
            auto const immediate   = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate);
        }
        void ret_cc    ()
        {
            auto const condition = bit::extract(instruction_, 3u, 2u);

            if ((condition == 0u && register_.f.zero  == dtm::false_) ||
                (condition == 1u && register_.f.zero  == dtm::true_ ) ||
                (condition == 2u && register_.f.carry == dtm::false_) ||
                (condition == 3u && register_.f.carry == dtm::true_ )  )
            {
                auto const address = read_next<dtm::uint16_t>(register_.sp);
                register_.pc       = address;
            }
        }
        void ret       ()
        {
            auto const address = read_next<dtm::uint16_t>(register_.sp);
            register_.pc       = address;
        }
        void reti      ()
        {
            auto const address       = read_next<dtm::uint16_t>(register_.sp);
            register_.pc             = address;
            interrupt_master_enable_ = dtm::true_;
        }
        void jp_cc_nn  ()
        {
            auto const condition = bit::extract(instruction_, 3u, 2u);
            auto const address   = read_next<dtm::uint16_t>(register_.pc);

            if ((condition == 0u && register_.f.zero  == dtm::false_) ||
                (condition == 1u && register_.f.zero  == dtm::true_ ) ||
                (condition == 2u && register_.f.carry == dtm::false_) ||
                (condition == 3u && register_.f.carry == dtm::true_ )  )
            {
                register_.pc = address;
            }
        }
        void jp_nn     ()
        {
            auto const immediate = read_next<dtm::uint16_t>(register_.pc);
            register_.pc         = immediate;
        }
        void jp_hl     ()
        {
            register_.pc = register_.hl;
        }
        void call_cc_nn()
        {
            auto const condition = bit::extract(instruction_, 3u, 2u);
            auto const address   = read_next<dtm::uint16_t>(register_.pc);

            if ((condition == 0u && register_.f.zero  == dtm::false_) ||
                (condition == 1u && register_.f.zero  == dtm::true_ ) ||
                (condition == 2u && register_.f.carry == dtm::false_) ||
                (condition == 3u && register_.f.carry == dtm::true_ )  )
            {
                auto const [lpc, hpc] = bit::unpack<bit::endian_e::little, dtm::uint8_t>(register_.pc);
                
                memory_.write(--register_.sp, hpc);
                memory_.write(--register_.sp, lpc);

                register_.pc          = address;
            }
        }
        void call_nn   ()
        {
            auto const address    = read_next<dtm::uint16_t>(register_.pc);
            auto const [lpc, hpc] = bit::unpack<bit::endian_e::little, dtm::uint8_t >(register_.pc);
            
            memory_.write(--register_.sp, hpc);
            memory_.write(--register_.sp, lpc);
                
            register_.pc          = address;
        }
        void rst       ()
        {
            auto const address    = static_cast<dtm::uint8_t>(instruction_ ^ 0xC7u);
            auto const [lpc, hpc] = bit::unpack<bit::endian_e::little, dtm::uint8_t >(register_.pc);

            memory_.write(--register_.sp, hpc);
            memory_.write(--register_.sp, lpc);

            register_.pc          = bit::pack<bit::endian_e::little, dtm::uint16_t>(address, dtm::uint8_t{ 0x00 });
        }
        void pop_rr    ()
        {
            auto const index   = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            auto const address = read_next<dtm::uint16_t>(register_.sp);
            
            register_[index]   = address;
        }
        void pop_af    ()
        {
            auto const index   = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            auto const address = read_next<dtm::uint16_t>(register_.sp);
            
            register_[index]   = address;
            register_.f._      = 0u;
        }
        void push_rr   ()
        {
            auto const index       = bit::extract<dtm::uint16_t>(instruction_, 4u, 2u);
            auto const [low, high] = bit::unpack<bit::endian_e::little, dtm::uint8_t>(register_[index]);

            memory_.write(--register_.sp, high);
            memory_.write(--register_.sp, low );
        }
        void prefix    () //TODO
        {
            auto const instruction    = memory_.read(register_.pc++);
            auto const index          = bit::extract(instruction, 0u, 3u);
            auto const bit_index      = bit::extract(instruction, 3u, 3u);
            auto const opcode         = (instruction & 0xF8) >> 3;
            auto const opcode_group   = (instruction & 0xC0) >> 6;
            auto const is_hl_indirect = (index == 0x06) || (index == 0x0E);

            auto       operand        = is_hl_indirect ? memory_.read(register_.hl) : register_[index];
            auto const original       = operand;

            auto set_flags = [this](dtm::uint8_t val, dtm::flag_t carry)
                {
                    register_.f.zero       = val == 0u;
                    register_.f.carry      = carry;
                    register_.f.negative   = dtm::false_;
                    register_.f.half_carry = dtm::false_;
                };

            switch (opcode)
            {
                case 0u: // RLC - Rotate left circular
                    operand = bit::rotate<bit::direction_e::left>(operand, 1u);
                    set_flags(operand, bit::test(original, 7u));
                    break;

                case 1u: // RRC - Rotate right circular
                    operand = bit::rotate<bit::direction_e::right>(operand, 1u);
                    set_flags(operand, bit::test(original, 0u));
                    break;

                case 2u: // RL - Rotate left through carry
                    operand = bit::rotate<bit::direction_e::left>(operand, 1u);
                    operand = bit::set(operand, 0u, register_.f.carry);
                    set_flags(operand, bit::test(original, 7u));
                    break;

                case 3u: // RR - Rotate right through carry
                    operand = bit::rotate<bit::direction_e::right>(operand, 1u);
                    operand = bit::set(operand, 7u, register_.f.carry);
                    set_flags(operand, bit::test(original, 0u));
                    break;

                case 4u: // SLA - Shift left arithmetic
                    operand = bit::shift<bit::shift_e::arithmetic, bit::direction_e::left>(operand, 1u);
                    set_flags(operand, bit::test(original, 7u));
                    break;

                case 5u: // SRA - Shift right arithmetic
                    operand = bit::shift<bit::shift_e::arithmetic, bit::direction_e::right>(operand, 1u);
                    set_flags(operand, bit::test(original, 0u));
                    break;

                case 6u: // SWAP - Swap nibbles
                    operand = bit::rotate<bit::direction_e::left>(operand, 4u);
                    set_flags(operand, dtm::false_);
                    break;

                case 7u: // SRL - Shift right logical
                    operand = bit::shift<bit::shift_e::logical, bit::direction_e::right>(operand, 1u);
                    set_flags(operand, bit::test(original, 0u));
                    break;
            }

            if (opcode_group == 1u)
            {
                register_.f.zero       = !bit::test(operand, bit_index);
                register_.f.negative   = dtm::false_;
                register_.f.half_carry = dtm::true_;

                return;
            }
            else if (opcode_group == 2u) operand = bit::set(operand, bit_index, dtm::false_);
            else if (opcode_group == 3u) operand = bit::set(operand, bit_index, dtm::true_ );

            if   (is_hl_indirect) memory_.write(register_.hl, operand);
            else                  register_[index] = operand;
        }
        void ldh_dc_a  ()
        {
            auto const address = bit::pack<bit::endian_e::little, dtm::uint16_t>(register_.c, dtm::uint8_t{ 0xFF });
            
            memory_.write(address, register_.a);
        }
        void ldh_dn_a  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dtm::uint16_t>(immediate, dtm::uint8_t{ 0xFF });
            
            memory_.write(address, register_.a);
        }
        void ld_dnn_a  ()
        {
            auto const address = read_next<dtm::uint16_t>(register_.pc);
            
            memory_.write(address, register_.a);
        }
        void ldh_a_dc  ()
        {
            auto const address = bit::pack<bit::endian_e::little, dtm::uint16_t>(register_.c, dtm::uint8_t{ 0xFF });
            
            register_.a        = memory_.read(address);
        }
        void ldh_a_dn  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            auto const address   = bit::pack<bit::endian_e::little, dtm::uint16_t>(immediate, dtm::uint8_t{ 0xFF });

            register_.a          = memory_.read(address);
        }
        void ld_a_dnn  ()
        {
            auto const address = read_next<dtm::uint16_t>(register_.pc);
            
            register_.a        = memory_.read(address);
        }
        void add_sp_e  ()
        {
            auto const offset      = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint16_t>(register_.sp + static_cast<dtm::int8_t>(offset));

            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(static_cast<dtm::uint8_t>(register_.sp), offset);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(static_cast<dtm::uint8_t>(register_.sp), offset);
            register_.sp           = result;
        }
        void ld_hl_sp_e()
        {
            auto const offset      = memory_.read(register_.pc++);
            auto const result      = static_cast<dtm::uint16_t>(register_.sp + static_cast<dtm::int8_t>(offset));

            register_.f.zero       = dtm::false_;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(static_cast<dtm::uint8_t>(register_.sp), offset);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(static_cast<dtm::uint8_t>(register_.sp), offset);
            register_.hl           = result; 
        }
        void ld_sp_hl  ()
        {
            register_.sp = register_.hl;
        }
        void di        ()
        {
            interrupt_master_enable_ = dtm::false_;
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
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(immediate + 1u);

            memory_.write(register_.hl, result);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(immediate, dtm::uint8_t{ 1u });
        }
        void dec_dhl   ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(immediate - 1u);

            memory_.write(register_.hl, result);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(immediate, dtm::uint8_t{ 1u });
        }
        void ld_dhl_n  ()
        {
            auto const immediate = memory_.read(register_.pc++);
            
            memory_.write(register_.hl, immediate);
        }
        void ld_r_dhl  ()
        {
            auto const index = bit::extract<dtm::uint8_t>(instruction_, 3u, 3u);

            register_[index] = memory_.read(register_.hl);
        }
        void ld_dhl_r  ()
        {
            auto const index = bit::extract<dtm::uint8_t>(instruction_, 0u, 3u);
            
            memory_.write(register_.hl, register_[index]);
        }
        void add_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, immediate);
            register_.a            = result;
        }
        void adc_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a + immediate + register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::add>(register_.a, immediate, register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::add>(register_.a, immediate, register_.f.carry);
            register_.a            = result;
        }
        void sub_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate);
            register_.a            = result;
        }
        void sbc_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate - register_.f.carry);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate, register_.f.carry);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate, register_.f.carry);
            register_.a            = result;
        }
        void and_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a & immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::true_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void xor_a_dhl ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a ^ immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void or_a_dhl  ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a | immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::false_;
            register_.f.half_carry = dtm::false_;
            register_.f.carry      = dtm::false_;
            register_.a            = result;
        }
        void cp_a_dhl  ()
        {
            auto const immediate   = memory_.read(register_.hl);
            auto const result      = static_cast<dtm::uint8_t>(register_.a - immediate);

            register_.f.zero       = result == 0u;
            register_.f.negative   = dtm::true_;
            register_.f.half_carry = bit::test_half_carry<bit::operation_e::subtract>(register_.a, immediate);
            register_.f.carry      = bit::test_carry     <bit::operation_e::subtract>(register_.a, immediate);
        }
        void _         ()
        {
            is_running_ = dtm::false_;
        }

        template<typename value_t>
        auto read_next(dtm::uint16_t& address) -> value_t
        {
                 if constexpr (std::is_same_v<value_t, dtm::uint8_t >)
            {
                return memory_.read(address++);
            }
            else if constexpr (std::is_same_v<value_t, dtm::uint16_t>)
            {
                auto const alpha = memory_.read(address++);
                auto const beta  = memory_.read(address++);
                
                return bit::pack<bit::endian_e::little, dtm::uint16_t>(alpha, beta);
            }
            else static_assert(dtm::false_, "invalid type");
        }



        memory_t             memory_;
        register_t           register_;
        dtm::bool_t         is_running_;
        dtm::instruction8_t instruction_;
        dtm::bool_t         interrupt_master_enable_;
        dtm::uint8_t        interrupt_master_enable_delay;
    };
}
