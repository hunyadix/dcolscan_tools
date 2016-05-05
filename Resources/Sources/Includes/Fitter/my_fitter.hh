#ifndef MY_FITTER_HH
#define MY_FITTER_HH

#include "../Timer/Timer_colored.hh"
#include "../Console_actor/common_actors.hh"

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <algorithm>

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"

struct Fitted_TH1D
{
		int         num_layer;
		std::string name;
		TH1D*       histogram;
		TF1*        fit;
};

///////////////////////////
// Utility function list //
///////////////////////////

template <typename T>
T                        get_plot(std::string fname, std::string hname);
template <typename T>
void                     print_vector(std::ostream& stream, std::vector<T> vector, const std::string& delimiter);
template <typename T>
std::string              to_string_with_precision(const T a_value, const int n = 6);
std::string              generate_filename_suffix_by_efficiency(double eficiency);
std::string              generate_file_name_by_efficiency(const std::string& filename_base, double efficiency);
std::vector<Fitted_TH1D> create_hiteff_vs_dcoleff_histograms(const std::vector<double>& dcol_efficiencies, const std::vector<TH1D*>& mc_det_eff, const std::vector<int>& layers);
std::vector<Fitted_TH1D> create_instlumi_vs_dcoleff_histograms(const std::vector<Fitted_TH1D>& hiteff_vs_dcoleff_histograms, const std::vector<TH1D*>& data_instlumi_eff, const std::vector<int>& layers, const std::vector<std::string>& laynames);

/////////////////////
// Fitter function //
/////////////////////

void fitter(bool draw_fits = true, std::string print_factors = std::string("db"));

#endif