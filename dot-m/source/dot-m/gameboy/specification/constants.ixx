export module dotm.gameboy.constants;

import dotm.types;

export namespace dtm::dmg
{
    auto constexpr rom_00_range           = dtm::memory_range{ 0x0000, 0x3FFF };
    auto constexpr rom_01_range           = dtm::memory_range{ 0x4000, 0x7FFF };
    auto constexpr vram_range             = dtm::memory_range{ 0x8000, 0xBFFF };
    auto constexpr wram_00_range          = dtm::memory_range{ 0xC000, 0xCFFF };
    auto constexpr wram_01_range          = dtm::memory_range{ 0xD000, 0xDFFF };
    auto constexpr echo_ram_range         = dtm::memory_range{ 0xE000, 0xFDFF };
    auto constexpr oam_range              = dtm::memory_range{ 0xFE00, 0xFE9F };
    auto constexpr prohibited_range       = dtm::memory_range{ 0xFEA0, 0xFEFF };
    auto constexpr io_register_range      = dtm::memory_range{ 0xFF00, 0xFF7F };
    auto constexpr hram_range             = dtm::memory_range{ 0xFF80, 0xFFFE };
}
export namespace dtm::dmg::hwr
{
    auto constexpr p1   = dtm::address16_t{ 0xFF00 };
    auto constexpr sb   = dtm::address16_t{ 0xFF01 };
    auto constexpr sc   = dtm::address16_t{ 0xFF02 };
    auto constexpr div  = dtm::address16_t{ 0xFF04 };
    auto constexpr tima = dtm::address16_t{ 0xFF05 };
    auto constexpr tma  = dtm::address16_t{ 0xFF06 };
    auto constexpr tac  = dtm::address16_t{ 0xFF07 };
    auto constexpr if_  = dtm::address16_t{ 0xFF0F };
    auto constexpr nr10 = dtm::address16_t{ 0xFF10 };
    auto constexpr nr11 = dtm::address16_t{ 0xFF11 };
    auto constexpr nr12 = dtm::address16_t{ 0xFF12 };
    auto constexpr nr13 = dtm::address16_t{ 0xFF13 };
    auto constexpr nr14 = dtm::address16_t{ 0xFF14 };
    auto constexpr nr21 = dtm::address16_t{ 0xFF16 };
    auto constexpr nr22 = dtm::address16_t{ 0xFF17 };
    auto constexpr nr23 = dtm::address16_t{ 0xFF18 };
    auto constexpr nr24 = dtm::address16_t{ 0xFF19 };
    auto constexpr nr30 = dtm::address16_t{ 0xFF1A };
    auto constexpr nr31 = dtm::address16_t{ 0xFF1B };
    auto constexpr nr32 = dtm::address16_t{ 0xFF1C };
    auto constexpr nr33 = dtm::address16_t{ 0xFF1D };
    auto constexpr nr34 = dtm::address16_t{ 0xFF1E };
    auto constexpr nr41 = dtm::address16_t{ 0xFF20 };
    auto constexpr nr42 = dtm::address16_t{ 0xFF21 };
    auto constexpr nr43 = dtm::address16_t{ 0xFF22 };
    auto constexpr nr44 = dtm::address16_t{ 0xFF23 };
    auto constexpr nr50 = dtm::address16_t{ 0xFF24 };
    auto constexpr nr51 = dtm::address16_t{ 0xFF25 };
    auto constexpr nr52 = dtm::address16_t{ 0xFF26 };
    auto constexpr wave = dtm::address16_t{ 0xFF30 };
    auto constexpr lcdc = dtm::address16_t{ 0xFF40 };
    auto constexpr stat = dtm::address16_t{ 0xFF41 };
    auto constexpr scy  = dtm::address16_t{ 0xFF42 };
    auto constexpr scx  = dtm::address16_t{ 0xFF43 };
    auto constexpr ly   = dtm::address16_t{ 0xFF44 };
    auto constexpr lyc  = dtm::address16_t{ 0xFF45 };
    auto constexpr dma  = dtm::address16_t{ 0xFF46 };
    auto constexpr bgp  = dtm::address16_t{ 0xFF47 };
    auto constexpr obp0 = dtm::address16_t{ 0xFF48 };
    auto constexpr obp1 = dtm::address16_t{ 0xFF49 };
    auto constexpr wy   = dtm::address16_t{ 0xFF4A };
    auto constexpr wx   = dtm::address16_t{ 0xFF4B };
    auto constexpr key0 = dtm::address16_t{ 0xFF4C };
    auto constexpr key1 = dtm::address16_t{ 0xFF4D };
    auto constexpr vbk  = dtm::address16_t{ 0xFF4F };
    auto constexpr bank = dtm::address16_t{ 0xFF50 };
    auto constexpr hdma1= dtm::address16_t{ 0xFF51 };
    auto constexpr hdma2= dtm::address16_t{ 0xFF52 };
    auto constexpr hdma3= dtm::address16_t{ 0xFF53 };
    auto constexpr hdma4= dtm::address16_t{ 0xFF54 };
    auto constexpr hdma5= dtm::address16_t{ 0xFF55 };
    auto constexpr rp   = dtm::address16_t{ 0xFF56 };
    auto constexpr bcps = dtm::address16_t{ 0xFF68 };
    auto constexpr bcpd = dtm::address16_t{ 0xFF69 };
    auto constexpr ocps = dtm::address16_t{ 0xFF6A };
    auto constexpr ocpd = dtm::address16_t{ 0xFF6B };
    auto constexpr opri = dtm::address16_t{ 0xFF6C };
    auto constexpr svbk = dtm::address16_t{ 0xFF70 };
    auto constexpr pcm12= dtm::address16_t{ 0xFF76 };
    auto constexpr pcm34= dtm::address16_t{ 0xFF77 };
    auto constexpr ie   = dtm::address16_t{ 0xFFFF };
}
