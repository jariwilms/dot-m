import std;
import dotm;

auto read(const std::filesystem::path& path) -> std::vector<dotm::byte_t>
{
    auto const size = std::filesystem::file_size(path);
    auto       buffer = std::vector<dotm::byte_t>(size);
    auto       file = std::ifstream(path, std::ios::binary);
    if (!file) throw std::runtime_error{ "failed to open file" };

    file.read(reinterpret_cast<dotm::char_t*>(buffer.data()), size);
    return buffer;
}
void write(const std::filesystem::path& path, std::span<const dotm::byte_t> memory)
{
    auto file = std::ofstream(path, std::ios::binary | std::ios::trunc);
    if (!file) throw std::runtime_error{ "failed to open file" };

    file.write(reinterpret_cast<const dotm::char_t*>(memory.data()), memory.size_bytes());
}

//void log_instrs(dotm::sm83& sm83)
//{
//    static std::unordered_map<dotm::instruction8_t, dotm::uint64_t> asd{};
//    if (!asd.contains(sm83.instruction_))
//    {
//        asd[sm83.instruction_] = 0;
//        std::println("new instructions: 0x{:2x}", sm83.instruction_);
//    }
//    else
//    {
//        ++asd[sm83.instruction_];
//    }
//}

void log_register_state(const dotm::sm83& sm83, const std::string& log_file = "debug.log")
{
    std::ofstream log(log_file, std::ios::app);

    auto& regs = sm83.register_;
    auto& mem = sm83.memory_.memory_;

    if (!log) {
        throw std::runtime_error("Failed to open log file");
    }

    log << std::format("A:{:02X} F:{:02X} B:{:02X} C:{:02X} D:{:02X} E:{:02X} "
        "H:{:02X} L:{:02X} SP:{:04X} PC:{:04X} PCMEM:{:02X},{:02X},{:02X},{:02X}\n",
        regs.a, regs.f._, regs.b, regs.c, regs.d, regs.e,
        regs.h, regs.l, regs.sp, regs.pc,
        mem[regs.pc + 0], mem[regs.pc + 1], mem[regs.pc + 2], mem[regs.pc + 3]);
}

auto main() -> int
{
    auto sm83            = dotm::sm83{};
    auto const cartridge = read("test/cpu_instrs/individual/01-special.gb");
    //auto const cartridge = read("dmg_boot.bin");
    auto& memory         = sm83.memory_.memory_;

    std::memcpy(sm83.memory_.memory_.data(), cartridge.data(), cartridge.size());
    std::remove("debug.log");

    auto count = 0;
    while (true)
    {
        log_register_state(sm83);
        //if (sm83.register_.pc == 0xC7B2) __debugbreak();
        sm83.cycle();

        auto& serial_active = memory[0xFF02];
        if (serial_active || memory[0xFF01])
        {
            serial_active = dotm::false_;
            //std::println("serial out: {}", memory[0xFF01]);
        }

        if (++count >= 1256634) __debugbreak();
    }

    std::flush(std::cout);
    return 0;
}
