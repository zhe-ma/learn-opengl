#include "spdlog/spdlog.h"
int main()
{
  SPDLOG_ERROR("{}", "asdf");
  //Use the default logger (stdout, multi-threaded, colored)
}