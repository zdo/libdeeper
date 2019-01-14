#ifndef CUSTOMFILTERS_HPP
#define CUSTOMFILTERS_HPP

#include <QDate>

namespace deeper {
namespace filter {

struct AbstractFilter
{
    bool isEnabled;
};

struct TrackDateFilter : public AbstractFilter
{
    QDate start, end;
};

}
} // namespace deeper

#endif // CUSTOMFILTERS_HPP
