export module dotm.sm83_memory;

import std;
import dotm.types;

export namespace dotm
{
    struct sm83_memory
    {
    public:
        auto read (dotm::uint16_t address                   ) -> dotm::byte_t
        {
            if (address == 0xFF44) return 0x90;
            else return memory_[address];
        }
        void write(dotm::uint16_t address, dotm::byte_t value)
        {
            memory_[address] = value;
        }

    //private:
        std::array<dotm::byte_t, 0x10000> memory_;
    };
}
