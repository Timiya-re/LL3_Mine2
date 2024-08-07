#include "plugin/Exception.h"
#include "plugin/LL3Mine2.h"


namespace LL3Mine2_Class::Exception {


void ProcessException(std::exception_ptr ptr) {
    if (ptr) {
        try {
            std::rethrow_exception(ptr);
        } catch (const std::exception& exc) {
            LOGGER.error("Exception Message:");
            LOGGER.error(exc.what());
        } catch (...) {
            LOGGER.error("Abnormal info is not an abnormal type!");
        }
    } else {
        LOGGER.error("Unable to resolve exception!");
    }
}
} // namespace LL3Mine2_Class::Exception