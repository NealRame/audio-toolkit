/// utils_pimpl.h
///
/// Created on: October 04, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#pragma once

#include <memory>

#define PIMPL                \
	private:             \
		struct impl; \
		std::unique_ptr<impl> d_;
