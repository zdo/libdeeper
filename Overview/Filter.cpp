#include "Filter.hpp"

namespace deeper {

Filter::Filter()
{
    m_allFilters["trackDate"] = &trackDate;
}

} // namespace deeper
