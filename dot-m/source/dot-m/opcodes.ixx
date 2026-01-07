export module opcodes;

import std;
import dotm.bitwise;
import dotm.processor;
import dotm.sm83_memory;
import dotm.sm83_register;
import dotm.types;

export namespace dotm
{
    class opcodes
    {
    public:
        using instruction_pointer_t = dotm::void_t(opcodes::*)();

    //private:

        void instruction_0x00()
        {

        }
        void instruction_0x01()
        {
            register_.bc = bit::pack<bit::endian_e::little, dotm::uint16_t>(memory_.read(register_.pc++), memory_.read(register_.pc++));
        }
        void instruction_0x02()
        {
            memory_.write(register_.bc, register_.a);
        }
        void instruction_0x03()
        {
            ++register_.bc;
        }
        void instruction_0x04()
        {
            auto const operand = register_.b;

            register_.b   = register_.b + dotm::uint8_t{ 1u };
            register_.f.z = register_.b == 0u;
            register_.f.n = dotm::false_;
            register_.f.h = bit::check_half_carry<std::plus>(operand, dotm::uint8_t{ 1u });
        }
        void instruction_0x05()
        {

        }
        void instruction_0x06()
        {

        }
        void instruction_0x07()
        {

        }
        void instruction_0x08()
        {

        }
        void instruction_0x09()
        {

        }
        void instruction_0x0A()
        {

        }
        void instruction_0x0B()
        {

        }
        void instruction_0x0C()
        {

        }
        void instruction_0x0D()
        {

        }
        void instruction_0x0E()
        {

        }
        void instruction_0x0F()
        {

        }

        void instruction_0x10()
        {

        }
        void instruction_0x11()
        {

        }
        void instruction_0x12()
        {

        }
        void instruction_0x13()
        {

        }
        void instruction_0x14()
        {

        }
        void instruction_0x15()
        {

        }
        void instruction_0x16()
        {

        }
        void instruction_0x17()
        {

        }
        void instruction_0x18()
        {

        }
        void instruction_0x19()
        {

        }
        void instruction_0x1A()
        {

        }
        void instruction_0x1B()
        {

        }
        void instruction_0x1C()
        {

        }
        void instruction_0x1D()
        {

        }
        void instruction_0x1E()
        {

        }
        void instruction_0x1F()
        {

        }

        dotm::bool_t                               is_running_;
        dotm::instruction8_t                       instruction_;
        dotm::sm83_memory                          memory_;
        dotm::sm83_register<bit::endian_e::little> register_;
    };
}
