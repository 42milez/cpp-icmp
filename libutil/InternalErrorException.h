#ifndef ICMP_INTERNALERROREXCEPTION_H
#define ICMP_INTERNALERROREXCEPTION_H


#include <exception>
#include <string>

namespace util
{
  class InternalErrorException : public std::exception {
  public:
    explicit InternalErrorException(std::string msg);
    const char* what() const noexcept override;
  private:
    std::string msg_;
  };
}


#endif // ICMP_INTERNALERROREXCEPTION_H
