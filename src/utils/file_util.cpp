#include "utils/file_util.h"

#include <sstream>
#include "spdlog/spdlog.h"

namespace utils {

std::string ReadFile(const std::string& file_path) {
  std::ifstream ifs(file_path);
  if (ifs.fail()) {
    SPDLOG_ERROR("Failed to open file: {}", file_path);
    return "";
  }

  std::stringstream ss;
  ss << ifs.rdbuf();
  ifs.close();
  return ss.str();
}

}  // namespace utils