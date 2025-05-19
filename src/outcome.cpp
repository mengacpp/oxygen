#include "oxygen/outcome.hpp"

void xgn::Outcome::update(xgn::Outcome out)
{
    if (out.is_ok() || !is_ok())
        return;

    m_code = out.code();
    m_message = out.message();
}