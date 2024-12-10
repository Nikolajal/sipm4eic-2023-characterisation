#pragma once
#include "general_utility.h"
struct TObjects
{
  std::map<std::string, TH1F *> _TH1F;
  std::map<std::string, TH1D *> _TH1D;
  std::map<std::string, TH2F *> _TH2F;
  std::map<std::string, TH2I *> _TH2I;
  std::map<std::string, TH3F *> _TH3F;
  std::map<std::string, TProfile *> _TProfile;
  std::map<std::string, TGraph *> _TGraph;
  std::map<std::string, TGraphErrors *> _TGraphErrors;
};

void write_all(TObjects target)
{
  for (auto [label, object] : target._TProfile)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH1F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH1D)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH2F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH2I)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH3F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TGraph)
    object->Write(label.c_str());
  for (auto [label, object] : target._TGraphErrors)
    object->Write(label.c_str());
}

template <bool logx = true, bool logy = true>
TCanvas *standard_canvas(std::string name, std::string title, float nXpixels = 1000, float nYpixels = 1000)
{
  TCanvas *cResult = new TCanvas(name.c_str(), title.c_str(), nXpixels, nYpixels);
  gStyle->SetOptStat(0);
  gPad->SetGridx(logx);
  gPad->SetGridy(logy);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  return cResult;
}

template <bool logx = true, bool logy = true>
TCanvas *standard_canvas_2D(std::string name, std::string title, float nXpixels = 1145, float nYpixels = 1000)
{
  TCanvas *cResult = new TCanvas(name.c_str(), title.c_str(), nXpixels, nYpixels);
  gStyle->SetOptStat(0);
  gPad->SetGridx(logx);
  gPad->SetGridy(logy);
  gPad->SetRightMargin(0.15);
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  return cResult;
}

std::time_t s_ts(const std::string &datetime)
{
  std::tm tm = {};
  std::stringstream ss(datetime);

  // Parse the datetime string into the tm structure
  ss >> std::get_time(&tm, "%Y%m%d-%H%M%S");

  if (ss.fail())
  {
    // throw std::runtime_error("Failed to parse date time string");
    return 0;
  }

  // Convert tm to time_t (seconds since the epoch)
  std::time_t timestamp = std::mktime(&tm);

  if (timestamp == -1)
  {
    // throw std::runtime_error("Failed to convert to time_t");
    return 0;
  }

  return timestamp;
}

template <typename th1_type = TH1F>
th1_type *
make_log_th1(int n_bins, double min_x, double max_x)
{
  //  swap values is mi max are swapped
  if (min_x > max_x)
    utility::swap_values(min_x, max_x);

  // Create an array to hold bin edges
  double bin_edges[n_bins + 1];

  // Calculate the bin edges in log scale
  double log_min_x = std::log10(min_x);
  double log_max_x = std::log10(max_x);
  double log_step_ = (log_max_x - log_min_x) / n_bins;
  for (int i = 0; i <= n_bins; ++i)
  {
    bin_edges[i] = std::pow(10, log_min_x + i * log_step_);
  }

  //  Build the TH1
  th1_type *result = new th1_type("", "", n_bins, bin_edges);

  return result;
}

namespace utility
{
  template <typename arg_type>
  inline void swap_values(arg_type first_element, arg_type second_element);

}

template <typename arg_type>
inline void utility::swap_values(arg_type first_element, arg_type second_element)
{
  auto memory = second_element;
  second_element = first_element;
  first_element = memory;
}