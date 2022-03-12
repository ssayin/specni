#pragma once

#include <string>

namespace specni::core::swe {

struct Context {
	Context();

	explicit Context(std::string eph_path);

	~Context();
};

struct ContextInit {
	ContextInit();
};

} // namespace specni
