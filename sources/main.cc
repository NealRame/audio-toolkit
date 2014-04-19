#include <iostream>
#include <type_traits>

#include <audio/buffer>

using namespace com::nealrame::audio;


int main (int argc, char **argv) {

	std::cout << std::is_convertible<buffer::frame::reference, buffer::const_frame::reference>::value << std::endl;

	return 0;
}
