#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#define _CATCH_CODES(IN)                                                                                               \
    LOGGER.error("Exception In {}!", IN);                                                                              \
    std::exception_ptr exc_ptr = std::current_exception();                                                             \
    LL3Mine2_Class::Exception::ProcessException(exc_ptr);

namespace LL3Mine2_Class::Exception {
class DirCannotCreatedException : public std::exception {
public:
    DirCannotCreatedException(const std::string& _Message) { msg += _Message; }
    DirCannotCreatedException(const char* _Message) { msg += std::string(_Message); }
    const char* what() const override { return msg.c_str(); }


private:
    std::string msg = "[DirCannotCreatedException] ";
};

void ProcessException(std::exception_ptr);
} // namespace LL3Mine2_Class::Exception