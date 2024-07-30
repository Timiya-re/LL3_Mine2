#pragma once

#include <exception>

#define _CATCH_CODES(IN)                                                                                               \
    LOGGER.error("Exception In {}!", IN);                                                                              \
    std::exception_ptr exc_ptr = std::current_exception();                                                             \
    LL3Mine2_Class::Exception::ProcessException(exc_ptr);

namespace LL3Mine2_Class::Exception {
void ProcessException(std::exception_ptr);
}