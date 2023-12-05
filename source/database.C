#pragma once

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
  std::vector<std::string> sensors_full = {"HPK S13360-3050VS", "HPK S13361-3075VS", "HPK S14161-3050HS"};
  //
  //  Board oriented quantities
  //  --- All available boards
  std::vector<std::string> all_boards = {
      "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
      "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
      "31", "32", "33", "34"};
  std::map<std::string, std::vector<std::string>> standard_status_to_boards =
      {{"NEW", all_boards},
       {"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}},
       {"LNL-IRR1", {}}};
  //  --- Board to status
  //  --- --- { Board, Status list }
  std::map<std::string, std::vector<std::string>> board_to_status_list; // TBI: Fill it reading fro the database
  //
  //  Channel oriented quantities
  //  --- All available channels
  std::vector<std::string> all_channels = {
      "A1", "A2", "A3", "A4",
      "B1", "B2", "B3", "B4",
      "C1", "C2", "C3", "C4"};
  //  --- Channel to sensor
  //  --- --- { Channels, Sensor }
  std::map<std::string, std::string> channel_to_sensor = {
      {"A1", "Hamamatsu S13361-3050"},
      {"A2", "Hamamatsu S13361-3050"},
      {"A3", "Hamamatsu S13361-3050"},
      {"A4", "Hamamatsu S13361-3050"},
      {"B1", "Hamamatsu S13361-3075"},
      {"B2", "Hamamatsu S13361-3075"},
      {"B3", "Hamamatsu S13361-3075"},
      {"B4", "Hamamatsu S13361-3075"},
      {"C1", "Hamamatsu S14161-3050"},
      {"C2", "Hamamatsu S14161-3050"},
      {"C3", "Hamamatsu S14161-3050"},
      {"C4", "Hamamatsu S14161-3050"}};
  std::map<std::string, std::string> channel_to_sensor_short = {
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
  std::map<std::string, std::vector<std::string>> sensor_to_channels = {
      {"Hamamatsu S13361-3050", {"A1", "A2", "A3", "A4"}},
      {"Hamamatsu S13361-3075", {"B1", "B2", "B3", "B4"}},
      {"Hamamatsu S14161-3050", {"C1", "C2", "C3", "C4"}}};
  std::map<std::string, std::vector<std::string>> sensor_short_to_channels = {
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
  //  --- Status to annealing time
  //  --- --- { Status, Time (min) }
  std::map<std::string, double> status_to_annealing_time = {
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
  //  --- --- { Status, Temperature (K) }
  std::map<std::string, double> status_to_temperature = {
      {"NEW", 243.15},
      {"TIFPA-IRR1", 243.15},
      {"TEMP313", 313.15},
      {"TEMP303", 303.15},
      {"TEMP293", 293.15},
      {"TEMP283", 283.15},
      {"TEMP278", 278.15},
      {"TEMP273", 273.15},
      {"TEMP263", 263.15},
      {"TEMP253", 253.15}};
  //
  //  Sensor oriented quantities
  std::map<std::string, int> sensor_to_color = {
      {"Hamamatsu S13361-3050", kRed},
      {"Hamamatsu S13361-3075", kBlue},
      {"Hamamatsu S14161-3050", kYellow + 2}};
  std::map<std::string, int> sensor_short_to_color = {
      {"S13361_3050", kRed},
      {"S13361_3075", kBlue},
      {"S14161_3050", kYellow + 2}};
  //
  //  --- Utility data structures ---
  //  --- Data structure in database reference
  std::vector<std::string> fields = {"run", "quality", "step", "setup", "iv-mux-1", "iv-mux-2", "dcr-chip-2", "dcr-chip-3"};
  std::vector<std::string> criteria = {"run", "quality", "step", "setup", "iv-mux-1", "iv-mux-2", "dcr-chip-2", "dcr-chip-3", "temp", "ann-temp", "ann-time"};
  //  --- Global variables
  std::string basedir = ".";
  //  --- Board, Channel, Status, Curve (IV, DCR, Gain)
  std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, TGraphErrors *>>>> cache_memory;
  //  --- Board, Status, Information (set-up, mux, dcr-chip )
  std::map<std::string, std::map<std::string, std::map<std::string, std::vector<std::string>>>> database_memory;
  //
  //  --- Declarations ---
  //  --- I/O
  void read_database(std::string fname);
  //  --- Getters
  TGraphErrors *get_iv_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_vbias_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_threshold_scan(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_gain(std::string board, std::string channel, std::string step, int marker = 1, int color = 1);
  TGraphErrors *get_iv_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_vbias_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_dcr_threshold_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  TGraphErrors *get_gain(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker = 1, int color = 1);
  std::map<std::string, std::string> get_filename(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters = {{}});
  std::string get_database_info(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters);
  //   --- Quality assurance
  void check_all_plots_loaded();
  bool check_iv_goodness(TGraphErrors *gTarget);
  //
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
      std::map<std::string, std::string> data_by_field;
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
  TGraphErrors *
  get_iv_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_iv_scan(board, channel, step, {{}}, marker, color);
  }
  TGraphErrors *
  get_dcr_vbias_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_dcr_vbias_scan(board, channel, step, {{}}, marker, color);
  }
  TGraphErrors *
  get_dcr_threshold_scan(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_dcr_threshold_scan(board, channel, step, {{}}, marker, color);
  }
  TGraphErrors *
  get_gain(std::string board, std::string channel, std::string step, int marker, int color)
  {
    return get_gain(board, channel, step, {{}}, marker, color);
  }
  TGraphErrors *
  get_iv_scan(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters, int marker, int color)
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
      cout << "[WARNING][database::get_iv_scan] Run number not found in database" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  Make iv scan
    cache_memory[board][channel][step][key_word] = make_iv_scan(file_name["iv"], file_name["iv-open"]);
    //  If something went wrong, return nullptr
    if (!cache_memory[board][channel][step][key_word])
    {
      cout << "[WARNING][database::get_iv_scan] IV scan TGraph error" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  If TGraphErrors has 0 points, return nullptr
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      cout << "[WARNING][database::get_iv_scan] IV scan TGraph has 0 points" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    //  Set requested style
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
  }
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
      cout << "[WARNING][database::get_dcr_vbias_scan] Run number not found in database" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    cache_memory[board][channel][step][key_word] = ureadout_dcr_get(file_name["dcr-vbias"], "bias_voltage", "dead_rate");
    if (!cache_memory[board][channel][step][key_word])
    {
      cout << "[WARNING][database::get_dcr_vbias_scan] DCR vbias scan TGraph error" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      cout << "[WARNING][database::get_dcr_vbias_scan] DCR vbias scan TGraph has 0 points" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
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
      cout << "[WARNING][database::get_dcr_threshold_scan] Run number not found in database" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    cache_memory[board][channel][step][key_word] = ureadout_dcr_get(file_name["dcr-threshold"], "threshold", "dead_rate");
    if (!cache_memory[board][channel][step][key_word])
    {
      cout << "[WARNING][database::get_dcr_threshold_scan] DCR threshold scan TGraph error" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    if (cache_memory[board][channel][step][key_word]->GetN() == 0)
    {
      cout << "[WARNING][database::get_dcr_threshold_scan] DCR threshold scan TGraph has 0 points" << endl;
      cache_memory[board][channel][step][key_word] = nullptr;
      return cache_memory[board][channel][step][key_word];
    }
    graphutils::set_style(cache_memory[board][channel][step][key_word], marker, color);
    //  Return result
    return cache_memory[board][channel][step][key_word];
  }
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
  std::map<std::string, std::string>
  get_filename(std::string board, std::string channel, std::string step, std::vector<std::pair<std::string, std::string>> other_filters)
  {
    if (!database_memory.count(board))
    {
      cout << "[WARNING][database::get_filename] Board " << board << " not stored in the database" << endl;
      return {};
    }
    if (!database_memory[board].count(step))
    {
      cout << "[WARNING][database::get_filename] Step " << step << " not stored in the database" << endl;
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
      cout << "[WARNING][database::get_database_info] Board " << board << " is not stored in the database" << endl;
      return "";
    }
    if (!database_memory[board].count(step))
    {
      cout << "[WARNING][database::get_database_info] For board " << board << ", step  " << step << " is not stored in the database" << endl;
      return "";
    }
    if (!database_memory[board][step].count(info))
    {
      cout << "[WARNING][database::get_database_info] For board " << board << ", step  " << step << ", " << info << " is not stored in the database" << endl;
      return "";
    }
    return database_memory[board][step][info].back();
  }
  std::map<std::string, std::map<std::string, std::vector<std::string>>>
  get_board_history(std::string board)
  {
    std::map<std::string, std::map<std::string, std::vector<std::string>>> null_rslt;
    if (!database_memory.count(board))
    {
      cout << "[WARNING][database::get_board_history] Board " << board << " is not stored in the database" << endl;
      return null_rslt;
    }
    return database_memory[board];
  }
  std::map<std::string, std::vector<std::string>>
  get_board_state_infos(std::string board, std::string step)
  {
    std::map<std::string, std::vector<std::string>> null_rslt;
    if (!database_memory.count(board))
    {
      cout << "[WARNING][database::get_board_state_infos] Board " << board << " is not stored in the database" << endl;
      return null_rslt;
    }
    if (!database_memory[board].count(step))
    {
      cout << "[WARNING][database::get_board_state_infos] For board " << board << ", step  " << step << " is not stored in the database" << endl;
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
      cout << "[WARNING][database::get_board_state_infos] Board " << board << " is not stored in the database" << endl;
      return null_rslt;
    }
    if (!database_memory[board].count(step))
    {
      cout << "[WARNING][database::get_board_state_infos] For board " << board << ", step  " << step << " is not stored in the database" << endl;
      return null_rslt;
    }
    return database_memory[board][step][info];
  }
  //  --- Quality assurance
  void
  show_database()
  {
    cout << "[INFO][database::show_database] Starting print all database entries" << endl;
    for (auto [current_board, all_statuses] : database_memory)
      for (auto [current_status, all_informations] : all_statuses)
        for (auto [current_information, all_values] : all_informations)
          for (auto current_value : all_values)
            cout << "[INFO][database::show_database] Board " << current_board << " - Status " << current_status << " - Info " << current_information << " - Value " << current_value << endl;
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
            cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] Missing IV!" << endl;
          else if (!check_iv_goodness(get_iv_scan(current_board, current_channel, current_status)))
            cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] IV found, measurement failed" << endl;
          if (!get_dcr_vbias_scan(current_board, current_channel, current_status))
            cout << "[QA::WARNING][database::check_all_plots_loaded][Board:" << current_board.c_str() << " Channel:" << current_channel.c_str() << " Status:" << current_status.c_str() << "] Missing DCR scan!" << endl;
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
}
