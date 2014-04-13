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

	auto uint32_slice = buf.get_slice<uint32_t>(0);
	std::cout << "count of uint32_t: " << uint32_slice.count() << std::endl;
	for (auto v: buf.get_slice<uint32_t>(0)) {
		std::cout << v << ' ';
	}
	std::cout << std::endl;

	buffer buf2;

	buf2 = create_buffer(8);
	std::cout << "    size: " << buf2.size() << std::endl;
	std::cout << "capacity: " << buf2.capacity() << std::endl;
	for (auto v: buf2.get_slice<char>(0)) {
		std::cout << v;
	}
	std::cout << std::endl;

	std::cout << "    size: " << buf.size() << std::endl;
	std::cout << "capacity: " << buf.capacity() << std::endl;
	buf.append(buf2);
	std::cout << "    size: " << buf.size() << std::endl;
	std::cout << "capacity: " << buf.capacity() << std::endl;
	for (auto v: buf.get_slice<char>(0)) {
		std::cout << v;
	}
	std::cout << std::endl;

	auto slice = buf.get_slice<char>(0);
	for (auto it = slice.rbegin(), end = slice.rend(); it != end; ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;

	return 0;
}
