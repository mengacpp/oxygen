#include "oxygen/log.hpp"

std::ostream &operator<<(std::ostream &os, const xgn::log::Message &msg) {
  os << msg.get_output();
  return os;
}