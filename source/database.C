#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "TROOT.h"
#include "TFile.h"
#include "TStyle.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TLatex.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TVirtualFitter.h"
#include "make_iv_scan.C"
#include "ureadout_dcr_get.C"
#include "../utils/graphutils.C"

namespace database
{
  //  --- Constants ---
  //  Physics constants
  const double k_boltz_eV = 8.617333262 * 1.e-5; // eV K^{-1}
  const double k_boltz_J = 1.380649 * 1.e-23;    // J K^{-1}
  //
  //  --- Sensors oriented quantities ---
  //  All sensors
  std::vector<std::string> sensors_full = {"HPK S13360-3050VS", "HPK S13360-3075VS", "HPK S14161-3050HS"};
  std::vector<std::string> sensors_short = {"S13361_3050", "S13361_3075", "S14161_3050"};
  //
  //  Board oriented quantities
  //  --- All available boards
  std::vector<std::string> all_boards = {
      "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
      "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
      "31", "32", "33", "34"};
  std::unordered_map<std::string, std::vector<std::string>> standard_status_to_boards = {
      {"NEW", all_boards},
      {"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}},
      {"LNL-IRR1", {"21", "22", "23", "24", "25"}},
      {"TIFPA-OANN", {"2"}}};
  std::unordered_map<std::string, std::string> standard_status_to_status = {
      {"NEW", "NEW"},
      {"TIFPA-IRR1", "TIFPA-IRR1"},
      {"LNL-IRR1", "LNL-IRR1"},
      {"TIFPA-OANN", "OANN-150-150h"}};
  //  --- Board to status
  //  --- --- { Board, Status list }
  std::unordered_map<std::string, std::vector<std::string>> board_to_status_list; // TBI: Fill it reading from the database
  //
  //  Channel oriented quantities
  //  --- All available channels
  std::vector<std::string> all_channels = {
      "A1", "A2", "A3", "A4",
      "B1", "B2", "B3", "B4",
      "C1", "C2", "C3", "C4"};
  //  --- Channel to sensor
  //  --- --- { Channels, Sensor }
  std::unordered_map<std::string, std::string> channel_to_sensor = {
      {"A1", "HPK S13360-3050VS"},
      {"A2", "HPK S13360-3050VS"},
      {"A3", "HPK S13360-3050VS"},
      {"A4", "HPK S13360-3050VS"},
      {"B1", "HPK S13360-3075VS"},
      {"B2", "HPK S13360-3075VS"},
      {"B3", "HPK S13360-3075VS"},
      {"B4", "HPK S13360-3075VS"},
      {"C1", "HPK S14161-3050HS"},
      {"C2", "HPK S14161-3050HS"},
      {"C3", "HPK S14161-3050HS"},
      {"C4", "HPK S14161-3050HS"}};
  std::unordered_map<std::string, std::string> channel_to_sensor_short = {
      {"A1", "S13361_3050"},
      {"A2", "S13361_3050"},
      {"A3", "S13361_3050"},
      {"A4", "S13361_3050"},
      {"B1", "S13361_3075"},
      {"B2", "S13361_3075"},
      {"B3", "S13361_3075"},
      {"B4", "S13361_3075"},
      {"C1", "S14161_3050"},
      {"C2", "S14161_3050"},
      {"C3", "S14161_3050"},
      {"C4", "S14161_3050"}};
  //  --- Sensor to channels
  //  --- --- { Sensor, Channels }
  std::unordered_map<std::string, std::vector<std::string>> sensor_to_channels = {
      {"HPK S13360-3050VS", {"A1", "A2", "A3", "A4"}},
      {"HPK S13360-3075VS", {"B1", "B2", "B3", "B4"}},
      {"HPK S14161-3050HS", {"C1", "C2", "C3", "C4"}}};
  std::unordered_map<std::string, std::vector<std::string>> sensor_short_to_channels = {
      {"S13361_3050", {"A1", "A2", "A3", "A4"}},
      {"S13361_3075", {"B1", "B2", "B3", "B4"}},
      {"S14161_3050", {"C1", "C2", "C3", "C4"}}};
  //
  //  Status oriented quantities
  //  --- All available statuses
  std::vector<std::string> all_statuses = {
      "NEW",                                                                                   // New State
      "TIFPA-IRR1", "TIFPA-IRR1on",                                                            // Irradiated
      "SANN-100-30m", "SANN-100-2.5h", "SANN-100-24h",                                         // First cycle of fwd/rev annealing
      "SANN-125-30m", "SANN-125-2.5h", "SANN-125-24h",                                         // Second cycle of fwd/rev annealing
      "SANN-150-30m", "SANN-150-2.5h", "SANN-150-24h", "SANN-150-73h", "SANN-150-250h",        // Third cycle of fwd/rev annealing
      "PANN1", "PANN2", "PANN3", "PANN3x",                                                     // Preventive annealing
      "TEMP313", "TEMP303", "TEMP293", "TEMP283", "TEMP278", "TEMP273", "TEMP263", "TEMP253"}; // Temperature Vbd Scan
  std::vector<std::string> TScan_statuses = {
      "TEMP303", "TEMP283", "TEMP278", "TEMP273", "TEMP263", "TEMP253"};
  //  --- Status to annealing time
  //  --- --- { Status, Time (min) }
  std::unordered_map<std::string, double> status_to_annealing_time = {
      {"NEW", -1},              // New sensors
      {"TIFPA-IRR1", 0},        // Irradiated sensors
      {"TIFPA-IRR1-ON", 158.3}, // Online annealing -> Fix in the database.txt
      {"SANN-100-30m", 30},     // Self annealing @100C
      {"SANN-100-2.5h", 180},
      {"SANN-100-24h", 1620},
      {"SANN-125-30m", 1650}, // Self annealing @125C
      {"SANN-125-2.5h", 1830},
      {"SANN-125-24h", 3240},
      {"SANN-150-30m", 3270}, // Self annealing @150C
      {"SANN-150-2.5h", 3450},
      {"SANN-150-24h", 4860},
      {"SANN-150-73h", 9240},
      {"SANN-150-250h", 24240},
      {"SANN-175-30m", 24270},
      {"PANN1", 9000}, // Preventive annealing @150C
      {"PANN2", 18000},
      {"PANN3", 9000},
      {"PANN3x", 27000}};
  //  --- Status to temperature
  std::unordered_map<std::string, int> status_to_color = {
      {"NEW", kAzure - 3},
      {"TIFPA-IRR1", kGreen + 2},
      {"LNL-IRR1", kSpring - 7},
      {"LNL-IRR2", kGreen - 5},
      {"GIF-IRR1", kOrange},
      {"OANN-150-150h", kRed - 3}};
  std::unordered_map<std::string, int> status_to_marker = {
      {"NEW", 20},
      {"TIFPA-IRR1", 20},
      {"GIF-IRR1", 20},
      {"LNL-IRR1", 21},
      {"LNL-IRR2", 21},
      {"OANN-150-150h", 20}};
  //
  //  Sensor oriented quantities
  std::vector<std::string> all_sensors = {
      "HPK S13360-3050VS",
      "HPK S13360-3075VS",
      "HPK S14161-3050HS"};
  std::unordered_map<std::string, int> sensor_to_color = {
      {"HPK S13360-3050VS", kRed},
      {"HPK S13360-3075VS", kGreen - 2},
      {"HPK S14161-3050HS", kAzure - 3}};
  std::unordered_map<std::string, int> sensor_to_marker = {
      {"HPK S13360-3050VS", 20},
      {"HPK S13360-3075VS", 21},
      {"HPK S14161-3050HS", 29}};
  std::unordered_map<std::string, std::string> sensor_to_code = {
      {"HPK S13360-3050VS", "HPK_13_50"},
      {"HPK S13360-3075VS", "HPK_13_75"},
      {"HPK S14161-3050HS", "HPK_14_50"}};
  std::unordered_map<std::string, std::string> code_to_sensor = {
      {"HPK_13_50", "HPK S13360-3050VS"},
      {"HPK_13_75", "HPK S13360-3075VS"},
      {"HPK_14_50", "HPK S14161-3050HS"}};
  std::unordered_map<std::string, float> sensor_to_vbd = {
      {"HPK S13360-3050VS", 48.4},
      {"HPK S13360-3075VS", 47.9},
      {"HPK S14161-3050HS", 36.5}};
  std::unordered_map<std::string, float> sensor_to_vbd_Tdep = {
      {"HPK S13360-3050VS", 0.054},
      {"HPK S13360-3075VS", 0.054},
      {"HPK S14161-3050HS", 0.034}};
  std::unordered_map<std::string, float> DTS_PDE = {
      {"HPK S13360-3050VS", 0.4},
      {"HPK S13360-3075VS", 0.5},
      {"HPK S14161-3050HS", 0.5}};
  std::unordered_map<std::string, float> DTS_OVop = {
      {"HPK S13360-3050VS", 3.},
      {"HPK S13360-3075VS", 3.},
      {"HPK S14161-3050HS", 2.7}};
  std::vector<float> standard_overvoltage_points = {
      1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0};
  //
  //  --- Utility data structures ---
  //  --- Data structure in database reference
  std::vector<std::string> fields = {
      "run", "quality", "step", "setup", "iv-mux-1", "iv-mux-2", "dcr-chip-2", "dcr-chip-3"};
  std::vector<std::string> criteria = {
      "run", "quality", "step", "setup", "iv-mux-1", "iv-mux-2", "dcr-chip-2", "dcr-chip-3", "temp", "ann-temp", "ann-time"};
  //  --- Global variables
  std::string basedir = ".";
  //  --- Board, Channel, Status, Curve (IV, DCR, Gain)
  std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, TGraphErrors *>>>> cache_memory;
  //  --- Board, Status, Information (set-up, mux, dcr-chip )
  std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>> database_memory;
  //  --- TGraph creation
  using mkgraph_data_structure = std::unordered_map<std::array<float, 2>, std::vector<std::array<float, 2>>>;
  //
  //  --- Declarations ---
  //  --- I/O
  void read_database(std::string fname);
  //  --- Getters
  TGraphErrors *get_iv_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_vbias_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_threshold_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_gain(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  std::array<float, 2> get_cross_talk(std::string board, std::string channel, std::string step, std::string check_name = "");
  TGraphErrors *get_iv_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_vbias_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_threshold_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_gain(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::array<float, 2>
  get_Yvalue_at_X(std::string board, std::string channel, std::string step, float target_voltage);
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::array<float, 2>
  get_value_at_overvoltage(std::string board, std::string channel, std::string step, float target_overvoltage);
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_values_at_overvoltage(std::string board, std::string sensor, std::string step, float target_overvoltage);
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_values_at_overvoltage(std::vector<std::string> boards, std::string sensor, std::string step, float target_overvoltage);
  std::unordered_map<std::string, std::string>
  get_filename(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters = {{}});
  std::string
  get_database_info(std::string board, std::string step, std::string info);
  std::vector<std::array<std::string, 2>> get_board_step_history(std::string board, bool is_dcr = false, bool return_runlist = false)
  {
    std::vector<std::array<std::string, 2>> result;
    for (auto [current_status, all_infos] : database_memory[board])
    {
      if ((all_infos.find("quality") != all_infos.end()) && all_infos["quality"][0] == "good")
        if ((is_dcr ? all_infos.find("dcr-run") != all_infos.end() : all_infos.find("iv-run") != all_infos.end()) && ((is_dcr ? all_infos["dcr-run"][0] : all_infos["iv-run"][0]) != ""))
          result.push_back({current_status, is_dcr ? all_infos["dcr-run"][0] : all_infos["iv-run"][0]});
    }
    std::sort(result.begin(), result.end(), [](const auto &a, const auto &b)
              { return a[1] < b[1]; });
    return result;
  }

  //  Setters
  template <typename target_type>
  void set_standard_style(target_type *target, int marker_style = 20, int marker_color = kBlack)
  {
    //  Line qualities
    target->SetLineColor(marker_color);
    target->SetLineStyle(kSolid);
    target->SetLineWidth(2);
    //  Marker qualities
    target->SetMarkerColor(marker_color);
    target->SetMarkerStyle(marker_style);
    target->SetMarkerSize(2);
    //  Fill qualities
    target->SetFillStyle(0);
    target->SetFillColor(marker_color);
  }

  //   --- Quality assurance
  void check_all_plots_loaded();
  bool check_iv_goodness(TGraphErrors *gTarget);

  //  Calculations
  std::array<float, 2> measure_cross_talk(TGraphErrors *gTarget, std::string plot_name = "");
  std::array<float, 2> measure_signal_amp(TGraphErrors *gTarget, std::string plot_name = "");
  std::array<float, 2> measure_fraction_of_damage(std::array<float, 2> target_step, std::array<float, 2> new_reference, std::array<float, 2> maximum_reference);

  //  Utilities
  std::vector<TGraphErrors *> make_graph(mkgraph_data_structure data_structure);
  void set_multi_graph(std::vector<TGraphErrors *> &target_graphs, int target_color, int target_marker);
  std::array<float, 2> measure_average(std::vector<std::array<float, 2>> measurements, bool use_rms = false);

  //  Implementations
  //  ---  I/O
  void read_database(std::string database_input_file)
  {
    //  Start reading the file
    std::ifstream data_stream(database_input_file);
    std::string current_line;
    while (std::getline(data_stream, current_line))
    {
      //  Skip comment characters
      if (current_line[0] == '#' || current_line[0] == ' ')
        continue;
      //  Read database
      std::stringstream string_in_stream(current_line);
      std::unordered_map<std::string, std::string> data_by_field;
      std::string current_data;
      for (auto current_field : fields)
      {
        string_in_stream >> current_data;
        data_by_field[current_field] = current_data;
      }
      //  Record IV from multiplexer 1
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["iv-run"].push_back(data_by_field["run"]);
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["iv-setup"].push_back(data_by_field["setup"]);
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["iv-mux"].push_back("1");
      //  Record IV from multiplexer 2
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["iv-run"].push_back(data_by_field["run"]);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["iv-setup"].push_back(data_by_field["setup"]);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["iv-mux"].push_back("2");
      //  Record DCR from chip 2
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["dcr-run"].push_back(data_by_field["run"]);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["dcr-setup"].push_back(data_by_field["setup"]);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["dcr-chip"].push_back("2");
      //  Record DCR from chip 3
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["dcr-run"].push_back(data_by_field["run"]);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["dcr-setup"].push_back(data_by_field["setup"]);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["dcr-chip"].push_back("3");
      //  Record quality
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["quality"].push_back(data_by_field["quality"]);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["quality"].push_back(data_by_field["quality"]);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["quality"].push_back(data_by_field["quality"]);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["quality"].push_back(data_by_field["quality"]);
      //  Record temperature
      std::string temperature = "243";
      if (data_by_field["step"].find("TEMP") != std::string::npos)
      {
        temperature = data_by_field["step"].substr(data_by_field["step"].find("TEMP") + 4, data_by_field["step"].find("TEMP") + 7);
      }
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["temp"].push_back(temperature);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["temp"].push_back(temperature);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["temp"].push_back(temperature);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["temp"].push_back(temperature);
      //  Record annealing temperature and time
      std::string anneal_temp = "-1";
      std::string anneal_time = "-1";
      if (data_by_field["step"].find("ANN") != std::string::npos)
      {
        std::stringstream stream;
        //  String with annealing time and temp
        //  --- Extracting temperature
        auto temp_time_argument = data_by_field["step"].substr(data_by_field["step"].find("ANN-") + 4);
        if ((temp_time_argument.find("-") != std::string::npos))
        {
          auto temp_anneal_temp = std::stof(temp_time_argument.substr(0, temp_time_argument.find("-"))) + 273;
          stream << std::fixed << std::setprecision(0) << temp_anneal_temp;
          anneal_temp = stream.str();
        }
        //  --- Extracting annealing time
        auto time = temp_time_argument.substr(temp_time_argument.find("-") + 1);
        auto time_m = -0.5 / 60;
        auto time_h = -0.5 / (60 * 60);
        if ((time.find("m") != std::string::npos))
        {
          auto temp_time_m = time.substr(time.find_first_of("h") + 1, -time.find_first_of("h") + time.find_first_of("m") - 1);
          time_m = std::stof(temp_time_m);
        }
        if ((time.find("h") != std::string::npos))
        {
          auto temp_time_h = time.substr(0, time.find_first_of("h"));
          time_h = std::stof(time.substr(0, time.find_first_of("h")));
        }
        stream.str("");
        stream << std::fixed << std::setprecision(0) << time_m * 60 + time_h * 60 * 60;
        anneal_time = stream.str();
      }
      //  Add annealing temperature to infos
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["ann-temp"].push_back(anneal_temp);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["ann-temp"].push_back(anneal_temp);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["ann-temp"].push_back(anneal_temp);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["ann-temp"].push_back(anneal_temp);
      //  Add annealing time to infos
      database_memory[data_by_field["iv-mux-1"]][data_by_field["step"]]["ann-time"].push_back(anneal_time);
      database_memory[data_by_field["iv-mux-2"]][data_by_field["step"]]["ann-time"].push_back(anneal_time);
      database_memory[data_by_field["dcr-chip-2"]][data_by_field["step"]]["ann-time"].push_back(anneal_time);
      database_memory[data_by_field["dcr-chip-3"]][data_by_field["step"]]["ann-time"].push_back(anneal_time);
    }
  }

  //  --- Getters
  //  --- --- General functions
  //  --- --- --- Getters at a given X

  //  - F - Getter of a value at a given x, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::array<float, 2>
  get_Yvalue_at_X(std::string board, std::string channel, std::string step, float target_x)
  {
    auto target_curve = current_curve_getter(board, channel, step, 0, 0);
    if (!target_curve)
      return {-1., -1.};
    return graphutils::eval_with_errors(target_curve, target_x);
  }
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, std::string)>
  std::array<float, 2>
  get_Yvalue_at_X(std::string board, std::string channel, std::string step, float target_x)
  {
    auto target_curve = current_curve_getter(board, channel, step, "center");
    if (!target_curve)
      return {-1., -1.};
    auto result = graphutils::eval_with_errors(target_curve, target_x);
    /*
    TCanvas *c1 = new TCanvas();
    target_curve->Draw("ALPE");
    cout << board << " - " << channel << " - " << step << " - " << result[0] << " - " << result[1] << " target_x:" << target_x << endl;
    */
    return result;
  }

  //  - F - Getter of all values for a given sensor at a given x, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_sensor_Yvalues_at_X(std::string board, std::string sensor, std::string step, float target_x)
  {
    std::vector<std::array<float, 2>> result;
    for (auto current_channel : sensor_to_channels[sensor])
      result.push_back(get_Yvalue_at_X<current_curve_getter>(board, current_channel, step, target_x));
    return result;
  }

  //  - F - Getter of all values for a given sensor at a given x for all listed boards, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_sensor_Yvalues_at_X(std::vector<std::string> board_list, std::string sensor, std::string step, float target_x)
  {
    std::vector<std::array<float, 2>> result;
    for (auto current_board : board_list)
      for (auto current_channel : sensor_to_channels[sensor])
        result.push_back(get_Yvalue_at_X<current_curve_getter>(current_board, current_channel, step, target_x));
    return result;
  }

  //  - F - Getter of TGraphErrors with average values for a given sensor at a given list of x for all listed boards, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int), bool use_rms = false>
  TGraphErrors *
  get_average_sensor_Yvalues_at_X(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> target_x_list)
  {
    TGraphErrors *gResult = new TGraphErrors;
    for (auto current_x : target_x_list)
    {
      auto current_point = gResult->GetN();
      auto current_point_y = measure_average(get_all_sensor_Yvalues_at_X<current_curve_getter>(board_list, sensor, step, current_x), use_rms);
      gResult->SetPoint(current_point, current_x, current_point_y[0]);
      gResult->SetPointError(current_point, 0., current_point_y[1]);
    }
    return gResult;
  }

  //  - F - Getter of TGraphErrors with average values for a given sensor at a given list of x for one board, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int), bool use_rms = false>
  TGraphErrors *
  get_average_sensor_Yvalues_at_X(std::string board, std::string sensor, std::string step, std::vector<float> target_x_list)
  {
    return get_average_sensor_Yvalues_at_X<current_curve_getter, use_rms>({board}, sensor, step, target_x_list);
  }

  /*
    TCanvas *template_history_at_overvoltage(std::vector<std::pair<std::vector<std::string>,std::array<std::string, 2>>> step_map, float overvoltage)
    {
      //  Canvas
      TCanvas *c1 = new TCanvas("", "", 1800, 600);
      c1->Divide(3, 1);
      gStyle->SetOptStat(0);
      TLatex *lLatex = new TLatex();

      //  Frame creation
      TH1F *hframe;
      if (!time_axis)
        hframe = new TH1F("hframe", ";;current (A)", steps.size(), 0, steps.size());
      else
        hframe = new TH1F("hframe", ";;current (A)", 1000, s_ts(steps[0][1]) - 1000, s_ts(steps[steps.size() - 1][1]) + 1000);
      hframe->SetMaximum(max);
      hframe->SetMinimum(min);

      //  TGraphErrors for all sensors type
      TGraphErrors *gSingleGraphs = new TGraphErrors[12];
      TGraphErrors *gSummaryGraphs = new TGraphErrors[6];

      //  Loop over requested steps
      auto iStep = -1;
      for (auto current_step : steps)
      {
        iStep++;
        auto iSensor = -1;
        for (auto current_sensor : sensors_full)
        {
          auto current_timestamp = s_ts(current_step[1]);
          iSensor++;
          if (!time_axis)
            hframe->GetXaxis()->SetBinLabel(iStep + 1, current_step[0].c_str());
          auto current_overvoltage_values = get_all_iv_at_overvoltage(board, current_sensor, current_step[0], overvoltage);
          auto average_overvoltage_values = measure_average(current_overvoltage_values);
          auto average_overvoltage_valRMS = measure_average(current_overvoltage_values, true);
          auto current_point = gSummaryGraphs[0 + iSensor * 2].GetN();
          time_axis ? gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, current_timestamp, average_overvoltage_values[0]) : gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, 0.5 + iStep, average_overvoltage_values[0]);
          gSummaryGraphs[0 + iSensor * 2].SetPointError(current_point, 0., average_overvoltage_values[1]);
          time_axis ? gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, current_timestamp, average_overvoltage_valRMS[0]) : gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, 0.5 + iStep, average_overvoltage_valRMS[0]);
          gSummaryGraphs[1 + iSensor * 2].SetPointError(current_point, 0., average_overvoltage_valRMS[1]);
          for (auto single_sensor = 0; single_sensor < 4; single_sensor++)
          {
            current_point = gSingleGraphs[iSensor * 4 + single_sensor].GetN();
            time_axis ? gSingleGraphs[iSensor * 4 + single_sensor].SetPoint(current_point, current_timestamp, current_overvoltage_values[single_sensor][0]) : gSingleGraphs[iSensor * 4 + single_sensor].SetPoint(current_point, 0.5 + iStep, current_overvoltage_values[single_sensor][0]);
            gSingleGraphs[iSensor * 4 + single_sensor].SetPointError(current_point, 0., current_overvoltage_values[single_sensor][1]);
          }
        }
      }
      if (time_axis)
      {
        hframe->GetXaxis()->SetTimeDisplay(true);
        hframe->LabelsOption("v");
      }
      c1->cd(1);
      hframe->Draw();
      gPad->SetLogy();
      lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[0].c_str());
      lLatex->DrawLatexNDC(0.725, 0.9, Form("board #%s", board.c_str()));
      c1->cd(2);
      hframe->Draw();
      gPad->SetLogy();
      lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[1].c_str());
      c1->cd(3);
      hframe->Draw();
      gPad->SetLogy();
      lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[2].c_str());
      for (auto iSns = 0; iSns < 12; iSns++)
      {
        gSingleGraphs[iSns].SetMarkerStyle(24);
        gSingleGraphs[iSns].SetMarkerColor(kGray);
        gSingleGraphs[iSns].SetLineColor(kGray);
        c1->cd(1 + iSns / 4);
        gSingleGraphs[iSns].Draw("samelp");
      }
      for (auto iSns = 0; iSns < 6; iSns++)
      {
        gSummaryGraphs[iSns].SetMarkerStyle(20);
        gSummaryGraphs[iSns].SetMarkerColor(kBlack);
        gSummaryGraphs[iSns].SetLineColor(kBlack);
        c1->cd(1 + iSns / 2);
        gSummaryGraphs[iSns].Draw("samelp");
      }
      return c1;
    }
  */

  //
  //  --- --- --- Getters at a given overvoltage

  //  - F - Getter of a value at a given overvoltage, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::array<float, 2>
  get_value_at_overvoltage(std::string board, std::string channel, std::string step, float target_overvoltage)
  {
    return get_Yvalue_at_X<current_curve_getter>(board, channel, step, sensor_to_vbd[channel_to_sensor[channel]] + target_overvoltage);
  }
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, std::string)>
  std::array<float, 2>
  get_value_at_overvoltage(std::string board, std::string channel, std::string step, float target_overvoltage)
  {
    return get_Yvalue_at_X<current_curve_getter>(board, channel, step, sensor_to_vbd[channel_to_sensor[channel]] + target_overvoltage);
  }

  //  - F - Getter of all values for a given sensor at a given overvoltage, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_values_at_overvoltage(std::string board, std::string sensor, std::string step, float target_overvoltage)
  {
    return get_all_sensor_Yvalues_at_X<current_curve_getter>(board, sensor, step, sensor_to_vbd[sensor] + target_overvoltage);
  }

  //  - F - Getter of all values for a given sensor at a given overvoltage for all listed boards, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int)>
  std::vector<std::array<float, 2>>
  get_all_values_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, float target_overvoltage)
  {
    return get_all_sensor_Yvalues_at_X<current_curve_getter>(board_list, sensor, step, sensor_to_vbd[sensor] + target_overvoltage);
  }

  //  - F - Getter of TGraphErrors with average values for a given sensor at a given list of overvoltages for all listed boards, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int), bool use_rms = false>
  TGraphErrors *
  get_average_sensor_values_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points, int marker = kCircle, int color = kBlue)
  {
    for (auto &current_overvoltage : overvoltage_points)
      current_overvoltage += sensor_to_vbd[sensor] + sensor_to_vbd_Tdep[sensor] * (std::stof(get_database_info(board_list[0], step, "temp")) - 243);
    auto result = get_average_sensor_Yvalues_at_X<current_curve_getter, use_rms>(board_list, sensor, step, overvoltage_points);
    for (auto iPnt = 0; iPnt < result->GetN(); ++iPnt)
      result->GetX()[iPnt] -= sensor_to_vbd[sensor] + sensor_to_vbd_Tdep[sensor] * (std::stof(get_database_info(board_list[0], step, "temp")) - 243);
    result->SetMarkerStyle(marker);
    result->SetMarkerColor(color);
    return result;
  }

  //  - F - Getter of TGraphErrors with average values for a given sensor at a given list of x for one board, the curve is taken as a template argument
  template <TGraphErrors *(*current_curve_getter)(std::string, std::string, std::string, int, int), bool use_rms = false>
  TGraphErrors *
  get_average_sensor_values_at_overvoltage(std::string board, std::string sensor, std::string step, std::vector<float> target_x_list, int marker = kCircle, int color = kBlue)
  {
    return get_average_sensor_values_at_overvoltage<current_curve_getter, use_rms>({board}, sensor, step, target_x_list, marker, color);
  }

  //
  //  --- --- IV

  //  - F - Main function for IV curve getter
  TGraphErrors *get_iv_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker, int color)
  {
    std::string key_word = "IV";
    //  Check the IV curve has been stored
    if (cache_memory.find(board) != cache_memory.end())
    {
      if (cache_memory[board].find(channel) != cache_memory[board].end())
      {
        if (cache_memory[board][channel].find(step) != cache_memory[board][channel].end())
        {
          if (cache_memory[board][channel][step].find(key_word) != cache_memory[board][channel][step].end())
          {
            graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
            return cache_memory[board][channel][step][key_word];
          }
        }
      }
    }
    //  Not found: we need to calculate it
    auto file_name = get_filename(board, channel, step, other_filters);
    //  Database do not have the run number, return nullptr
    if (file_name["iv"].empty() || file_name["iv-open"].empty())
    {
      std::cout << "[WARNING][database::get_iv_scan] Run number not found in database" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  Make iv scan
    cache_memory[board][channel][step][key_word] = make_iv_scan(file_name["iv"], file_name["iv-open"]);
    //  If something went wrong, return nullptr
    if (!cache_memory[board][channel][step][key_word])
    {
      std::cout << "[WARNING][database::get_iv_scan] IV scan TGraph error" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  If TGraphErrors has 0 points, return nullptr
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      std::cout << "[WARNING][database::get_iv_scan] IV scan TGraph has 0 points" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  Set requested style
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
  }

  //  - F - Simplified function for IV curve getter
  TGraphErrors *
  get_iv_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_iv_scan(board, channel, step, {{}}, marker, color);
  }

  //  - F - Get average IV over listed parameters
  template <bool use_rms = false>
  TGraphErrors *
  get_average_iv_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points, int marker = kCircle, int color = kBlue)
  {
    return get_average_sensor_values_at_overvoltage<get_iv_scan, use_rms>(board_list, sensor, step, overvoltage_points, marker, color);
  }

  //
  //  --- --- DCR

  //  - F - Main function for DCR curve getter
  TGraphErrors *
  get_dcr_vbias_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker, int color)
  {
    std::string key_word = "DCR_vbias";
    //  Check the DCR curve has been stored
    if (cache_memory.find(board) != cache_memory.end())
    {
      if (cache_memory[board].find(channel) != cache_memory[board].end())
      {
        if (cache_memory[board][channel].find(step) != cache_memory[board][channel].end())
        {
          if (cache_memory[board][channel][step].find(key_word) != cache_memory[board][channel][step].end())
          {
            graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
            return cache_memory[board][channel][step][key_word];
          }
        }
      }
    }
    //  Not found: we need to calculate it
    auto file_name = get_filename(board, channel, step, other_filters);
    if (file_name["dcr-vbias"].empty())
    {
      std::cout << "[WARNING][database::get_dcr_vbias_scan] Run number not found in database (B: " << board << " C:" << channel << " S:" << step << ")" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    cache_memory[board][channel][step][key_word] = ureadout_dcr_get(file_name["dcr-vbias"], "bias_voltage", "dead_rate");
    if (!cache_memory[board][channel][step][key_word])
    {
      std::cout << "[WARNING][database::get_dcr_vbias_scan] DCR vbias scan TGraph error" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      std::cout << "[WARNING][database::get_dcr_vbias_scan] DCR vbias scan TGraph has 0 points" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
  }

  //  - F - Simplified function for DCR curve getter
  TGraphErrors *get_dcr_vbias_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_dcr_vbias_scan(board, channel, step, {{}}, marker, color);
  }

  //  - F - Get average gain over listed parameters
  template <bool use_rms = false>
  TGraphErrors *
  get_average_dcr_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points, int marker = kCircle, int color = kBlue)
  {
    return get_average_sensor_values_at_overvoltage<get_dcr_vbias_scan, use_rms>(board_list, sensor, step, overvoltage_points, marker, color);
  }

  //
  //  --- --- Gain

  //  - F - Main function for Gain curve getter
  TGraphErrors *
  get_gain(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker, int color)
  {
    //  Check the Gain curve has been stored
    if (cache_memory.find(board) != cache_memory.end())
    {
      if (cache_memory[board].find(channel) != cache_memory[board].end())
      {
        if (cache_memory[board][channel].find(step) != cache_memory[board][channel].end())
        {
          if (cache_memory[board][channel][step].find("Gain") != cache_memory[board][channel][step].end())
          {
            graphutils::set_style(cache_memory[board][channel][step]["Gain"], marker, color);
            return cache_memory[board][channel][step]["Gain"];
          }
        }
      }
    }
    //  Recover IV and DCR curves
    auto reference_iv = get_iv_scan(board, channel, step, other_filters, marker, color);
    if (!reference_iv)
    {
      cache_memory[board][channel][step]["Gain"] = nullptr;
      return cache_memory[board][channel][step]["Gain"];
    }
    auto current_iv = ((TGraphErrors *)(reference_iv)->Clone("current_iv"));
    if (!current_iv)
    {
      cache_memory[board][channel][step]["Gain"] = nullptr;
      return cache_memory[board][channel][step]["Gain"];
    }
    auto current_dcr = get_dcr_vbias_scan(board, channel, step, other_filters, marker, color);
    if (!current_dcr)
    {
      cache_memory[board][channel][step]["Gain"] = nullptr;
      return cache_memory[board][channel][step]["Gain"];
    }
    //  Subctract surface current
    auto baseline = 0.;
    for (int iPnt = 0; iPnt < 5; ++iPnt)
    {
      baseline += current_iv->GetY()[iPnt];
    }
    graphutils::y_shift(current_iv, baseline / 5.);
    //  Make ratio
    cache_memory[board][channel][step]["Gain"] = graphutils::ratio(current_iv, current_dcr);
    if (!cache_memory[board][channel][step]["Gain"])
    {
      cache_memory[board][channel][step]["Gain"] = nullptr;
      return cache_memory[board][channel][step]["Gain"];
    }
    graphutils::y_scale(cache_memory[board][channel][step]["Gain"], 1. / TMath::Qe());
    //
    return cache_memory[board][channel][step]["Gain"];
  }

  //  - F - Simplified function for Gain curve getter
  TGraphErrors *
  get_gain(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_gain(board, channel, step, {{}}, marker, color);
  }

  //  - F - Get gain at overvoltage
  std::array<float, 2>
  get_gain_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage)
  {
    return database::get_value_at_overvoltage<database::get_gain>(board, channel, step, overvoltage);
  }

  //  - F - Get average gain over listed parameters
  template <bool use_rms = false>
  TGraphErrors *
  get_average_gain_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points, int marker = kCircle, int color = kBlue)
  {
    return get_average_sensor_values_at_overvoltage<get_gain>(board_list, sensor, step, overvoltage_points, marker, color);
  }

  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //  --- SORT ME PLEASE FOR THE LOG --
  //  --- --- DCR
  //  --- --- --- Vbias

  std::array<float, 2>
  get_dcr_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage)
  {
    return database::get_value_at_overvoltage<database::get_dcr_vbias_scan>(board, channel, step, overvoltage);
  }
  std::vector<std::array<float, 2>>
  get_all_dcr_at_overvoltage(std::string board, std::string sensor, std::string step, float overvoltage)
  {
    return database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(board, sensor, step, overvoltage);
  }
  std::vector<std::array<float, 2>>
  get_all_dcr_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, float overvoltage)
  {
    std::vector<std::array<float, 2>> result;
    for (auto current_board : board_list)
    {
      auto list_of_current_values = database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(current_board, sensor, step, overvoltage);
      for (auto current_value : list_of_current_values)
        result.push_back(current_value);
    }
    return result;
  }
  std::vector<std::array<float, 2>>
  get_all_dcr_at_overvoltage(std::string sensor, std::string step, float overvoltage)
  {
    std::vector<std::array<float, 2>> result;
    for (auto current_board : standard_status_to_boards[step])
    {
      auto list_of_current_values = database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(current_board, sensor, step, overvoltage);
      for (auto current_value : list_of_current_values)
        result.push_back(current_value);
    }
    return result;
  }

  //  Recover standard procedure with std::pair
  std::tuple<float, float, float>
  get_PDE_merit(std::vector<std::string> board_list, std::string sensor, std::string step, float window_of_acquisition = 1.)
  {
    std::tuple<float, float, float> result;
    auto dcr_avg = measure_average(get_all_dcr_at_overvoltage(board_list, sensor, step, DTS_OVop[sensor]), false);
    auto dcr_RMS = measure_average(get_all_dcr_at_overvoltage(board_list, sensor, step, DTS_OVop[sensor]), true);
    get<0>(result) = (DTS_PDE[sensor] / (dcr_avg[0] * window_of_acquisition));
    get<1>(result) = (dcr_avg[1] / dcr_avg[0]) * (DTS_PDE[sensor] / (dcr_avg[0] * window_of_acquisition));
    get<2>(result) = (dcr_RMS[1] / dcr_avg[0]) * (DTS_PDE[sensor] / (dcr_avg[0] * window_of_acquisition));
    return result;
  }

  std::tuple<float, float, float>
  get_relative_PDE_merit(std::tuple<float, float, float> target_merit, std::tuple<float, float, float> standard_merit)
  {
    float ref = get<0>(standard_merit);
    std::tuple<float, float, float> result;
    get<0>(result) = (get<0>(target_merit) - ref) / ref;
    get<1>(result) = get<1>(target_merit) / ref;
    get<2>(result) = get<2>(target_merit) / ref;
    // get<1>(result) = sqrt((get<1>(target_merit) / get<0>(target_merit)) * (get<1>(target_merit) / get<0>(target_merit)) + ((get<1>(standard_merit) * get<0>(target_merit) / (get<0>(standard_merit) * get<0>(standard_merit)))) * (get<1>(standard_merit) * get<0>(target_merit) / (get<0>(standard_merit) * get<0>(standard_merit))));
    // get<2>(result) = sqrt((get<2>(target_merit) / get<0>(target_merit)) * (get<2>(target_merit) / get<0>(target_merit)) + ((get<2>(standard_merit) * get<0>(target_merit) / (get<0>(standard_merit) * get<0>(standard_merit)))) * (get<2>(standard_merit) * get<0>(target_merit) / (get<0>(standard_merit) * get<0>(standard_merit))));
    return result;
  }

  //  Generalise
  template <bool use_rms>
  TGraphErrors *
  get_dcr_overvoltage_scan(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points)
  {
    TGraphErrors *gResult = new TGraphErrors;
    for (auto current_overvoltage : overvoltage_points)
    {
      auto current_point = gResult->GetN();
      auto current_point_y = measure_average(get_all_dcr_at_overvoltage(board_list, sensor, step, current_overvoltage), use_rms);
      gResult->SetPoint(current_point, current_overvoltage, current_point_y[0]);
      gResult->SetPointError(current_point, 0., current_point_y[1]);
    }
    return gResult;
  }

  std::array<float, 2>
  get_dcr_excess_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage)
  {
    auto current_dcr = database::get_value_at_overvoltage<database::get_dcr_vbias_scan>(board, channel, step, overvoltage);
    auto new_step_dcr = database::get_value_at_overvoltage<database::get_dcr_vbias_scan>(board, channel, "NEW", overvoltage);
    return {current_dcr[0] - new_step_dcr[0], sqrt(current_dcr[1] * current_dcr[1] + new_step_dcr[1] * new_step_dcr[1])};
  }
  std::vector<std::array<float, 2>>
  get_all_dcr_excess_at_overvoltage(std::string board, std::string sensor, std::string step, float overvoltage)
  {
    std::vector<std::array<float, 2>> result;
    auto all_current_dcr = database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(board, sensor, step, overvoltage);
    auto all_new_step_dcr = database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(board, sensor, "NEW", overvoltage);
    auto iTer = -1;
    for (auto current_dcr : all_current_dcr)
    {
      iTer++;
      auto new_step_dcr = all_new_step_dcr[iTer];
      result.push_back({current_dcr[0] - new_step_dcr[0], sqrt(current_dcr[1] * current_dcr[1] + new_step_dcr[1] * new_step_dcr[1])});
    }
    return result;
  }
  std::vector<std::array<float, 2>>
  get_all_dcr_excess_at_overvoltage(std::vector<std::string> board_list, std::string sensor, std::string step, float overvoltage)
  {
    std::vector<std::array<float, 2>> result;
    for (auto current_board : board_list)
    {
      auto list_of_current_values = database::get_all_dcr_excess_at_overvoltage(current_board, sensor, step, overvoltage);
      for (auto current_value : list_of_current_values)
        result.push_back(current_value);
    }
    return result;
  }
  template <bool use_rms>
  TGraphErrors *
  get_dcr_increase_overvoltage_scan(std::vector<std::string> board_list, std::string sensor, std::string step, std::vector<float> overvoltage_points = standard_overvoltage_points)
  {
    TGraphErrors *gResult = new TGraphErrors;
    for (auto current_overvoltage : overvoltage_points)
    {
      auto current_point = gResult->GetN();
      auto current_point_y = measure_average(get_all_dcr_excess_at_overvoltage(board_list, sensor, step, current_overvoltage), use_rms);
      gResult->SetPoint(current_point, current_overvoltage, current_point_y[0]);
      gResult->SetPointError(current_point, 0., current_point_y[1]);
    }
    return gResult;
  }

  //  --- --- --- Threshold
  TGraphErrors *
  get_dcr_threshold_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_dcr_threshold_scan(board, channel, step, {{}}, marker, color);
  }
  TGraphErrors *
  get_dcr_threshold_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker, int color)
  {
    std::string key_word = "DCR_threshold";
    //  Check the DCR curve has been stored
    if (cache_memory.find(board) != cache_memory.end())
    {
      if (cache_memory[board].find(channel) != cache_memory[board].end())
      {
        if (cache_memory[board][channel].find(step) != cache_memory[board][channel].end())
        {
          if (cache_memory[board][channel][step].find(key_word) != cache_memory[board][channel][step].end())
          {
            graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
            return cache_memory[board][channel][step][key_word];
          }
        }
      }
    }
    //  Not found: we need to calculate it
    auto file_name = get_filename(board, channel, step, other_filters);
    if (file_name["dcr-threshold"].empty())
    {
      std::cout << "[WARNING][database::get_dcr_threshold_scan] Run number not found in database" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    cache_memory[board][channel][step][key_word] = ureadout_dcr_get(file_name["dcr-threshold"], "threshold", "dead_rate");
    if (!cache_memory[board][channel][step][key_word])
    {
      std::cout << "[WARNING][database::get_dcr_threshold_scan] DCR threshold scan TGraph error" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      std::cout << "[WARNING][database::get_dcr_threshold_scan] DCR threshold scan TGraph has 0 points" << std::endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
  }

  //  --- --- IV

  //  --- --- --- Vbias
  std::array<float, 2>
  get_iv_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage)
  {
    return database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, step, overvoltage);
  }
  std::vector<std::array<float, 2>>
  get_all_iv_at_overvoltage(std::string board, std::string sensor, std::string step, float overvoltage)
  {
    return database::get_all_values_at_overvoltage<database::get_iv_scan>(board, sensor, step, overvoltage);
  }
  std::array<float, 2>
  get_iv_excess_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage)
  {
    auto current_dcr = database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, step, overvoltage);
    auto new_step_dcr = database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, "NEW", overvoltage);
    return {current_dcr[0] - new_step_dcr[0], sqrt(current_dcr[1] * current_dcr[1] + new_step_dcr[1] * new_step_dcr[1])};
  }

  std::vector<std::array<float, 2>>
  get_all_iv_excess_at_overvoltage(std::string board, std::string sensor, std::string step, float overvoltage)
  {
    std::vector<std::array<float, 2>> result;
    auto all_current_dcr = database::get_all_values_at_overvoltage<database::get_iv_scan>(board, sensor, step, overvoltage);
    auto all_new_step_dcr = database::get_all_values_at_overvoltage<database::get_iv_scan>(board, sensor, "NEW", overvoltage);
    auto iTer = -1;
    for (auto current_dcr : all_current_dcr)
    {
      iTer++;
      auto new_step_dcr = all_new_step_dcr[iTer];
      result.push_back({current_dcr[0] - new_step_dcr[0], sqrt(current_dcr[1] * current_dcr[1] + new_step_dcr[1] * new_step_dcr[1])});
    }
    return result;
  }

  std::array<float, 2>
  get_iv_FOD_at_overvoltage(std::string board, std::string channel, std::string step, float overvoltage, std::string max_damage_step = "TIFPA-IRR1", std::string new_step = "NEW")
  {
    auto current_iv = database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, step, overvoltage);
    auto baselin_iv = database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, new_step, overvoltage);
    auto max_dmg_iv = database::get_value_at_overvoltage<database::get_iv_scan>(board, channel, max_damage_step, overvoltage);
    return measure_fraction_of_damage(current_iv, baselin_iv, max_dmg_iv);
  }

  TCanvas *
  get_history_iv_at_overvoltage(std::string board, std::vector<std::array<std::string, 2>> steps, float overvoltage, bool time_axis = true, float min = 1.e-10, float max = 1.e-5)
  {
    //  Canvas
    TCanvas *c1 = new TCanvas("", "", 1800, 600);
    c1->Divide(3, 1);
    gStyle->SetOptStat(0);
    TLatex *lLatex = new TLatex();

    //  Frame creation
    TH1F *hframe;
    if (!time_axis)
      hframe = new TH1F("hframe", ";;current (A)", steps.size(), 0, steps.size());
    else
      hframe = new TH1F("hframe", ";;current (A)", 5, s_ts(steps[0][1]) - 1000, s_ts(steps[steps.size() - 1][1]) + 1000);
    hframe->SetMaximum(max);
    hframe->SetMinimum(min);

    //  TGraphErrors for all sensors type
    TGraphErrors *gSingleGraphs = new TGraphErrors[12];
    TGraphErrors *gSummaryGraphs = new TGraphErrors[6];

    //  Loop over requested steps
    auto iStep = -1;
    for (auto current_step : steps)
    {
      iStep++;
      auto iSensor = -1;
      for (auto current_sensor : sensors_full)
      {
        auto current_timestamp = s_ts(current_step[1]);
        iSensor++;
        if (!time_axis)
          hframe->GetXaxis()->SetBinLabel(iStep + 1, current_step[0].c_str());
        auto current_overvoltage_values = get_all_iv_at_overvoltage(board, current_sensor, current_step[0], overvoltage);
        auto average_overvoltage_values = measure_average(current_overvoltage_values);
        auto average_overvoltage_valRMS = measure_average(current_overvoltage_values, true);
        auto current_point = gSummaryGraphs[0 + iSensor * 2].GetN();
        time_axis ? gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, current_timestamp, average_overvoltage_values[0]) : gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, 0.5 + iStep, average_overvoltage_values[0]);
        gSummaryGraphs[0 + iSensor * 2].SetPointError(current_point, 0., average_overvoltage_values[1]);
        time_axis ? gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, current_timestamp, average_overvoltage_valRMS[0]) : gSummaryGraphs[0 + iSensor * 2].SetPoint(current_point, 0.5 + iStep, average_overvoltage_valRMS[0]);
        gSummaryGraphs[1 + iSensor * 2].SetPointError(current_point, 0., average_overvoltage_valRMS[1]);
        for (auto single_sensor = 0; single_sensor < 4; single_sensor++)
        {
          current_point = gSingleGraphs[iSensor * 4 + single_sensor].GetN();
          time_axis ? gSingleGraphs[iSensor * 4 + single_sensor].SetPoint(current_point, current_timestamp, current_overvoltage_values[single_sensor][0]) : gSingleGraphs[iSensor * 4 + single_sensor].SetPoint(current_point, 0.5 + iStep, current_overvoltage_values[single_sensor][0]);
          gSingleGraphs[iSensor * 4 + single_sensor].SetPointError(current_point, 0., current_overvoltage_values[single_sensor][1]);
        }
      }
    }
    hframe->GetXaxis()->LabelsOption("v");
    if (time_axis)
    {
      hframe->GetXaxis()->SetTimeDisplay(true);
      hframe->GetXaxis()->SetTimeFormat("%d\/%m\/%Y%F1970-01-01 00:00:00");
    }
    c1->cd(1);
    hframe->Draw();
    gPad->SetLogy();
    lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[0].c_str());
    lLatex->DrawLatexNDC(0.725, 0.9, Form("board #%s", board.c_str()));
    c1->cd(2);
    hframe->Draw();
    gPad->SetLogy();
    lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[1].c_str());
    c1->cd(3);
    hframe->Draw();
    gPad->SetLogy();
    lLatex->DrawLatexNDC(0.175, 0.9, sensors_full[2].c_str());
    for (auto iSns = 0; iSns < 12; iSns++)
    {
      gSingleGraphs[iSns].SetMarkerStyle(24);
      gSingleGraphs[iSns].SetMarkerColor(kGray);
      gSingleGraphs[iSns].SetLineColor(kGray);
      c1->cd(1 + iSns / 4);
      gSingleGraphs[iSns].Draw("samelp");
    }
    for (auto iSns = 0; iSns < 6; iSns++)
    {
      gSummaryGraphs[iSns].SetMarkerStyle(20);
      gSummaryGraphs[iSns].SetMarkerColor(kBlack);
      gSummaryGraphs[iSns].SetLineColor(kBlack);
      c1->cd(1 + iSns / 2);
      gSummaryGraphs[iSns].Draw("samelp");
    }
    return c1;
  }
  //  --- SORT ME ---
  //  --- SORT ME ---
  //  --- SORT ME ---

  std::array<float, 2>
  get_cross_talk(std::string board, std::string channel, std::string step, std::string check_name)
  {
    return measure_cross_talk(get_dcr_threshold_scan(board, channel, step), check_name);
  }
  std::array<float, 2>
  get_sig_amp(std::string board, std::string channel, std::string step, std::string check_name)
  {
    return measure_signal_amp(get_dcr_threshold_scan(board, channel, step), check_name);
  }
  std::unordered_map<std::string, std::string>
  get_filename(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters)
  {
    if (!database_memory.count(board))
    {
      std::cout << "[WARNING][database::get_filename] Board " << board << " not stored in the database" << std::endl;
      return {};
    }
    if (!database_memory[board].count(step))
    {
      std::cout << "[WARNING][database::get_filename] Step " << step << " not stored in the database" << std::endl;
      return {};
    }
    std::string ivfname = "";
    std::string ivofname = "";
    std::string dcrthrfname = "";
    std::string dcrbiasfname = "";
    if (database_memory[board][step].count("iv-run"))
    {
      auto ivrun = database_memory[board][step]["iv-run"].back();
      auto ivsetup = database_memory[board][step]["iv-setup"].back();
      auto ivmux = database_memory[board][step]["iv-mux"].back();
      auto temperature = database_memory[board][step]["temp"].back();

      ivfname = basedir + "/" + ivrun + "/" + ivsetup + "/iv/HAMA3_sn0_mux" + ivmux + "/HAMA3_sn0_" + temperature + "K_" + channel + ".ivscan.csv";
      ivofname = basedir + "/" + ivrun + "/" + ivsetup + "/iv/HAMA3_sn0_mux" + ivmux + "/HAMA3_sn0_" + temperature + "K_OPEN-" + channel + ".ivscan.csv";
    }
    if (database_memory[board][step].count("dcr-run"))
    {
      auto dcrrun = database_memory[board][step]["dcr-run"].back();
      auto dcrsetup = database_memory[board][step]["dcr-setup"].back();
      auto dcrchip = database_memory[board][step]["dcr-chip"].back();
      dcrthrfname = basedir + "/" + dcrrun + "/" + dcrsetup + "/dcr/HAMA3-chip" + dcrchip + "/rate/threshold_scan/chip" + dcrchip + "-" + channel + ".ureadout_dcr_scan.tree.root";
      dcrbiasfname = basedir + "/" + dcrrun + "/" + dcrsetup + "/dcr/HAMA3-chip" + dcrchip + "/rate/vbias_scan/chip" + dcrchip + "-" + channel + ".ureadout_dcr_scan.tree.root";
    }
    return {{"iv", ivfname}, {"iv-open", ivofname}, {"dcr-threshold", dcrthrfname}, {"dcr-vbias", dcrbiasfname}};
  }
  std::string
  get_database_info(std::string board, std::string step, std::string info)
  {
    if (!database_memory.count(board))
    {
      std::cout << "[WARNING][database::get_database_info] Board " << board << " is not stored in the database" << std::endl;
      return "";
    }
    if (!database_memory[board].count(step))
    {
      std::cout << "[WARNING][database::get_database_info] For board " << board << ", step  " << step << " is not stored in the database" << std::endl;
      return "";
    }
    if (!database_memory[board][step].count(info))
    {
      std::cout << "[WARNING][database::get_database_info] For board " << board << ", step  " << step << ", " << info << " is not stored in the database" << std::endl;
      return "";
    }
    return database_memory[board][step][info].back();
  }
  std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>
  get_board_history(std::string board)
  {
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> null_rslt;
    if (!database_memory.count(board))
    {
      std::cout << "[WARNING][database::get_board_history] Board " << board << " is not stored in the database" << std::endl;
      return null_rslt;
    }
    return database_memory[board];
  }
  std::unordered_map<std::string, std::vector<std::string>>
  get_board_state_infos(std::string board, std::string step)
  {
    std::unordered_map<std::string, std::vector<std::string>> null_rslt;
    if (!database_memory.count(board))
    {
      std::cout << "[WARNING][database::get_board_state_infos] Board " << board << " is not stored in the database" << std::endl;
      return null_rslt;
    }
    if (!database_memory[board].count(step))
    {
      std::cout << "[WARNING][database::get_board_state_infos] For board " << board << ", step  " << step << " is not stored in the database" << std::endl;
      return null_rslt;
    }
    return database_memory[board][step];
  }
  std::vector<std::string>
  get_board_state_info_history(std::string board, std::string step, std::string info)
  {
    std::vector<std::string> null_rslt;
    if (!database_memory.count(board))
    {
      std::cout << "[WARNING][database::get_board_state_infos] Board " << board << " is not stored in the database" << std::endl;
      return null_rslt;
    }
    if (!database_memory[board].count(step))
    {
      std::cout << "[WARNING][database::get_board_state_infos] For board " << board << ", step  " << step << " is not stored in the database" << std::endl;
      return null_rslt;
    }
    return database_memory[board][step][info];
  }
  //  --- Quality assurance
  void
  show_database()
  {
    std::cout << "[INFO][database::show_database] Starting print all database entries" << std::endl;
    for (auto [current_board, all_statuses] : database_memory)
      for (auto [current_status, all_informations] : all_statuses)
        for (auto [current_information, all_values] : all_informations)
          for (auto current_value : all_values)
            std::cout << "[INFO][database::show_database] Board " << current_board << " - Status " << current_status << " - Info " << current_information << " - Value " << current_value << std::endl;
  }
  void
  check_all_plots_loaded()
  {
    for (auto [current_board, available_statuses] : board_to_status_list)
    {
      for (auto current_status : available_statuses)
      {
        for (auto current_channel : all_channels)
        {
          if (!get_iv_scan(current_board, current_channel, current_status))
            std::cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] Missing IV!" << std::endl;
          else if (!check_iv_goodness(get_iv_scan(current_board, current_channel, current_status)))
            std::cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] IV found, measurement failed" << std::endl;
          if (!get_dcr_vbias_scan(current_board, current_channel, current_status))
            std::cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] Missing DCR scan!" << std::endl;
        }
      }
    }
  }
  bool
  check_iv_goodness(TGraphErrors *gTarget)
  {
    gTarget->Sort();
    auto maximum = gTarget->GetY()[gTarget->GetN() - 1];
    auto minimum = gTarget->GetY()[0];
    return (fabs(maximum / minimum)) > 100.;
  }
  //  Calculations
  std::array<std::array<float, 2>, 2>
  measure_cross_talk_and_signal_amp(TGraphErrors *gTarget, std::string plot_name)
  {
    //  Clone the target TGraphErrors for drawing check
    auto new_target = (TGraphErrors *)(gTarget->Clone("new_target"));

    //  --- Beautifying
    new_target->SetMarkerStyle(20);
    new_target->SetMarkerColor(kAzure - 3);

    //  --- Drawing check
    TGraphErrors *check_my_points = new TGraphErrors();
    check_my_points->SetMarkerStyle(24);
    check_my_points->SetMarkerColor(kRed);
    check_my_points->SetMarkerSize(1.2);

    //  Plateau calculation
    std::array<std::array<float, 2>, 2> plateau_values;

    //  --- Start roughly at mid-point of first plateau
    auto first_plateau_tolerance = 0.25;
    auto second_plateau_tolerance = 0.50;
    auto start_point_plateau = 5;
    plateau_values[0][0] = gTarget->GetPointY(start_point_plateau);
    plateau_values[0][1] = gTarget->GetErrorY(start_point_plateau) * gTarget->GetErrorY(start_point_plateau);
    first_plateau_tolerance += (gTarget->GetErrorY(start_point_plateau) / gTarget->GetPointY(start_point_plateau));

    //  --- Add point to check drawing
    check_my_points->SetPoint(0, gTarget->GetPointX(start_point_plateau), gTarget->GetPointY(start_point_plateau));

    //  --- Utility for the mean calculation
    auto cuncurrent_points = 1;

    //  Start looping to the left
    for (auto iPnt = start_point_plateau + 1; iPnt < gTarget->GetN(); iPnt++)
    {
      auto current_x_val = gTarget->GetPointX(iPnt);
      auto current_x_err = gTarget->GetErrorX(iPnt);
      auto current_y_val = gTarget->GetPointY(iPnt);
      auto current_y_err = gTarget->GetErrorY(iPnt);
      //  Stop when the points deviate over tolerance
      if (fabs((current_y_val - plateau_values[0][0] / cuncurrent_points) / (plateau_values[0][0] / cuncurrent_points)) > first_plateau_tolerance)
        break;
      cuncurrent_points++;
      plateau_values[0][0] += current_y_val;
      plateau_values[0][1] += current_y_err * current_y_err;

      //  --- Add point to check drawing
      check_my_points->SetPoint(check_my_points->GetN(), gTarget->GetPointX(iPnt), gTarget->GetPointY(iPnt));
    }
    //  Start looping to the right
    for (auto iPnt = start_point_plateau - 1; iPnt >= 0; iPnt--)
    {
      auto current_y_val = gTarget->GetPointY(iPnt);
      auto current_y_err = gTarget->GetErrorY(iPnt);
      //  Stop when the points deviate over 15%
      if (fabs((current_y_val - plateau_values[0][0] / cuncurrent_points) / (plateau_values[0][0] / cuncurrent_points)) > first_plateau_tolerance)
        break;
      cuncurrent_points++;
      plateau_values[0][0] += current_y_val;
      plateau_values[0][1] += current_y_err * current_y_err;
      //  --- Add point to check drawing
      check_my_points->SetPoint(check_my_points->GetN(), gTarget->GetPointX(iPnt), gTarget->GetPointY(iPnt));
    }

    //  --- Finalise calculation
    plateau_values[0][0] /= cuncurrent_points;
    plateau_values[0][1] = sqrt(plateau_values[0][1]);
    plateau_values[0][1] /= cuncurrent_points;

    //  --- Start roughly at mid-point of first plateau
    start_point_plateau = cuncurrent_points + 5;
    plateau_values[1][0] = gTarget->GetPointY(start_point_plateau);
    plateau_values[1][1] = gTarget->GetErrorY(start_point_plateau) * gTarget->GetErrorY(start_point_plateau);
    second_plateau_tolerance += (gTarget->GetErrorY(start_point_plateau) / gTarget->GetPointY(start_point_plateau));

    //  --- Add point to check drawing
    check_my_points->SetPoint(check_my_points->GetN(), gTarget->GetPointX(start_point_plateau), gTarget->GetPointY(start_point_plateau));

    //  --- Utility for the mean calculation
    cuncurrent_points = 1;

    //  Start looping to the left
    for (auto iPnt = start_point_plateau + 1; iPnt < gTarget->GetN(); iPnt++)
    {
      auto current_y_val = gTarget->GetPointY(iPnt);
      auto current_y_err = gTarget->GetErrorY(iPnt);
      //  Stop when the points deviate over 15%
      if (fabs((current_y_val - plateau_values[1][0] / cuncurrent_points) / (plateau_values[1][0] / cuncurrent_points)) > second_plateau_tolerance)
        break;
      cuncurrent_points++;
      plateau_values[1][0] += current_y_val;
      plateau_values[1][1] += current_y_err * current_y_err;
      //  --- Add point to check drawing
      check_my_points->SetPoint(check_my_points->GetN(), gTarget->GetPointX(iPnt), gTarget->GetPointY(iPnt));
    }
    for (auto iPnt = start_point_plateau - 1; iPnt >= 0; iPnt--)
    {
      auto current_y_val = gTarget->GetPointY(iPnt);
      auto current_y_err = gTarget->GetErrorY(iPnt);
      //  Stop when the points deviate over 15%
      if (fabs((current_y_val - plateau_values[1][0] / cuncurrent_points) / (plateau_values[1][0] / cuncurrent_points)) > second_plateau_tolerance)
        break;
      cuncurrent_points++;
      plateau_values[1][0] += current_y_val;
      plateau_values[1][1] += current_y_err * current_y_err;
      //  --- Add point to check drawing
      check_my_points->SetPoint(check_my_points->GetN(), gTarget->GetPointX(iPnt), gTarget->GetPointY(iPnt));
    }
    //  --- Finalise calculation
    plateau_values[1][0] /= cuncurrent_points;
    plateau_values[1][1] = sqrt(plateau_values[1][1]);
    plateau_values[1][1] /= cuncurrent_points;

    //  Error calculation
    auto error_on_first_plateau = plateau_values[0][1] * plateau_values[0][1] * ((plateau_values[1][0]) / ((plateau_values[0][0] - plateau_values[1][0]) * (plateau_values[0][0] - plateau_values[1][0])));
    auto error_on_second_plateau = plateau_values[1][1] * plateau_values[1][1] * ((plateau_values[0][0]) / ((plateau_values[0][0] - plateau_values[1][0]) * (plateau_values[0][0] - plateau_values[1][0])));

    //  Plot if requested
    if (!plot_name.empty())
    {
      TCanvas *c1 = new TCanvas();
      TLatex *lLatex = new TLatex();
      gPad->SetLogy();
      new_target->Draw("AP");
      check_my_points->Draw("SAME P");
      lLatex->DrawLatexNDC(0.6, 0.8, Form("CT: %.1f#pm%.1f%% ", 100 * plateau_values[1][0] / (plateau_values[0][0] - plateau_values[1][0]), 100 * sqrt(error_on_first_plateau * error_on_first_plateau + error_on_second_plateau * error_on_second_plateau)));
      c1->SaveAs(Form("%s.pdf", plot_name.c_str()));
    }

    //  Return
    std::array<float, 2> cross_talk = {plateau_values[1][0] / (plateau_values[0][0] - plateau_values[1][0]), sqrt(error_on_first_plateau * error_on_first_plateau + error_on_second_plateau * error_on_second_plateau)};
    std::array<float, 2> sig_ampl = {-1., -1.};
    return {cross_talk, sig_ampl};
  }
  std::array<float, 2>
  measure_cross_talk(TGraphErrors *gTarget, std::string plot_name)
  {
    return measure_cross_talk_and_signal_amp(gTarget, plot_name)[0];
  }
  std::array<float, 2>
  measure_signal_amp(TGraphErrors *gTarget, std::string plot_name)
  {
    return measure_cross_talk_and_signal_amp(gTarget, plot_name)[1];
  }

  std::array<float, 2>
  measure_fraction_of_damage(std::array<float, 2> target_step, std::array<float, 2> new_reference, std::array<float, 2> maximum_reference)
  {
    std::array<float, 2> result;
    //  Calculate mean value
    result[0] = (target_step[0] - new_reference[0]) / (maximum_reference[0] - new_reference[0]);
    //  Calulate uncertainty
    //  --- uncertainty related to target_step
    auto target_uncertainty = target_step[1] * target_step[1] * (1. / (maximum_reference[0] - new_reference[0])) * (1. / (maximum_reference[0] - new_reference[0]));
    //  --- uncertainty related to new
    auto new_uncertainty = new_reference[1] * new_reference[1] * ((target_step[0] - maximum_reference[0]) / ((maximum_reference[0] - new_reference[0]) * (maximum_reference[0] - new_reference[0]))) * ((target_step[0] - maximum_reference[0]) / ((maximum_reference[0] - new_reference[0]) * (maximum_reference[0] - new_reference[0])));
    //  --- uncertainty related to maximum
    auto maximum_uncertainty = maximum_reference[1] * maximum_reference[1] * ((new_reference[0] - target_step[0]) / ((maximum_reference[0] - new_reference[0]) * (maximum_reference[0] - new_reference[0]))) * ((new_reference[0] - target_step[0]) / ((maximum_reference[0] - new_reference[0]) * (maximum_reference[0] - new_reference[0])));
    //  --- Total uncertainty
    result[1] = sqrt(target_uncertainty + new_uncertainty + maximum_uncertainty);
    return result;
  }
  std::vector<std::array<float, 2>>
  measure_fraction_of_damage(std::vector<std::array<float, 2>> target_step, std::vector<std::array<float, 2>> new_reference, std::vector<std::array<float, 2>> maximum_reference)
  {
    std::vector<std::array<float, 2>> result;

    return result;
  }

  //  Utilities
  std::vector<TGraphErrors *> make_graph(mkgraph_data_structure data_structure)
  {
    std::vector<TGraphErrors *> result;
    auto maximum_size = 0.;
    for (auto [x_coordinate, y_coordinate_array] : data_structure)
      maximum_size = std::max(maximum_size, 1. * y_coordinate_array.size());
    for (auto _unused = 0; _unused < maximum_size; _unused++)
      result.push_back(new TGraphErrors());
    for (auto [x_coordinate, y_coordinate_array] : data_structure)
    {
      auto iy = -1;
      for (auto y_coordinate : y_coordinate_array)
      {
        iy++;
        auto ix = result[iy]->GetN();
        result[iy]->SetPoint(ix, x_coordinate[0], y_coordinate[0]);
        result[iy]->SetPointError(ix, x_coordinate[1], y_coordinate[1]);
      }
    }
    return result;
  }
  void set_multi_graph(std::vector<TGraphErrors *> &target_graphs, int target_color, int target_marker)
  {
    for (auto current_graph : target_graphs)
    {
      current_graph->SetLineColorAlpha(target_color, 0.2);
      current_graph->SetLineStyle(kDashed);
      current_graph->SetMarkerColorAlpha(target_color, 0.2);
      current_graph->SetMarkerStyle(target_marker);
    }
    target_graphs.at(0)->SetLineColor(target_color);
    target_graphs.at(0)->SetLineStyle(kSolid);
    target_graphs.at(0)->SetLineWidth(2);
    target_graphs.at(0)->SetMarkerColor(target_color);
    target_graphs.at(0)->SetMarkerStyle(target_marker);
    target_graphs.at(0)->SetMarkerSize(2);
    target_graphs.at(1)->SetLineColor(target_color);
    target_graphs.at(1)->SetLineStyle(kSolid);
    target_graphs.at(1)->SetLineWidth(2);
    target_graphs.at(1)->SetMarkerColor(target_color);
    target_graphs.at(1)->SetMarkerStyle(target_marker);
    target_graphs.at(1)->SetMarkerSize(2);
  }

  template <bool use_sqsum = true, bool skip_unfit_skim = false>
  std::tuple<float, float, float>
  measure_average_and_rms(std::vector<std::array<float, 2>> measurements, float rms_tolerance = 5.)
  {
    //  Final result
    std::tuple<float, float, float> result = {0., 0., 0.};
    std::vector<std::array<float, 2>> skimmed_data;

    //  Skim dataset from dangerous values
    for (auto current_measurement : measurements)
    {
      if (skip_unfit_skim)
        break;
      if (std::isnan(current_measurement[0]))
        continue;
      if (std::isinf(current_measurement[0]))
        continue;
      skimmed_data.push_back(current_measurement);
    }
    if (skip_unfit_skim)
      skimmed_data = measurements;

    //  Measure average and error on the average
    for (auto current_measurement : skimmed_data)
    {
      get<0>(result) += current_measurement[0];
      get<1>(result) += use_sqsum ? current_measurement[1] * current_measurement[1] : current_measurement[1];
    }
    get<0>(result) /= skimmed_data.size();
    get<1>(result) = sqrt(get<1>(result)) / skimmed_data.size();

    //  Measure RMS
    for (auto current_measurement : skimmed_data)
      get<2>(result) += (get<0>(result) - current_measurement[0]) * (get<0>(result) - current_measurement[0]);
    get<2>(result) = sqrt(get<2>(result) / skimmed_data.size());

    //  Second skimming for outliers
    std::vector<std::array<float, 2>> re_skimmed_data;
    for (auto current_measurement : skimmed_data)
      if (fabs(current_measurement[0] - get<0>(result)) < rms_tolerance * get<2>(result))
        re_skimmed_data.push_back(current_measurement);
    if (re_skimmed_data.size() != skimmed_data.size())
      return measure_average_and_rms<true, true>(re_skimmed_data);

    return result;
  }
  //  --- ---
  std::array<float, 2>
  measure_average(std::vector<std::array<float, 2>> measurements, bool use_rms)
  {
    std::array<float, 2> result;
    auto avg = measure_average_and_rms(measurements);
    return {get<0>(avg), use_rms ? get<2>(avg) : get<1>(avg)};
  }
  template <bool use_rms = false>
  std::array<float, 2>
  measure_average(std::vector<std::array<float, 2>> measurements)
  {
    std::array<float, 2> result;
    auto avg = measure_average_and_rms(measurements);
    return {get<0>(avg), use_rms ? get<2>(avg) : get<1>(avg)};
  }
  void
  set_as_main_line(TGraph *gTarget, int marker, int color)
  {
    gTarget->SetLineColor(color);
    gTarget->SetLineStyle(kSolid);
    gTarget->SetLineWidth(2);
    gTarget->SetFillStyle(0);
    gTarget->SetMarkerColor(color);
    gTarget->SetMarkerStyle(marker);
    gTarget->SetMarkerSize(2);
  }
  void
  set_as_main_line(TGraphErrors *gTarget, int marker, int color)
  {
    gTarget->SetLineColor(color);
    gTarget->SetLineStyle(kSolid);
    gTarget->SetLineWidth(2);
    gTarget->SetFillStyle(0);
    gTarget->SetMarkerColor(color);
    gTarget->SetMarkerStyle(marker);
    gTarget->SetMarkerSize(2);
  }
  void
  set_as_main_line(TH1 *hTarget, int marker, int color)
  {
    hTarget->SetLineColor(color);
    hTarget->SetLineStyle(kSolid);
    hTarget->SetLineWidth(2);
    hTarget->SetMarkerColor(color);
    hTarget->SetMarkerStyle(marker);
    hTarget->SetMarkerSize(2);
    hTarget->SetFillStyle(0);
    hTarget->SetFillColorAlpha(0., 0.);
    hTarget->GetXaxis()->SetBinLabel(1, all_sensors[0].c_str());
    hTarget->GetXaxis()->SetBinLabel(2, all_sensors[1].c_str());
    hTarget->GetXaxis()->SetBinLabel(3, all_sensors[2].c_str());
  }

  //  - F -
  template <typename ArgType,
            std::array<float, 2> (*current_value_getter)(std::string, std::string, std::string, ArgType)>
  std::vector<TGraphErrors *>
  get_steps_history(std::string board, std::string sensor, std::vector<std::string> step_list, ArgType target_fourth, int marker = kFullCircle, int color = kBlue, int sec_marker = kCircle, int sec_color = kGray)
  {
    std::vector<TGraphErrors *> result;
    bool req_all_sensors = true;
    if (sensor_to_channels.find(sensor) != sensor_to_channels.end())
      req_all_sensors = false;
    for (auto iter = 0; iter < (req_all_sensors ? 14 : 6); iter++)
      result.push_back(new TGraphErrors());
    for (auto iter = 0; iter < 2; iter++)
    {
      result[iter]->SetMarkerStyle(marker);
      result[iter]->SetMarkerColor(color);
    }
    for (auto iter = 2; iter < (req_all_sensors ? 14 : 6); iter++)
    {
      result[iter]->SetMarkerStyle(sec_marker);
      result[iter]->SetMarkerColor(sec_color);
    }
    auto i_step = -1;
    for (auto current_step : step_list)
    {
      i_step++;
      std::vector<std::array<float, 2>> current_step_channel_values;
      auto i_sensor = -1;
      if (!req_all_sensors)
      {
        for (auto current_channel : sensor_to_channels[sensor])
        {
          i_sensor++;
          auto current_value = current_value_getter(board, current_channel, current_step, target_fourth);
          current_step_channel_values.push_back(current_value);
          result[2 + i_sensor]->SetPoint(i_step, 0.5 + i_step, current_value[0]);
          result[2 + i_sensor]->SetPointError(i_step, 0, current_value[1]);
        }
      }
      else
      {
        for (auto current_channel : all_channels)
        {
          i_sensor++;
          auto current_value = current_value_getter(board, current_channel, current_step, target_fourth);
          cout << "current_value[0]: " << current_value[0] << endl;
          current_step_channel_values.push_back(current_value);
          result[2 + i_sensor]->SetPoint(i_step, 0.5 + i_step, current_value[0]);
          result[2 + i_sensor]->SetPointError(i_step, 0, current_value[1]);
        }
      }
      auto current_avg = measure_average(current_step_channel_values, false);
      auto current_avg_rms = measure_average(current_step_channel_values, true);
      result[0]->SetPoint(i_step, 0.5 + i_step, current_avg[0]);
      result[0]->SetPointError(i_step, 0, current_avg[1]);
      result[1]->SetPoint(i_step, 0.5 + i_step, current_avg_rms[0]);
      result[1]->SetPointError(i_step, 0, current_avg_rms[1]);
    }
    return result;
  }

  std::vector<TGraphErrors *>
  get_steps_history_iv(std::string board, std::string sensor, std::vector<std::string> step_list, float overvoltage, int marker = kFullCircle, int color = kBlue, int sec_marker = kCircle, int sec_color = kGray)
  {
    return get_steps_history<float, get_iv_at_overvoltage>(board, sensor, step_list, overvoltage, marker, color, sec_marker, sec_color);
  }

  std::vector<TGraphErrors *>
  get_steps_history_gain(std::string board, std::string sensor, std::vector<std::string> step_list, float overvoltage, int marker = kFullCircle, int color = kBlue, int sec_marker = kCircle, int sec_color = kGray)
  {
    return get_steps_history<float, get_gain_at_overvoltage>(board, sensor, step_list, overvoltage, marker, color, sec_marker, sec_color);
  }

  std::vector<TGraphErrors *>
  get_steps_history_CT(std::string board, std::string sensor, std::vector<std::string> step_list, std::string save_dir, int marker = kFullCircle, int color = kBlue, int sec_marker = kCircle, int sec_color = kGray)
  {
    return get_steps_history<std::string, get_cross_talk>(board, sensor, step_list, save_dir, marker, color, sec_marker, sec_color);
  }

  std::array<float, 2>
  get_iv_FOD_at_overvoltage_std_TIFPA(std::string board, std::string channel, std::string step, float overvoltage)
  {
    return get_iv_FOD_at_overvoltage(board, channel, step, overvoltage, "TIFPA-IRR1", "NEW");
  }

  std::vector<TGraphErrors *>
  get_steps_history_FOD_std_TIFPA(std::string board, std::string sensor, std::vector<std::string> step_list, float overvoltage, int marker = kFullCircle, int color = kBlue, int sec_marker = kCircle, int sec_color = kGray)
  {
    return get_steps_history<float, database::get_iv_FOD_at_overvoltage_std_TIFPA>(board, sensor, step_list, overvoltage, marker, color, sec_marker, sec_color);
  }

  //  - F -
  template <typename ArgType,
            std::array<float, 2> (*current_value_getter)(std::string, std::string, std::string, ArgType)>
  std::vector<TGraphErrors *>
  get_general_TGraphs(std::string sensor, std::vector<std::pair<std::string, std::vector<std::string>>> step_list_w_boards, ArgType target_fourth, int marker = kFullCircle, int color = kBlue)
  {
    std::vector<TGraphErrors *> result;
    bool req_all_sensors = true;
    if (sensor_to_channels.find(sensor) != sensor_to_channels.end())
      req_all_sensors = false;
    for (auto iter = 0; iter < 2; iter++)
    {
      result.push_back(new TGraphErrors());
      result[iter]->SetMarkerStyle(marker);
      result[iter]->SetMarkerColor(color);
    }
    auto i_step = -1;
    for (auto [current_step, all_boards] : step_list_w_boards)
    {
      i_step++;
      std::vector<std::array<float, 2>> current_step_channel_values;
      for (auto current_board : all_boards)
      {
        auto i_sensor = -1;
        if (!req_all_sensors)
        {
          for (auto current_channel : sensor_to_channels[sensor])
          {
            i_sensor++;
            auto current_value = current_value_getter(current_board, current_channel, current_step, target_fourth);
            if (current_value[0] < 0)
              continue;
            current_step_channel_values.push_back(current_value);
          }
        }
        else
        {
          for (auto current_channel : all_channels)
          {
            i_sensor++;
            auto current_value = current_value_getter(current_board, current_channel, current_step, target_fourth);
            if (current_value[0] < 0)
              continue;
            current_step_channel_values.push_back(current_value);
          }
        }
      }
      auto current_avg = measure_average(current_step_channel_values, false);
      auto current_avg_rms = measure_average(current_step_channel_values, true);
      result[0]->SetPoint(i_step, 0.5 + i_step, current_avg[0]);
      result[0]->SetPointError(i_step, 0, current_avg[1]);
      result[1]->SetPoint(i_step, 0.5 + i_step, current_avg_rms[0]);
      result[1]->SetPointError(i_step, 0, current_avg_rms[1]);
    }
    return result;
  }

  std::vector<TGraphErrors *>
  get_general_TGraphs_iv(std::string sensor, std::vector<std::pair<std::string, std::vector<std::string>>> step_list_w_boards, float overvoltage, int marker = kFullCircle, int color = kBlue)
  {
    return get_general_TGraphs<float, get_iv_at_overvoltage>(sensor, step_list_w_boards, overvoltage, marker, color);
  }

}
