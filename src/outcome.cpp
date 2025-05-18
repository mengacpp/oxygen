#include "oxygen/outcome.hpp"

xgn::Outcome xgn::outcome_ok() {
  return Outcome(
    OutcomeCode::Ok, 
    "Operation completed succesfully");
}