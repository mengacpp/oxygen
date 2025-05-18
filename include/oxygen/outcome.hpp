#pragma once

#include <string>
#include <optional>

#include "log.hpp"

namespace xgn {

  /// Each code represent a different error
  enum class OutcomeCode {
    Ok,
    InvalidParams,
    PermissionDenied,
    MallocFailed,
    IoError,
    Unknown,
    NotFound,
  };

  class Outcome {
  public:

    // constructors

    Outcome(OutcomeCode code, std::string message) : 
      m_code(code),
      m_message(message) {}
    Outcome(const Outcome &outcome) {
      m_code = outcome.m_code;
      m_message = outcome.m_message;
    }
    ~Outcome() = default;

    // error checks

    inline bool is_ok() const 
      {return m_code == OutcomeCode::Ok ? true : false; }
    inline OutcomeCode code() const {return m_code; };

    // message logging

    inline std::string message() const {return m_message; }
    inline std::string error_message() const {return log::Error() << m_message; }
    inline std::string fatal_message() const {return log::Fatal() << m_message; }
    inline std::string warning_message() const {return log::Warning() << m_message; }

  protected:
    OutcomeCode m_code;
    std::string m_message;
  };

  Outcome outcome_ok();

  template <typename T> class OutcomeOr {
  public:
    OutcomeOr(T result) : 
      m_outcome(OutcomeCode::Ok, "Dereference this to access value"), m_result(result) {}
    OutcomeOr(Outcome outcome) : m_outcome(outcome) {}

    inline bool is_ok() const {return m_outcome.is_ok(); }
    inline Outcome outcome() const {return m_outcome; }
    T& operator*() {
      return m_result.value();
    }
    const T& operator*() const {
      return m_result.value();
    }
  private:
    Outcome m_outcome;
    std::optional<T> m_result;
  };

  template <typename U> class OutcomeOr<std::unique_ptr<U>> {
  public:
    OutcomeOr(std::unique_ptr<U> result) : 
      m_outcome(OutcomeCode::Ok, "Dereference this to access value"), 
      m_result(std::move(result)) {}
    OutcomeOr(Outcome outcome) : 
      m_outcome(outcome), 
      m_result(nullptr) {}

    inline bool is_ok() const {return m_outcome.is_ok(); }
    inline Outcome outcome() const {return m_outcome; }

    U* operator->() {
    return m_result.get();
    }
    const U* operator->() const {
        return m_result.get();
    }

    U& operator*() & {
        return *m_result;
    }

    const U& operator*() const & {
        return *m_result;
    }
    
    std::unique_ptr<U> operator*() && {
        return std::move(m_result);
    }

  private:
    Outcome m_outcome;
    std::unique_ptr<U> m_result;
  };
} // namespace oxygen