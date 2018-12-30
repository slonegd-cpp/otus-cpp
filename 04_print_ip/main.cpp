/**
 * ТЗ
 * Прикладной код должен содержать следующие вызовы:
 * • Печать адреса как char(-1) -> 255
 * • Печать адреса как short(0) -> 0.0
 * • Печать адреса как int(2130706433) -> 127.0.0.1
 * • Печать адреса как long(8875824491850138409) -> 123.45.67.89.101.112.131.41
 * • Печать адреса как std::string
 * • Печать адреса как std::vector
 * • Печать адреса как std::list
 * • Опционально. Печать адреса как std::tuple
 * 
 */
#include "print_ip.h"
#include <sstream>



using namespace std::literals;

int main(int argc, char const *argv[])
{
    try {

        print_ip (static_cast<char>(-1));
        print_ip (static_cast<short>(0));
        print_ip (2130706433);
        print_ip (8875824491850138409L);
        print_ip ("1.2.3.4"s);
        print_ip (std::vector<int>{5,  6,  7,  8});
        print_ip (std::list<int>  {9, 10, 11, 12});
        print_ip (std::make_tuple(-1,  127,  255, 0xF101));

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
