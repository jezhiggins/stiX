#include "working_filepaths.hpp"
#include <filesystem>

std::string new_working_filepath() {
  static auto const started_at = std::to_string(std::time(nullptr)) + "_" + std::to_string(std::rand()) + "_";
  static auto count = 0;

  auto temp_dir = std::filesystem::temp_directory_path();
  auto temp_name = "sorting_" + started_at + std::to_string(++count);
  return temp_dir / temp_name;
}