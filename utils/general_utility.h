//  --- --- ---
//  General utility functions
//
//  Auth: Nicola Rubini
//  Mail: nicola.rubini@bo.infn.it
//
#pragma once

namespace utility
{
  //  Swap two values
  template <typename arg_type>
  inline void swap_values(arg_type &first_element, arg_type &second_element);
  //  Square sum
  template <typename arg_type>
  inline arg_type sq_sum(arg_type value);
  template <typename arg_type, typename... Args>
  inline arg_type sq_sum(arg_type first, Args... args);
  //  Average
  // template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
  // std::map<std::string, std::array<T, 2>> average(std::vector<std::array<T, 2>> list_of_measurements, bool skip_unfit_skim = false);
  //  Set precision of the number
  template <typename T>
  T round_digits(T value, int digits);
  //  Concatenate vectors
  template <typename T>
  std::vector<T> merge(const std::vector<T> &vec1, const std::vector<T> &vec2);
  //  Get folder list of a location
  std::vector<std::string> get_folders(const std::string &dir_path);
}

template <typename arg_type>
inline void utility::swap_values(arg_type &first_element, arg_type &second_element)
{
  auto memory = second_element;
  second_element = first_element;
  first_element = memory;
}

template <typename arg_type>
inline arg_type utility::sq_sum(arg_type value)
{
  return value * value; // Base case: square the single argument
}

template <typename arg_type, typename... Args>
inline arg_type utility::sq_sum(arg_type first, Args... args)
{
  return first * first + sq_sum(args...); // Recursive call
}

template <typename T>
T utility::round_digits(T value, int digits)
{
  //  Check the passed argument is a number
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric");
  T factor = std::pow(10, digits);
  return std::ceil(value * factor) / factor;
}

template <typename T>
std::vector<T> utility::merge(const std::vector<T> &vec1, const std::vector<T> &vec2)
{
  std::vector<T> result = vec1;                          // Start with the first vector
  result.insert(result.end(), vec2.begin(), vec2.end()); // Concatenate the second vector
  return result;
}

std::vector<std::string> utility::get_folders(const std::string &dir_path)
{
  std::vector<std::string> folders;
  for (const auto &entry : std::filesystem::directory_iterator(dir_path))
    if (entry.is_directory())
      folders.push_back(entry.path().filename().string());
  return folders;
}

namespace utility
{
  //  Check again
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
  std::map<std::string, std::array<T, 2>> average(std::vector<std::array<T, 2>> list_of_measurements, bool skip_unfit_skim = false)
  {
    //  Final result
    std::map<std::string, std::array<T, 2>> result;
    std::vector<std::array<T, 2>> skimmed_data;

    //  Skim dataset from dangerous values
    skimmed_data = list_of_measurements;
    if (!skip_unfit_skim)
    {
      skimmed_data.clear();
      for (auto current_measurement : list_of_measurements)
      {
        if (std::isnan(current_measurement[0]) || std::isnan(current_measurement[1]))
          continue;
        if (std::isinf(current_measurement[0]) || std::isinf(current_measurement[1]))
          continue;
        skimmed_data.push_back(current_measurement);
      }
    }

    //  Measure average and error on the average
    for (auto current_measurement : skimmed_data)
    {
      result["ave_err"][0] += current_measurement[0];
      result["ave_rms"][0] += current_measurement[0];
      result["ave_sqe"][0] += current_measurement[0];
      result["sqa_err"][0] += current_measurement[0] * current_measurement[0];
      result["sqa_rms"][0] += current_measurement[0] * current_measurement[0];
      result["sqa_sqe"][0] += current_measurement[0] * current_measurement[0];
      result["ave_err"][1] += current_measurement[1];
      // result["ave_rms"][1] += current_measurement[1];
      result["ave_sqe"][1] += current_measurement[1] * current_measurement[1];
      result["sqa_err"][1] += current_measurement[1];
      // result["sqa_rms"][1] += current_measurement[1];
      result["sqa_sqe"][1] += current_measurement[1] * current_measurement[1];
    }
    result["ave_err"][0] /= skimmed_data.size();
    result["ave_rms"][0] /= skimmed_data.size();
    result["ave_sqe"][0] /= skimmed_data.size();
    result["sqa_err"][0] /= skimmed_data.size();
    result["sqa_rms"][0] /= skimmed_data.size();
    result["sqa_sqe"][0] /= skimmed_data.size();
    result["sqa_err"][0] = sqrt(result["sqa_err"][0]);
    result["sqa_rms"][0] = sqrt(result["sqa_rms"][0]);
    result["sqa_sqe"][0] = sqrt(result["sqa_sqe"][0]);
    result["ave_err"][1] /= skimmed_data.size();
    // result["ave_rms"][1] += current_measurement[1];
    result["ave_sqe"][1] = sqrt(result["ave_sqe"][1]);
    result["sqa_err"][1] /= skimmed_data.size();
    // result["sqa_rms"][1] += current_measurement[1];
    result["sqa_sqe"][1] = sqrt(result["sqa_sqe"][1]);

    //  Measure RMS

    return result;
  }

}