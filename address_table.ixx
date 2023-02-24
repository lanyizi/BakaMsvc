module;
#include <wil/result_macros.h>
#include <string_view>
#include <unordered_map>
export module address_table;

export namespace address_table
{
    std::unordered_map<std::string_view, void*> const& get_address_table() noexcept;

    template<typename T = void*>
        requires(std::is_pointer_v<T>)
    T get_address(std::string_view key) noexcept
    {
        auto& table = get_address_table();
        FAIL_FAST_IF_MSG(not table.contains(key), "missing key `%.*hs` in address table", key.size(), key.data());
        return static_cast<T>(table.at(key));
    }
}

module: private;
using namespace std::literals;

namespace
{
    std::unordered_map<std::string_view, void*> build_address_table() noexcept;
}

std::unordered_map<std::string_view, void*> const& address_table::get_address_table() noexcept
{
    static auto const table = build_address_table();
    return table;
}

namespace
{
    std::unordered_map<std::string_view, void*> build_address_table() noexcept
    {
        static int foo = 0;
        auto result = std::unordered_map<std::string_view, void*>{};
        auto add = [&result](std::string_view key, std::uintptr_t value) noexcept
        {
            FAIL_FAST_IF_MSG(result.contains(key), "duplicate key `%.*hs` in address table", key.size(), key.data());
            result[key] = reinterpret_cast<void*>(value);
        };
        add("test"sv, reinterpret_cast<std::uintptr_t>(&foo));
        return result;
    }
}
