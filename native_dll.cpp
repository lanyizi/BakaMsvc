#include <string_view>
import address_table;

extern "C" __declspec(dllexport) void test()
{
    *address_table::get_address<int*>("test") = 0;
}