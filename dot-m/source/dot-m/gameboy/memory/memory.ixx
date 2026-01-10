export module dotm.sm83_memory;

import std;
import dotm.types;

export namespace dtm::dmg
{
    struct memory
    {
    public:
        auto read (dtm::uint16_t address                   ) -> dtm::byte_t
        {
            if (address == 0xFF44) return 0x90;
            else return memory_[address];
        }
        void write(dtm::uint16_t address, dtm::byte_t value)
        {
            memory_[address] = value;
        }

    //private:
        std::array<dtm::byte_t, 0x10000> memory_;
    };
}
