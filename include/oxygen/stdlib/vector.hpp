#pragma once

#include <unordered_set>
#include <vector>

namespace xgn
{
namespace stdlib
{
namespace vector
{
template <typename T>
bool have_common(const std::vector<T> &v1, const std::vector<T> &v2)
{
    std::unordered_set<T> seen(v1.begin(), v1.end());
    for (const T &x : v2)
    {
        if (seen.count(x))
        {
            return true;
        }
    }
    return false;
}
} // namespace std
} // namespace vector
} // namespace xgn