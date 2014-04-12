#include <iostream>
#include <type_traits>

#include "utils/utils_buffer.h"

static char BUF[] =  "I must not fear. Fear is a mind killer.";
// static char BUF2[] = "I must not kill";


com::nealrame::utils::buffer create_buffer(size_t size) {
	using com::nealrame::utils::buffer;
	buffer buf(size);
	buf.fill('x', size, 0);
	return buf;
}

int
main (int argc, char **argv) {

	using com::nealrame::utils::buffer;

	buffer buf(BUF, sizeof(BUF));

	std::cout << "    size: " << buf.size() << std::endl;
	std::cout << "capacity: " << buf.capacity() << std::endl;

	buf.reserve(2 * buf.size());
	std::cout << "capacity: " << buf.capacity() << std::endl;
	buf.shrink_to_fit();
	std::cout << "capacity: " << buf.capacity() << std::endl;

	std::cout << "count of uint32_t: " << buf.count<uint32_t>() << std::endl;
	for (auto slice = buf.get_slice<uint8_t>(2); slice.first != slice.last; ++(slice.first)) {
		std::cout << *slice.first;
	}
	std::cout << std::endl;

	buffer buf2;

	buf2 = create_buffer(8);
	std::cout << "    size: " << buf2.size() << std::endl;
	std::cout << "capacity: " << buf2.capacity() << std::endl;

	for (auto it = buf2.begin<uint8_t>(), end = buf2.end<uint8_t>(); it != end; ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;

	std::cout << "    size: " << buf.size() << std::endl;
	std::cout << "capacity: " << buf.capacity() << std::endl;
	buf.append(buf2);
	std::cout << "    size: " << buf.size() << std::endl;
	std::cout << "capacity: " << buf.capacity() << std::endl;
	for (auto it = buf.begin<uint8_t>(), end = buf.end<uint8_t>(); it != end; ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;
	for (auto it = buf.rbegin<uint8_t>(), end = buf.rend<uint8_t>(); it != end; ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;
	return 0;
}