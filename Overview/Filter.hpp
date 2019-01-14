#ifndef WHO_EVER_READS_THIS_DEFINES_HPP
#define WHO_EVER_READS_THIS_DEFINES_HPP

#include "CustomFilters.hpp"
#include <QMap>

namespace deeper {

struct Filter
{
    filter::TrackDateFilter trackDate;

    Filter();

private:
    QMap<QString, filter::AbstractFilter *> m_allFilters;
};

} // namespace deeper

#endif // WHO_EVER_READS_THIS_DEFINES_HPP
