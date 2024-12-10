#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>
#include <ctime>
#include <cmath>
#include <array>
#include <algorithm>

// Converts a timestamp string to seconds with fractions
double convertTimestampToSecondsWithFraction(const std::string &timestamp)
{
    // Validate format: YYYYMMDD-HHMMSS.SSS
    if (timestamp.size() != 19 || timestamp[8] != '-' || timestamp[15] != '.')
    {
        throw std::invalid_argument("Invalid timestamp format");
    }

    // Extract timestamp components
    int year = std::stoi(timestamp.substr(0, 4));
    int month = std::stoi(timestamp.substr(4, 2));
    int day = std::stoi(timestamp.substr(6, 2));
    int hour = std::stoi(timestamp.substr(9, 2));
    int minute = std::stoi(timestamp.substr(11, 2));
    int second = std::stoi(timestamp.substr(13, 2));
    int millisecond = std::stoi(timestamp.substr(16, 3));

    // Create a tm struct
    std::tm tmStruct = {};
    tmStruct.tm_year = year - 1900; // tm_year is years since 1900
    tmStruct.tm_mon = month - 1;    // tm_mon is 0-based
    tmStruct.tm_mday = day;
    tmStruct.tm_hour = hour;
    tmStruct.tm_min = minute;
    tmStruct.tm_sec = second;

    // Convert to time_t
    std::time_t timeInSeconds = std::mktime(&tmStruct);
    if (timeInSeconds == -1)
    {
        throw std::runtime_error("Failed to convert time");
    }

    // Add milliseconds as fractions of a second
    return static_cast<double>(timeInSeconds) + (millisecond / 1000.0);
}

// Converts a timestamp string to seconds with fractions
long int convertTimestampToDayTime(const std::string &timestamp)
{
    // Validate format: YYYYMMDD-HHMMSS.SSS
    if (timestamp.size() != 19 || timestamp[8] != '-' || timestamp[15] != '.')
    {
        throw std::invalid_argument("Invalid timestamp format");
    }

    // Extract timestamp components
    int hour = std::stoi(timestamp.substr(9, 2));
    int minute = std::stoi(timestamp.substr(11, 2));
    int second = std::stoi(timestamp.substr(13, 2));

    // Add milliseconds as fractions of a second
    return hour * 60 * 60 + minute * 60 + second;
}

// Converts a timestamp string to seconds with fractions
long int convertTimestampToDay(const std::string &timestamp)
{
    // Validate format: YYYYMMDD-HHMMSS.SSS
    if (timestamp.size() != 19 || timestamp[8] != '-' || timestamp[15] != '.')
    {
        throw std::invalid_argument("Invalid timestamp format");
    }

    // Extract timestamp components
    int year = std::stoi(timestamp.substr(0, 4));
    int month = std::stoi(timestamp.substr(4, 2));
    int day = std::stoi(timestamp.substr(6, 2));

    // Create a tm struct
    std::tm tmStruct = {};
    tmStruct.tm_year = year - 1900; // tm_year is years since 1900
    tmStruct.tm_mon = month - 1;    // tm_mon is 0-based
    tmStruct.tm_mday = day;
    tmStruct.tm_hour = 0;
    tmStruct.tm_min = 0;
    tmStruct.tm_sec = 0;

    // Convert to time_t
    std::time_t timeInSeconds = std::mktime(&tmStruct);
    if (timeInSeconds == -1)
    {
        throw std::runtime_error("Failed to convert time");
    }

    // Add milliseconds as fractions of a second
    return static_cast<double>(timeInSeconds);
}

// Reads a file and appends its content to a map
void readFileToMap(const std::string &filename, std::map<std::string, std::vector<std::string>> &dataMap)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    // Read header line
    std::string headerLine;
    if (!std::getline(file, headerLine))
    {
        throw std::runtime_error("Error: File is empty or missing header row.");
    }

    // Parse column titles
    std::istringstream headerStream(headerLine);
    std::vector<std::string> columnTitles;
    std::string title;
    while (headerStream >> title)
    {
        columnTitles.push_back(title);
        if (dataMap.find(title) == dataMap.end())
        {
            dataMap[title] = {}; // Initialize vector if not already present
        }
    }

    // Read remaining lines
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string value;
        for (size_t i = 0; i < columnTitles.size(); ++i)
        {
            if (!(lineStream >> value))
            {
                throw std::runtime_error("Error: Mismatched data and header columns.");
            }
            dataMap[columnTitles[i]].push_back(value);
        }
    }

    file.close();
}

std::vector<int> get_all_acceptable_events(std::map<std::string, std::vector<std::string>> input_list)
{
    std::vector<int> list_of_target_events;
    for (int i_pos = 0; i_pos < input_list["#time"].size(); i_pos++)
    {
        if (!stoi(input_list["check"][i_pos]))
        {
            list_of_target_events.push_back(i_pos);
            continue;
        }
        cout << " --- START OF CHECK i_pos: " << i_pos << " --- " << endl;
        cout << " --- ";
        auto check_fill = true;
        for (int i_fifo = 0; i_fifo < 24; i_fifo++)
            cout << " " << input_list[Form("fifo%i", i_fifo)][i_pos] << " ";
        cout << endl;
        for (int i_fifo = 0; i_fifo < 24; i_fifo += 4)
        {
            auto fifo0 = stoi(input_list[Form("fifo%i", i_fifo)][i_pos]);
            auto fifo1 = stoi(input_list[Form("fifo%i", i_fifo + 1)][i_pos]);
            auto fifo2 = stoi(input_list[Form("fifo%i", i_fifo + 2)][i_pos]);
            auto fifo3 = stoi(input_list[Form("fifo%i", i_fifo + 3)][i_pos]);
            cout << " - " << fifo0 << " - " << fifo1 << " - " << fifo2 << " - " << fifo3 << " - " << endl;
            if (fifo0 != fifo1)
            {
                check_fill = false;
                break;
            }
            if (fifo0 != fifo2)
            {
                check_fill = false;
                break;
            }
            if (fifo0 != fifo3)
            {
                check_fill = false;
                break;
            }
        }
        cout << " --- check_fill: " << check_fill << endl;
        if (check_fill)
            list_of_target_events.push_back(i_pos);
        cout << " --- END OF CHECK --- " << endl;
        if (i_pos > 248346)
            break;
    }
    return list_of_target_events;
}

std::vector<std::string> conf_1 = {
    "test/rollover_check_193.24.11.18.16.15.txt",
    "test/rollover_check_193.24.11.19.09.38.txt",
    "test/rollover_check_193.24.11.20.09.26.txt",
    "test/rollover_check_193.24.11.20.16.40.txt",
    "test/rollover_check_193.24.11.21.00.25.txt",
    "test/rollover_check_193.24.11.21.07.45.txt"};

std::vector<std::string> conf_2 = {
    "test/rollover_check_193.24.11.21.09.39.txt",
    "test/rollover_check_193.24.11.21.18.22.txt",
    "test/rollover_check_193.24.11.22.00.40.txt",
    "test/rollover_check_193.24.11.22.08.00.txt",
    "test/rollover_check_193.24.11.22.17.30.txt",
    "test/rollover_check_193.24.11.23.15.25.txt",
    "test/rollover_check_193.24.11.24.11.05.txt",
    "test/rollover_check_193.24.11.25.08.10.txt",
    "test/rollover_check_193.24.11.25.17.08.txt",
    "test/rollover_check_193.24.11.26.08.25.txt",
    "test/rollover_check_193.24.11.26.16.17.txt",
    "test/rollover_check_193.24.11.26.18.38.txt",
    "test/rollover_check_193.24.11.27.09.15.txt",
    "test/rollover_check_193.24.12.02.18.10.txt",
    "test/rollover_check_193.24.12.03.09.50.txt",
    "test/rollover_check_193.24.12.03.09.51.txt",
    "test/rollover_check_193.24.12.03.10.01.txt",
    "test/rollover_check_193.24.12.03.17.44.txt",
    "test/rollover_check_193.24.12.04.02.05.txt",
    "test/rollover_check_193.24.12.04.10.31.txt",
    "test/rollover_check_193.24.12.04.18.55.txt",
    "test/rollover_check_193.24.12.05.10.54.txt",
    "test/rollover_check_193.24.12.05.23.12.txt",
    "test/rollover_check_193.24.12.06.09.45.txt",
    "test/rollover_check_193.24.12.07.09.30.txt",
    "test/rollover_check_193.24.12.07.22.55.txt",
    "test/rollover_check_193.24.12.09.11.25.txt",
    "test/rollover_check_193.24.12.09.20.05.txt",
    "test/rollover_check_193.24.12.10.09.45.txt"};

// Main logic to process data and create graphs
void makegraph()
{
    //  Target files
    std::vector<std::string> fileList = conf_2;

    // Map to store data
    std::map<std::string, std::vector<std::string>> list_of_values;

    // Read all files
    for (const auto &fileName : fileList)
    {
        try
        {
            readFileToMap(fileName, list_of_values);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing file " << fileName << ": " << e.what() << std::endl;
            continue;
        }
    }
    auto good_events = get_all_acceptable_events(list_of_values);
    cout << "good_events.size(): " << good_events.size() << endl;
    return;

    //  Data summary graphs
    //  --- [0-23] fifo [24] check value
    std::vector<TGraph *> fifo_graph;
    std::string dataformat = "%H:%M%F1970-01-01 00:00:00";
    for (int i_fifo = 0; i_fifo < 24; i_fifo++)
    {
        fifo_graph.push_back(new TGraph());
        fifo_graph[i_fifo]->SetName(Form("fifo%i", i_fifo));
        fifo_graph[i_fifo]->GetXaxis()->SetTimeDisplay(true);
        fifo_graph[i_fifo]->GetXaxis()->SetTimeFormat(dataformat.c_str());
    }
    fifo_graph.push_back(new TGraph());
    fifo_graph[24]->SetName("check");
    fifo_graph[24]->GetXaxis()->SetTimeDisplay(true);
    fifo_graph[24]->GetXaxis()->SetTimeFormat(dataformat.c_str());

    auto n_div = 4;

    //  Check histograms
    TH1F *h_mismatch_occurence = new TH1F("h_mismatch_occurence", ";fifos involved;mismatch count", 25, -0.5, 24.5);
    TH1F *h_mismatch_occurence_chips = new TH1F("h_mismatch_occurence_chips", ";# chips involved w.r.t. chip 0;mismatch count", 6, -0.5, 5.5);
    TH1F *h_mismatch_wrt_fifo0 = new TH1F("h_mismatch_wrt_fifo0", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5);
    TH1F *h_mismatch_wrt_fifo0_norm = new TH1F("h_mismatch_wrt_fifo0_norm", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5);
    TH1F *h_mismatch_wrt_fifo0_rat = new TH1F("h_mismatch_wrt_fifo0_rat", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5);
    TH2F *h2_mismatch_wrt_fifo0 = new TH2F("h2_mismatch_wrt_fifo0", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5, n_div, 0, n_div);
    TH2F *h2_mismatch_wrt_fifo0_norm = new TH2F("h2_mismatch_wrt_fifo0_norm", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5, n_div, 0, n_div);
    TH2F *h2_mismatch_wrt_fifo0_rat = new TH2F("h2_mismatch_wrt_fifo0_rat", ";chip involved;mismatch count w.r.t. chip 0", 5, 0.5, 5.5, n_div, 0, n_div);
    TH1F *h_average_runtime = new TH1F("h_average_runtime", ";full acquisition duration (s)", 100, 2.0, 2.5);
    TH1F *h_mismatch_frequency = new TH1F("h_mismatch_frequency", ";# runs before another mismatch;occurences", 10, -0.5, 2999.5);
    TH1F *h_mismatch_frequency_s = new TH1F("h_mismatch_frequency_s", ";time before another mismatch (s);occurences", 10, -0.5, 4999.5);
    TH2F *h_mismatch_occurence_fifo_map = new TH2F("h_mismatch_occurence_fifo_map", ";fifo;fifo; fraction of occurrences (%)", 25, -0.5, 24.5, 25, -0.5, 24.5);
    TProfile2D *h_mismatch_difference_fifo_map = new TProfile2D("h_mismatch_difference_fifo_map", ";fifo;fifo", 25, -0.5, 24.5, 25, -0.5, 24.5);
    TProfile *h_mismatch_magnitude = new TProfile("h_mismatch_magnitude", "", 25, -0.5, 24.5);
    TProfile *h_mismatch_frequency_daytime = new TProfile("h_mismatch_frequency_daytime", ";time of day;occurence rate", 6, 0., 86400);
    h_mismatch_frequency_daytime->GetXaxis()->SetTimeDisplay(true);
    h_mismatch_frequency_daytime->GetXaxis()->SetTimeFormat(dataformat.c_str());
    h_mismatch_frequency_daytime->GetXaxis()->SetMaxDigits(2);
    TProfile *h_mismatch_frequency_day = new TProfile("h_mismatch_frequency_day", ";day of datataking;occurence rate", 12, 1732143600 - 86400 * 2, 1732143600 + 86400 * 10);
    h_mismatch_frequency_day->GetXaxis()->SetTimeDisplay(true);
    h_mismatch_frequency_day->GetXaxis()->SetTimeFormat("%d/%m/%Y%F1970-01-01 00:00:00");
    h_mismatch_frequency_day->GetXaxis()->SetMaxDigits(2);
    TGaxis::SetMaxDigits(3);

    //  Check variables
    int total_mismatches = 0;
    int mismatch_correct_runs = 0;
    int mismatch_correct_runs_timestamp = convertTimestampToSecondsWithFraction(list_of_values["#time"][0]);
    std::vector<int> mismatch_frequency;
    std::vector<double> mismatch_frequency_s;
    float mismatches_rate = 0;
    int current_mismatches = 0;
    int current_all_mismatches = 0;
    std::vector<std::array<int, 3>> mismatch_difference;
    std::map<std::array<int, 2>, int> mismatch_difference_on_chip;
    std::map<std::array<int, 2>, int> mismatch_on_chip;

    //  Set a cutoff time window
    double early_timestamp = convertTimestampToSecondsWithFraction("20241115-000000.000");
    double _late_timestamp = convertTimestampToSecondsWithFraction("20251120-000000.000");

    //  Loop on fifo
    auto i_point = -1;
    auto previous_timestamp = 0.;
    for (auto i_pos : good_events)
    {
        //  Entry infos
        double current_timestamp = convertTimestampToSecondsWithFraction(list_of_values["#time"][i_pos]);
        if (i_pos > 0)
            h_average_runtime->Fill(current_timestamp - previous_timestamp);
        h_mismatch_frequency_daytime->Fill(convertTimestampToDayTime(list_of_values["#time"][i_pos]), stoi(list_of_values[Form("check")][i_pos]));
        h_mismatch_frequency_day->Fill(convertTimestampToDay(list_of_values["#time"][i_pos]), stoi(list_of_values[Form("check")][i_pos]));
        previous_timestamp = current_timestamp;
        if (early_timestamp > current_timestamp)
            continue;
        if (_late_timestamp <= current_timestamp)
            continue;
        i_point++;
        double current_check_value = stoi(list_of_values[Form("check")][i_pos]);
        if (current_check_value)
        {
            mismatch_frequency_s.push_back(current_timestamp - mismatch_correct_runs_timestamp);
            mismatch_frequency.push_back(mismatch_correct_runs);
            mismatch_correct_runs_timestamp = current_timestamp;
            mismatch_correct_runs = 0;
        }
        else
            mismatch_correct_runs++;
        fifo_graph[24]->SetPoint(i_point, current_timestamp, current_check_value);
        total_mismatches += current_check_value;

        //  Check reset
        current_all_mismatches = 0;
        mismatch_difference.clear();
        for (auto i_chip = 0; i_chip < 5; i_chip++)
            for (auto j_chip = 0; j_chip < 5; j_chip++)
            {
                mismatch_on_chip[{i_chip, j_chip}] = 0;
                mismatch_difference_on_chip[{i_chip, j_chip}] = 0;
            }

        //  Loop over the fifo
        for (int i_fifo = 0; i_fifo < 24; i_fifo++)
        {
            int current_ififo_value = stoi(list_of_values[Form("fifo%i", i_fifo)][i_pos]);
            fifo_graph[i_fifo]->SetPoint(i_point, current_timestamp, current_ififo_value);

            //  Secondary loop over the fifo
            int current_mismatches = 0;
            for (int j_fifo = i_fifo + 1; j_fifo < 24; j_fifo++)
            {
                int current_jfifo_value = stoi(list_of_values[Form("fifo%i", j_fifo)][i_pos]);
                int current_ijfifo_diff = fabs(current_ififo_value - current_jfifo_value);
                if (current_ijfifo_diff)
                {
                    current_mismatches++;
                    mismatch_difference.push_back({i_fifo, j_fifo, current_ijfifo_diff});
                }
            }
            current_all_mismatches = max(current_all_mismatches, current_mismatches);
        }

        //  Fill histograms
        h_mismatch_occurence->Fill(current_all_mismatches);
        h_mismatch_occurence_chips->Fill(current_all_mismatches / 4);

        //  Loop over the fifo
        for (auto current_mismatch : mismatch_difference)
        {
            auto i_fifo = current_mismatch[0];
            auto j_fifo = current_mismatch[1];
            auto i_chip = i_fifo / 4;
            auto j_chip = j_fifo / 4;
            auto ij_dif = current_mismatch[2];
            h_mismatch_occurence_fifo_map->Fill(j_fifo, i_fifo);
            h_mismatch_difference_fifo_map->Fill(j_fifo, i_fifo, ij_dif);
            h_mismatch_magnitude->Fill(i_fifo, ij_dif);
            if (!i_fifo)
            {
                h_mismatch_wrt_fifo0->Fill(j_chip, 0.25);
                h2_mismatch_wrt_fifo0->Fill(j_chip, 4 * i_pos / (list_of_values["#time"].size()), 0.25);
            }
            if (i_chip == j_chip)
            {
                mismatch_on_chip[{i_chip, j_chip}]++;
                mismatch_difference_on_chip[{i_chip, j_chip}];
            }
        }
    }

    //  Post loop normalisations
    mismatches_rate = total_mismatches * 1. / (list_of_values["#time"].size());
    for (auto i = 0; i < (list_of_values["#time"].size()); i++)
        for (auto j = 1; j < 6; j++)
        {
            h_mismatch_wrt_fifo0_norm->Fill(j);
            h2_mismatch_wrt_fifo0_norm->Fill(j, 4 * i / (list_of_values["#time"].size()));
        }
    h_mismatch_occurence_fifo_map->Scale(100. / total_mismatches);
    h_mismatch_wrt_fifo0_rat->Divide(h_mismatch_wrt_fifo0, h_mismatch_wrt_fifo0_norm, 1., 1., "b");
    h2_mismatch_wrt_fifo0_rat->Divide(h2_mismatch_wrt_fifo0, h2_mismatch_wrt_fifo0_norm, 1., 1., "b");
    for (auto current_mismatch_period : mismatch_frequency)
        h_mismatch_frequency->Fill(current_mismatch_period);
    for (auto current_mismatch_period : mismatch_frequency_s)
        h_mismatch_frequency_s->Fill(current_mismatch_period);

    //  Prep
    TLatex *llatex = new TLatex();
    gStyle->SetOptStat(0);

    //  History canvas
    TCanvas *c_history_summary = new TCanvas("c_history_summary", "", 1750, 1000);
    fifo_graph[24]->Draw("ALP");
    fifo_graph[24]->SetMarkerStyle(20);
    fifo_graph[24]->SetMarkerColor(kBlack);
    auto integrated_time = (list_of_values["#time"].size()) * h_average_runtime->GetMean();
    auto integrated_sec = (int)(integrated_time) % 60;
    auto integrated_min = (int)((integrated_time - integrated_sec) / 60) % 60;
    auto integrated_hour = (int)((integrated_time - integrated_sec - integrated_min * 60) / 3600) % 24;
    auto integrated_day = (int)((integrated_time - integrated_sec - integrated_min * 60 - integrated_hour * 3600) / (24 * 3600));
    llatex->DrawLatexNDC(0.12, 0.92, Form("full integrated time %4dd %2d:%2d:%2d", integrated_day, integrated_hour, integrated_min, integrated_sec));
    llatex->DrawLatexNDC(0.12, 0.86, Form("total runs: %i", (list_of_values["#time"].size())));
    llatex->DrawLatexNDC(0.38, 0.86, Form("failures: %i", total_mismatches));
    llatex->DrawLatexNDC(0.62, 0.86, Form("failure rate: %.2f%%", mismatches_rate * 100));
    c_history_summary->SaveAs("test/c_history_summary.pdf");
    c_history_summary->SaveAs("test/c_history_summary.png");

    //  Run time canvas
    TCanvas *c_run_time_average = new TCanvas("c_run_time_average", "", 1250, 1250);
    h_average_runtime->Draw();
    h_average_runtime->Fit("gaus");
    auto current_gaus = h_average_runtime->GetFunction("gaus");
    llatex->DrawLatexNDC(0.5, 0.85, Form("avg:"));
    llatex->DrawLatexNDC(0.5, 0.80, Form("RMS:"));
    llatex->DrawLatexNDC(0.5, 0.75, Form("fit avg:"));
    llatex->DrawLatexNDC(0.5, 0.70, Form("fit #sigma:"));
    llatex->DrawLatexNDC(0.65, 0.85, Form("%.3f (s)", h_average_runtime->GetMean()));
    llatex->DrawLatexNDC(0.65, 0.80, Form("%.3f (s)", h_average_runtime->GetRMS()));
    llatex->DrawLatexNDC(0.65, 0.75, Form("%.3f (s)", current_gaus->GetParameter(1)));
    llatex->DrawLatexNDC(0.65, 0.70, Form("%.3f (s)", current_gaus->GetParameter(2)));
    c_run_time_average->SaveAs("test/c_run_time_average.pdf");
    c_run_time_average->SaveAs("test/c_run_time_average.png");

    //  Average time between mismatches canvas
    TCanvas *c_time_between_mismatches = new TCanvas("c_time_between_mismatches", "", 1250, 1250);
    h_mismatch_frequency_s->Draw();
    h_mismatch_frequency_s->Fit("expo");
    auto current_expo = h_mismatch_frequency_s->GetFunction("expo");
    llatex->DrawLatexNDC(0.45, 0.85, Form("avg:"));
    llatex->DrawLatexNDC(0.45, 0.80, Form("fit decay:"));
    llatex->DrawLatexNDC(0.65, 0.85, Form("%.0f (s)", h_mismatch_frequency_s->GetMean()));
    llatex->DrawLatexNDC(0.65, 0.80, Form("%.0f (s)", -1. / current_expo->GetParameter(1)));
    c_time_between_mismatches->SaveAs("test/c_mismatch_chips_involved.pdf");
    c_time_between_mismatches->SaveAs("test/c_mismatch_chips_involved.png");

    //  Mismatch fifos involved canvas
    TCanvas *c_mismatch_fifos_involved = new TCanvas("c_mismatch_chips_involved", "", 1250, 1250);
    gPad->SetLogy();
    h_mismatch_occurence->Draw("HIST TEXT");
    c_mismatch_fifos_involved->SaveAs("test/c_mismatch_fifos_involved.pdf");
    c_mismatch_fifos_involved->SaveAs("test/c_mismatch_fifos_involved.png");

    //  Mismatches chips involved canvas
    TCanvas *c_mismatch_chips_involved = new TCanvas("c_mismatch_chips_involved", "", 1250, 1250);
    gPad->SetLogy();
    h_mismatch_occurence_chips->Draw("HIST TEXT");
    c_mismatch_chips_involved->SaveAs("test/c_time_between_mismatches.pdf");
    c_mismatch_chips_involved->SaveAs("test/c_time_between_mismatches.png");

    //  Mismatches chips involved canvas
    TCanvas *c_mismatch_chips_ordered = new TCanvas("c_mismatch_chips_involved", "", 1250, 1250);
    h_mismatch_wrt_fifo0->Draw("HIST TEXT");
    c_mismatch_chips_ordered->SaveAs("test/c_mismatch_chips_ordered.pdf");
    c_mismatch_chips_ordered->SaveAs("test/c_mismatch_chips_ordered.png");

    TCanvas *cGeneral_stats = new TCanvas("cGeneral_stats", "", 1250, 1250);
    cGeneral_stats->Divide(2, 2);

    cGeneral_stats->cd(1);
    gPad->SetLogy();
    h_mismatch_occurence->Draw("HIST TEXT");
    h_mismatch_occurence->SetMinimum(0.5);
    llatex->SetTextSize(0.05);
    llatex->DrawLatexNDC(0.03, 0.95, Form("total runs: %i", (list_of_values["#time"].size())));
    llatex->DrawLatexNDC(0.40, 0.95, Form("failures: %i", total_mismatches));
    llatex->DrawLatexNDC(0.65, 0.95, Form("failure rate: %.2f%%", mismatches_rate * 100));

    cGeneral_stats->cd(2);
    h_average_runtime->Draw();
    llatex->DrawLatexNDC(0.3, 0.95, Form("avg: %.3f (s)", h_average_runtime->GetMean()));
    llatex->DrawLatexNDC(0.55, 0.95, Form("rms: %.3f (s)", h_average_runtime->GetRMS()));

    cGeneral_stats->cd(3);
    h_mismatch_wrt_fifo0->Draw("HIST TEXT");
    /*
    gPad->SetRightMargin(0.15);
    h_mismatch_occurence_fifo_map->Draw("COLZ TEXT");
    gStyle->SetPaintTextFormat("2.0f");
    h_mismatch_occurence_fifo_map->SetMaximum(100);
    h_mismatch_occurence_fifo_map->SetMinimum(0);
    */

    cGeneral_stats->cd(4);
    h_mismatch_frequency_s->Draw();
    llatex->DrawLatexNDC(0.3, 0.95, Form("avg: %.0f", h_mismatch_frequency_s->GetMean()));

    cGeneral_stats->SaveAs("cGeneral_stats.pdf");
    cGeneral_stats->SaveAs("cGeneral_stats.png");

    TCanvas *cTest = new TCanvas("cTest", "", 1250, 1250);
    cTest->Divide(2, 1);

    cTest->cd(1);
    h_mismatch_frequency_daytime->Draw();

    cTest->cd(2);
    h_mismatch_frequency_day->Draw();

    TCanvas *cTest2 = new TCanvas("cTest2", "", 1250, 1250);
    std::vector<int> colors = {kRed, kBlue, kGreen - 2, kYellow - 2};
    for (auto i_div = 0; i_div < n_div; i_div++)
    {
        auto current_slice = h2_mismatch_wrt_fifo0_rat->ProjectionX(Form("_%i", i_div), i_div + 1, i_div + 1);
        current_slice->SetMinimum(0.);
        current_slice->SetMaximum(0.0015);
        current_slice->SetMarkerStyle(20);
        current_slice->SetMarkerColor(colors[i_div]);
        current_slice->SetLineColor(colors[i_div]);
        current_slice->GetYaxis()->SetTitle("mismatch count w.r.t. chip 0");
        current_slice->Draw("SAME LEP X0");
    }

    //  Save to file
    TFile *fout = new TFile("conf_2.root", "RECREATE");
    h_mismatch_occurence->Write();
    h_mismatch_occurence_fifo_map->Write();
    h_mismatch_difference_fifo_map->Write();
    h_mismatch_magnitude->Write();
    h_average_runtime->Write();
    h_mismatch_frequency->Write();
    h_mismatch_frequency_s->Write();
    h_mismatch_wrt_fifo0->Write();
    h2_mismatch_wrt_fifo0_norm->Write();
    h_mismatch_wrt_fifo0_rat->Write();
    h2_mismatch_wrt_fifo0->Write();
    h2_mismatch_wrt_fifo0_rat->Write();
    h_mismatch_frequency_daytime->Write();
    h_mismatch_frequency_day->Write();
    for (auto current_graph : fifo_graph)
        current_graph->Write(current_graph->GetName());
    fout->Close();
}