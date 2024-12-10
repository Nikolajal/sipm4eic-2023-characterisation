#pragma once
#include "utility.h"

namespace graphutils
{

  void reassign_x_values(TGraphErrors *gTarget, std::vector<std::array<float, 2>> new_axis)
  {
    if (new_axis.size() < (gTarget->GetN()))
      throw std::invalid_argument("[ERROR][graphutils::reassign_x_values] reassing axis must be larger than n points");
    for (auto iPnt = 0; iPnt < gTarget->GetN(); iPnt++)
    {
      gTarget->GetX()[iPnt] = new_axis[iPnt][0];
      gTarget->GetEX()[iPnt] = new_axis[iPnt][1];
    }
  }

  std::array<float, 2>
  eval_with_errors(TGraphErrors *gTarget, float _xtarget)
  {
    auto iPnt = 0;
    auto current_x = 1.;
    auto previous_x = 1.;
    float interpolated_y = -1.;
    float interpolated_ey = -1.;
    for (iPnt = 1; iPnt < gTarget->GetN() - 1; iPnt++)
    {
      current_x = gTarget->GetPointX(iPnt);
      previous_x = gTarget->GetPointX(iPnt - 1);
      if ((current_x >= _xtarget) && (previous_x <= _xtarget))
      {
        break;
      }
    }
    if (iPnt == gTarget->GetN() - 1)
    {
      return {interpolated_y, interpolated_ey};
    }
    auto coeff_y1 = (_xtarget - previous_x) / (current_x - previous_x);
    auto coeff_y0 = (current_x - _xtarget) / (current_x - previous_x);
    interpolated_y = coeff_y1 * gTarget->GetPointY(iPnt) + coeff_y0 * gTarget->GetPointY(iPnt - 1);
    interpolated_ey = sqrt((coeff_y0 * gTarget->GetErrorY(iPnt)) * (coeff_y0 * gTarget->GetErrorY(iPnt)) + (coeff_y1 * gTarget->GetErrorY(iPnt - 1)) * (coeff_y1 * gTarget->GetErrorY(iPnt - 1)));
    return {interpolated_y, interpolated_ey};
  }

  void
  set_style(TGraphErrors *g, int marker, int color, int fill = 0)
  {
    if (!g)
      return;
    g->SetMarkerStyle(marker);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetFillColor(color);
    g->SetFillStyle(fill);
  }

  std::map<std::string, TGraphErrors *>
  average_same_x(TGraphErrors *target_graph)
  {
    std::map<std::string, TGraphErrors *> result;
    result["ave_err"] = new TGraphErrors();
    std::map<double, std::vector<std::array<double, 2>>> aggregator;
    for (auto current_point = 0; current_point < target_graph->GetN(); current_point++)
      aggregator[target_graph->GetPointX(current_point)].push_back({target_graph->GetPointY(current_point), target_graph->GetErrorY(current_point)});
    auto i_pnt = -1;
    for (auto [x_value, y_values_array] : aggregator)
    {
      i_pnt++;
      auto current_average = utility::average(y_values_array);
      result["ave_err"]->SetPoint(i_pnt, x_value, current_average["ave_err"][0]);
      result["ave_err"]->SetPointError(i_pnt, 0, current_average["ave_err"][1]);
    }
    return result;
  }

  TGraphErrors *average(TGraphErrors *target_graph, int average_over_n_points = 2)
  {
    auto result = new TGraphErrors();
    for (int i_pnt = 0; i_pnt < target_graph->GetN() - average_over_n_points; i_pnt += average_over_n_points)
    {
      double current_x = 0.;
      double current_y = 0.;
      double current_ex = 0.;
      double current_ey = 0.;
      for (int j = i_pnt; j < i_pnt + average_over_n_points; ++j)
      {
        current_x += target_graph->GetX()[j];
        current_y += target_graph->GetY()[j];
        current_ex += target_graph->GetEX()[j];
        current_ey += target_graph->GetEY()[j];
      }
      current_x /= (double)average_over_n_points;
      current_y /= (double)average_over_n_points;
      current_ex /= (double)average_over_n_points;
      current_ey /= (double)average_over_n_points;
      auto current_point = result->GetN();
      result->SetPoint(current_point, current_x, current_y);
      result->SetPointError(current_point, current_ex, current_ey);
    }
    return result;
  }

  std::map<std::string, TGraphErrors *>
  average_graphs(std::vector<TGraphErrors *> graphs_list)
  {
    std::map<std::string, TGraphErrors *> result;
    for (auto i_pnt = 0; i_pnt < graphs_list[0]->GetN(); i_pnt++)
    {
      std::vector<std::array<double, 2>> current_point_x;
      std::vector<std::array<double, 2>> current_point_y;
      for (auto current_graph : graphs_list)
      {
        current_point_x.push_back({current_graph->GetPointX(i_pnt), current_graph->GetErrorX(i_pnt)});
        current_point_y.push_back({current_graph->GetPointY(i_pnt), current_graph->GetErrorY(i_pnt)});
      }
      auto current_average_x = utility::average(current_point_x);
      auto current_average_y = utility::average(current_point_y);
      for (auto [type, value] : current_average_x)
      {
        if (i_pnt == 0)
          result[type] = new TGraphErrors();
        result[type]->SetPoint(i_pnt, current_average_x[type][0], current_average_y[type][0]);
        result[type]->SetPointError(i_pnt, current_average_x[type][1], current_average_y[type][1]);
      }
    }
    return result;
  }

  std::map<std::string, TGraphErrors *>
  make_average_rms(std::vector<TGraphErrors *> graphs, int marker = 20, int color = kAzure - 3)
  {
    TProfile p("p", "", 20000, -100., 100.);
    TProfile pS("pS", "", 20000, -100., 100., "S");
    double themin[20000], themax[20000];
    for (int i = 0; i < 20000; ++i)
      themin[i] = themax[i] = -999.;
    for (auto g : graphs)
    {
      for (int j = 0; j < g->GetN(); ++j)
      {
        auto x = g->GetX()[j];
        auto y = g->GetY()[j];
        int ibin = p.Fill(x, y);
        pS.Fill(x, y);
        if (themin[ibin - 1] == -999.)
          themin[ibin - 1] = y;
        if (themax[ibin - 1] == -999.)
          themax[ibin - 1] = y;
        if (y < themin[ibin - 1])
          themin[ibin - 1] = y;
        if (y > themax[ibin - 1])
          themax[ibin - 1] = y;
      }
    }

    TGraphErrors *gave = new TGraphErrors;
    gave->SetName("gave");
    set_style(gave, marker, color, 0);
    TGraphErrors *grms = new TGraphErrors;
    grms->SetName("grms");
    //  set_style(grms, marker, color, 3002);
    set_style(grms, marker, color, 0);
    TGraphErrors *gwidth = new TGraphErrors;
    gwidth->SetName("gwidth");
    set_style(gwidth, marker, color, 0);
    for (int i = 0; i < p.GetNbinsX(); ++i)
    {
      if (p.GetBinError(i + 1) <= 0)
        continue;
      auto n = gave->GetN();
      gave->SetPoint(n, p.GetBinLowEdge(i + 1), p.GetBinContent(i + 1));
      gave->SetPointError(n, 0., p.GetBinError(i + 1));
      grms->SetPoint(n, p.GetBinLowEdge(i + 1), p.GetBinContent(i + 1));
      grms->SetPointError(n, 0., pS.GetBinError(i + 1));
      gwidth->SetPoint(n, p.GetBinLowEdge(i + 1), 0.5 * (themax[i] + themin[i]));
      gwidth->SetPointError(n, 0., 0.5 * (themax[i] - themin[i]));
    }

    return {{"average", gave}, {"rms", grms}, {"width", gwidth}};
  }

  void remove_points(TGraphErrors *g, double min, double max)
  {
    int i = 0;
    while (i < g->GetN())
    {
      if (g->GetX()[i] < min || g->GetX()[i] > max)
      {
        g->RemovePoint(i);
        i = 0;
      }
      else
        ++i;
    }
  }

  void x_scale(TGraphErrors *g, double scale)
  {
    for (int i = 0; i < g->GetN(); ++i)
    {
      g->GetX()[i] *= scale;
      g->GetEX()[i] *= scale;
    }
  }

  void y_scale(TGraphErrors *g, double scale)
  {
    for (int i = 0; i < g->GetN(); ++i)
    {
      g->GetY()[i] *= scale;
      g->GetEY()[i] *= scale;
    }
  }

  void y_scale(std::vector<TGraphErrors *> vg, double scale)
  {
    for (auto &g : vg)
      y_scale(g, scale);
  }

  TH1 *project(TGraphErrors *g, int nbins, double min, double max)
  {
    auto h = new TH1F("h", "", nbins, min, max);
    for (int i = 0; i < g->GetN(); ++i)
    {
      h->Fill(g->GetY()[i]);
    }
    return h;
  }

  void style(TGraphErrors *g, int marker, int color)
  {
    g->SetMarkerStyle(marker);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
  }

  void style(std::vector<TGraphErrors *> vg, int marker, int color)
  {
    for (auto g : vg)
      style(g, marker, color);
  }

  void draw(std::vector<TGraphErrors *> vg, const char *opt = "")
  {
    for (auto g : vg)
      g->Draw(opt);
  }

  TGraphErrors *
  scale(TGraphErrors *gin, double factor)
  {
    auto g = (TGraphErrors *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto ex = gin->GetEX()[i];
      auto y = gin->GetY()[i];
      auto ey = gin->GetEY()[i];
      g->SetPoint(i, x, y / factor);
      g->SetPointError(i, ex, ey / factor);
    }
    return g;
  }

  TGraphErrors *
  scale(TGraphErrors *target_graph, std::array<double, 2> scaling_factor)
  {
    auto result = (TGraphErrors *)target_graph->Clone(target_graph->GetName());
    result->Set(0);
    for (int i = 0; i < target_graph->GetN(); ++i)
    {
      auto x = target_graph->GetX()[i];
      auto ex = target_graph->GetEX()[i];
      auto y = target_graph->GetY()[i];
      auto ey = target_graph->GetEY()[i];
      auto factor = scaling_factor[0];
      auto efactor = scaling_factor[1];
      result->SetPoint(i, x, y / factor);
      result->SetPointError(i, ex, sqrt(ey * ey / (factor * factor) + ((efactor * efactor) * (y * y) / (factor * factor * factor * factor))));
    }
    return result;
  }

  std::pair<double, double>
  eval(TGraphErrors *g, double x)
  {
    TGraph gval, gerr;
    for (int i = 0; i < g->GetN(); ++i)
    {
      gval.SetPoint(i, g->GetX()[i], g->GetY()[i]);
      gerr.SetPoint(i, g->GetX()[i], g->GetEY()[i]);
    }
    return {gval.Eval(x), gerr.Eval(x)};
  }

  TGraphErrors *
  swapxy(TGraphErrors *gin)
  {
    auto g = (TGraphErrors *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto ex = gin->GetEX()[i];
      auto y = gin->GetY()[i];
      auto ey = gin->GetEY()[i];
      g->SetPoint(i, y, x);
      g->SetPointError(i, ey, ex);
    }
    return g;
  }

  TGraph *
  swapxy(TGraph *gin)
  {
    auto g = (TGraph *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = gin->GetY()[i];
      g->SetPoint(i, y, x);
    }
    return g;
  }

  TGraphErrors *
  derivate(TGraphErrors *gin, double sign = 1.)
  {
    auto g = (TGraphErrors *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      if (gin->GetEY()[i] <= 0.)
        continue;
      auto x0 = gin->GetX()[i];
      auto y0 = gin->GetY()[i];
      auto ey0 = gin->GetEY()[i];
      for (int j = i + 1; j < gin->GetN(); ++j)
      {
        if (gin->GetEY()[j] <= 0.)
          continue;
        auto x1 = gin->GetX()[j];
        auto y1 = gin->GetY()[j];
        auto ey1 = gin->GetEY()[j];

        auto val = sign * (y1 - y0);
        auto vale = TMath::Sqrt(ey1 * ey1 + ey0 * ey0);

        auto n = g->GetN();
        //      g->SetPoint(n, 0.5*(x0 + x1), val);
        g->SetPoint(n, x0, val);
        //      g->SetPointError(n, 0.5*(x1 - x0), vale);
        g->SetPointError(n, 0., vale);
        break;
      }
    }

    return g;
  }

  TGraph *
  derivate(TGraph *gin, double sign = 1.)
  {
    auto g = (TGraph *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x0 = gin->GetX()[i];
      auto y0 = gin->GetY()[i];
      for (int j = i + 1; j < gin->GetN(); ++j)
      {
        auto x1 = gin->GetX()[j];
        auto y1 = gin->GetY()[j];

        auto val = sign * (y1 - y0) / (x1 - x0);

        auto n = g->GetN();
        //      g->SetPoint(n, 0.5*(x0 + x1), val);
        g->SetPoint(n, x0, val);
        //      g->SetPointError(n, 0.5*(x1 - x0), vale);
        break;
      }
    }

    return g;
  }

  /*
  TGraphErrors *
  derivate(TGraphErrors *gin)
  {
  auto g = (TGraphErrors*)gin->Clone();
  g->Set(0);
  g->SetName(gin->GetName());
  for (int i = 0; i < gin->GetN(); ++i) {
    if (gin->GetEY()[i] <= 0.) continue;
    auto x0 = gin->GetX()[i];
    auto y0 = gin->GetY()[i];
    auto ey0 = gin->GetEY()[i];
    for (int j = i + 1; j < gin->GetN(); ++j) {
      if (gin->GetEY()[j] <= 0.) continue;
      auto x1 = gin->GetX()[j];
      auto y1 = gin->GetY()[j];
      auto ey1 = gin->GetEY()[j];

      auto cen = 0.5 * (x1 + x0);
      auto val = -(y1 - y0) / (x1 - x0);
      auto vale = TMath::Sqrt(ey1 * ey1 + ey0 * ey0) / (x1 - x0);

      auto n = g->GetN();
      g->SetPoint(n, cen, val);
      g->SetPointError(n, 0, vale);
      break;
    }
  }

  return g;
  }
  */

  TGraphErrors *
  ratio(TGraphErrors *gn, TGraphErrors *gd, bool propagate_error = true)
  {
    auto g = (TGraphErrors *)gn->Clone();
    g->Set(0);
    g->SetName(gn->GetName());
    for (int i = 0; i < gn->GetN(); ++i)
    {
      auto x = gn->GetX()[i];
      auto y = gn->GetY()[i];
      auto ex = gn->GetEX()[i];
      auto ey = gn->GetEY()[i];
      if (x < gd->GetX()[0])
        continue;
      if (x > gd->GetX()[gd->GetN() - 1])
        continue;
      if (gn->Eval(x) == 0. || gd->Eval(x) == 0.)
        continue;
      auto ret = eval(gd, x);
      auto d = ret.first;
      auto ed = ret.second;
      auto rey = ey / y;
      auto red = ed / d;
      y = y / d;
      if (propagate_error)
        ey = sqrt(rey * rey + red * red) * y;
      else
        ey = ey / d;
      auto n = g->GetN();
      g->SetPoint(n, x, y);
      g->SetPointError(n, ex, ey);
      n++;
    }
    return g;
  }

  TGraphErrors *
  multi(TGraphErrors *gn, TGraphErrors *gd, bool propagate_error = true)
  {
    auto g = (TGraphErrors *)gn->Clone();
    g->Set(0);
    g->SetName(gn->GetName());
    for (int i = 0; i < gn->GetN(); ++i)
    {
      auto x = gn->GetX()[i];
      auto y = gn->GetY()[i];
      auto ex = gn->GetEX()[i];
      auto ey = gn->GetEY()[i];
      if (gn->Eval(x) == 0. || gd->Eval(x) == 0.)
        continue;
      auto ret = eval(gd, x);
      auto d = ret.first;
      auto ed = ret.second;
      auto rey = ey / y;
      auto red = ed / d;
      y = y * d;
      if (propagate_error)
        ey = sqrt(rey * rey + red * red) * y;
      else
        ey = ey * d;
      auto n = g->GetN();
      g->SetPoint(n, x, y);
      g->SetPointError(n, ex, ey);
      n++;
    }
    return g;
  }

  TGraphErrors *
  log(TGraphErrors *gin)
  {
    auto g = new TGraphErrors;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = gin->GetY()[i];
      auto ex = gin->GetEX()[i];
      auto ey = gin->GetEY()[i];
      g->SetPoint(i, x, std::log(y));
      g->SetPointError(i, ex, ey / y);
    }
    return g;
  }

  TGraphErrors *
  log10(TGraphErrors *gin)
  {
    auto g = new TGraphErrors;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = gin->GetY()[i];
      auto ex = gin->GetEX()[i];
      auto ey = gin->GetEY()[i];
      g->SetPoint(i, x, std::log10(y));
      g->SetPointError(i, ex, ey / y);
    }
    return g;
  }

  TGraphErrors *
  diff(TGraphErrors *gin, TF1 *f)
  {
    auto g = (TGraphErrors *)gin->Clone();
    g->Set(0);
    g->SetName(gin->GetName());
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = gin->GetY()[i];
      auto ex = gin->GetEX()[i];
      auto ey = gin->GetEY()[i];
      g->SetPoint(i, x, y - f->Eval(x));
      g->SetPointError(i, ex, ey);
    }
    return g;
  }

  TGraphErrors *
  diff(TGraphErrors *gn, TGraphErrors *gd, bool propagate_error = true)
  {
    auto g = (TGraphErrors *)gn->Clone();
    g->Set(0);
    g->SetName(gn->GetName());
    for (int i = 0; i < gn->GetN(); ++i)
    {
      auto x = gn->GetX()[i];
      auto y = gn->GetY()[i];
      auto ex = gn->GetEX()[i];
      auto ey = gn->GetEY()[i];
      if (gn->Eval(x) == 0. || gd->Eval(x) == 0.)
        continue;
      auto ret = eval(gd, x);
      auto d = ret.first;
      auto ed = ret.second;
      y = y - d;
      if (propagate_error)
        ey = sqrt(ey * ey + ed * ed);
      else
        ey = ey;
      auto n = g->GetN();
      g->SetPoint(n, x, y);
      g->SetPointError(n, ex, ey);
      n++;
    }
    return g;
  }

  void x_shift(TGraphErrors *g, float val)
  {
    for (int i = 0; i < g->GetN(); ++i)
      g->GetX()[i] -= val;
  }

  void y_shift(TGraphErrors *g, float val)
  {
    for (int i = 0; i < g->GetN(); ++i)
      g->GetY()[i] -= val;
  }

  TGraph *
  diff(TGraph *gin, TGraph *gref)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = gin->GetY()[i];
      if (x < gref->GetX()[0])
        continue;
      if (x > gref->GetX()[gref->GetN() - 1])
        continue;
      auto ref = gref->Eval(x);
      auto val = (y - ref) / ref;
      gout->SetPoint(gout->GetN(), x, val);
    }
    return gout;
  }

  TGraph *
  fromZero(TGraph *gin)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i] - gin->GetX()[0];
      auto y = gin->GetY()[i];
      gout->SetPoint(i, x, y);
    }
    return gout;
  }

  TGraph *
  invertY(TGraph *gin)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = -gin->GetY()[i];
      gout->SetPoint(i, x, y);
    }
    return gout;
  }

  TGraphErrors *
  invertY(TGraphErrors *gin)
  {
    auto gout = new TGraphErrors;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = gin->GetX()[i];
      auto y = -gin->GetY()[i];
      gout->SetPoint(i, x, y);
    }
    return gout;
  }

  TGraph *
  invertX(TGraph *gin)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN(); ++i)
    {
      auto x = -gin->GetX()[i];
      auto y = gin->GetY()[i];
      gout->SetPoint(i, x, y);
    }
    return gout;
  }

  TGraph *
  average(TGraph *gin, int n = 2)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN() - n; i += n)
    {
      double x = 0.;
      double y = 0.;
      for (int j = i; j < i + n; ++j)
      {
        x += gin->GetX()[j];
        y += gin->GetY()[j];
      }
      x /= (double)n;
      y /= (double)n;
      gout->SetPoint(gout->GetN(), x, y);
    }
    return gout;
  }

  TGraph *
  rms(TGraph *gin, int n = 2)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN() - n; i += n)
    {
      double x = 0.;
      double y = 0.;
      for (int j = i; j < i + n; ++j)
      {
        x += gin->GetX()[j];
        y += gin->GetY()[j];
      }
      x /= (double)n;
      y /= (double)n;
      auto mean = y;
      y = 0.;
      for (int j = i; j < i + n; ++j)
      {
        y += (gin->GetY()[j] - mean) * (gin->GetY()[j] - mean);
      }
      y /= (double)n;
      gout->SetPoint(gout->GetN(), x, std::sqrt(y));
    }
    return gout;
  }

  TGraph *
  moving_average(TGraph *gin, int n = 2)
  {
    auto gout = new TGraph;
    for (int i = 0; i < gin->GetN() - n; ++i)
    {
      double x = 0.;
      double y = 0.;
      for (int j = i; j < i + n; ++j)
      {
        x += gin->GetX()[j];
        y += gin->GetY()[j];
      }
      x /= (double)n;
      y /= (double)n;
      gout->SetPoint(i, x, y);
    }
    return gout;
  }
}
