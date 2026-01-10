export module dotm.dmg.specification;

import std;

export namespace dtm::dmg::specification
{
    auto constexpr cpu_name           = "SM83";
    auto constexpr master_clock_speed = 4.194304f;
    auto constexpr system_clock_speed = master_clock_speed / 4.0f;
    auto constexpr work_ram_size      = 8192u;
    auto constexpr video_ram_size     = 8192u;
    auto constexpr screen_resolution  = { 160u, 144u };
}
