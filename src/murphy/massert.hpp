#pragma once

#include <murphy\log\logger_macro.hpp>


#ifdef MRP_ASSERT

#define CHECK(expr)                                                            \
    do {                                                                        \
        if (!static_cast<bool>(expr)) [[unlikely]]                              \
            MRP_CORE_ERROR("Failed assert {} {}", __FILE__, __LINE__);          \
    } while (0)
	
//#else

//#define CHECK(expr) (expr)

#endif