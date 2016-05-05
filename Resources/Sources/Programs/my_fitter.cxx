#include "../Includes/Fitter/my_fitter.hh"
#include "../Includes/Fitter/custom_can_macro.hh"

#ifdef DEBUG_ALL
#undef DEBUG_ALL
#endif

void fitter(bool draw_fits, std::string print_factors)
{
	////////////////////
	// Argument check //
	////////////////////

	const std::vector<std::string> possible_print_factors =
	{
		"cfg",
		"db"
	};
	if(std::count(possible_print_factors.begin(), possible_print_factors.end(), print_factors) == 0)
	{
		std::cerr << error_prompt << "in fitter: invalid value set print_factors." << std::endl;
		std::cerr << "Possible_print_factors:" << std::endl;
		print_vector(std::cerr, possible_print_factors, "\n");
	}
	////////////////
	// Data paths //
	////////////////

	const std::string data_folder     = "/afs/cern.ch/user/a/ahunyadi/private/Dcolscan_creation/";
	const std::string mc_input_folder = data_folder + "Results/Dcolscans/";
	// const std::string data_input_file = data_folder + "Results/DynIneff_2015.root";
	const std::string data_input_file = data_folder + "Results/reweighting_step_1.root";

	const std::string default_hiteff_vs_layer_histo_name    = "HitEfficiency_vs_Layers/Ring12";
	const std::string default_hiteff_vs_instlumi_histo_prefix = "HitEfficiency_vs_Instlumi/Layer";

	///////////////
	// Constants //
	///////////////

	const std::vector<int>         layers{1, 2, 3};
	const std::vector<double>      dcol_efficiencies{1, 0.999, 0.996, 0.993, 0.99, 0.98, 0.97, 0.95, 0.90, 0.80}; 
	// const std::vector<double>      dcol_efficiencies{0.999, 0.996, 0.993, 0.98, 0.97, 0.90}; 
	const std::vector<std::string> laynames{"_Lay1", "_Lay2","_Lay3"};

	/////////////////////////
	// Important variables //
	/////////////////////////

	std::vector<TH1D*> data_instlumi_eff(3);
	std::vector<TH1D*> mc_det_eff(10);

	//////////////////////////////////
	// Initializing data histograms //
	//////////////////////////////////

	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		std::string plot_name  = default_hiteff_vs_instlumi_histo_prefix + std::to_string(lay) + "_Ring12";
		data_instlumi_eff[layer_index] = get_plot<TH1D*>(data_input_file, plot_name);
	}

	/////////////////////////////////////////
	// Initializing monte carlo histograms //
	/////////////////////////////////////////

	std::cout << process_prompt << "Reading layer efficiencies from Monte-Carlo outputs..." << std::endl;
	for(int i = 0, i_max = dcol_efficiencies.size(); i < i_max; ++i)
	{
		const double& efficiency = dcol_efficiencies[i];
		std::string mc_input_file_name(generate_file_name_by_efficiency("dcol", efficiency));
		mc_det_eff[i] = get_plot<TH1D*>(mc_input_folder + mc_input_file_name, default_hiteff_vs_layer_histo_name);
#ifdef DEBUG_ALL
		std::cerr << debug_prompt << "Input file with name: " << mc_input_file_name << " has been processed successfully." << std::endl;
#endif
	}

	////////////////////////////////
	// Data Predicted 2016 points //
	////////////////////////////////

	int fixbin_1 = data_instlumi_eff[0] -> FindBin(10.976);
	int fixbin_2 = data_instlumi_eff[0] -> FindBin(15.68);
	data_instlumi_eff[0] -> SetBinContent(fixbin_1, 0.975);
	data_instlumi_eff[0] -> SetBinContent(fixbin_2, 0.965);
	data_instlumi_eff[0] -> SetBinError(fixbin_1, 0.003);
	data_instlumi_eff[0] -> SetBinError(fixbin_2, 0.003);

	//////////////////////////////////////////////////
	// Hit efficiency vs dcol efficiency histograms //
	//////////////////////////////////////////////////

	std::vector<Fitted_TH1D> hiteff_vs_dcoleff_histograms(create_hiteff_vs_dcoleff_histograms(dcol_efficiencies, mc_det_eff, layers));

	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		data_instlumi_eff[layer_index] -> SetMarkerStyle(20 + lay);
	}

	////////////////////////////////////////////
	// Instlumi vs dcol efficiency histograms //
	////////////////////////////////////////////

	std::vector<Fitted_TH1D> instlumi_vs_dcoleff(create_instlumi_vs_dcoleff_histograms(hiteff_vs_dcoleff_histograms, data_instlumi_eff, layers, laynames));

	/////////////////////////////////////
	// Printing out the fit parameters //
	/////////////////////////////////////

	if(print_factors == "cfg")
	{
		for(const int& lay: layers)
		{
			const int layer_index = lay - 1;
			std::cout << "    thePUEfficiency_BPix" << (layer_index + 1) << " = cms.vdouble(" << std::endl;
			for(int par = 0; par < 2; par++)
			{
				std::cout << "        " << instlumi_vs_dcoleff[layer_index].fit -> GetParameter(par) << ",\n";
			}
			std::cout << "    ),\n";
		}
	}
	if(print_factors == "db")
	{
		std::cout << "    thePUEfficiency = cms.untracked.VPSet(\n";
		for(const int& lay: layers)
		{
			const int layer_index = lay - 1;
			std::cout << "        cms.PSet(\n";
			std::cout << "            det = cms.string(\"bpix\"),\n";
			std::cout << "            layer = cms.uint32(" << layer_index + 1 << "),\n";
			std::cout << "            factor = cms.vdouble(\n";
			for(int par = 0; par < 2; par++)
			{
				std::cout<<"              " << instlumi_vs_dcoleff[layer_index].fit -> GetParameter(par) << ",\n";
			}
			std::cout << "            ),\n";
			std::cout << "        ),\n";
		}
		std::cout << "    ),\n";
	}

	///////////////////
	// Drawing plots //
	///////////////////

	gStyle->SetOptTitle(0);
	TCanvas* canvas = custom_can_(hiteff_vs_dcoleff_histograms[0].histogram, "HitEfficiency_vs_DCol", 1, 1, 500, 500, 80, 20);
	canvas -> cd();
	hiteff_vs_dcoleff_histograms[0].histogram -> SetAxisRange(0.85, 1, "Y");
	hiteff_vs_dcoleff_histograms[0].histogram -> SetMarkerColor(633);
	hiteff_vs_dcoleff_histograms[0].histogram -> SetMarkerStyle(20);
	hiteff_vs_dcoleff_histograms[0].histogram -> Draw("P");
	hiteff_vs_dcoleff_histograms[1].histogram -> SetMarkerColor(618);
	hiteff_vs_dcoleff_histograms[1].histogram -> SetMarkerStyle(21);
	hiteff_vs_dcoleff_histograms[1].histogram -> Draw("SAMEP");
	hiteff_vs_dcoleff_histograms[2].histogram -> SetMarkerColor(601);
	hiteff_vs_dcoleff_histograms[2].histogram -> SetMarkerStyle(22);
	hiteff_vs_dcoleff_histograms[2].histogram -> Draw("SAMEP");
	hiteff_vs_dcoleff_histograms[0].fit       -> SetLineColor(633);
	hiteff_vs_dcoleff_histograms[1].fit       -> SetLineColor(618);
	hiteff_vs_dcoleff_histograms[2].fit       -> SetLineColor(601);
	hiteff_vs_dcoleff_histograms[0].fit       -> Draw("SAME");
	hiteff_vs_dcoleff_histograms[1].fit       -> Draw("SAME");
	hiteff_vs_dcoleff_histograms[2].fit       -> Draw("SAME");
	TLegend* leg = new TLegend(0.35, 0.2, 0.5, 0.4, "");
	leg -> AddEntry(hiteff_vs_dcoleff_histograms[0].histogram, "Layer 1", "P");
	leg -> AddEntry(hiteff_vs_dcoleff_histograms[1].histogram, "Layer 2", "P");
	leg -> AddEntry(hiteff_vs_dcoleff_histograms[2].histogram, "Layer 3", "P");
	leg -> SetFillColor(0);
	leg -> SetFillStyle(0);
	leg -> SetBorderSize(0);
	leg -> SetTextSize(0.04);
	leg -> Draw("SAMEP1");

	canvas = custom_can_(data_instlumi_eff[0], "HitEfficiency_vs_InstLumi_With2016Pred", 1,1, 500,500, 90,20);
	data_instlumi_eff[0] -> SetTitle("");
	data_instlumi_eff[0] -> GetYaxis()->SetTitle("Hit Efficiency");
	data_instlumi_eff[0] -> GetXaxis()->SetRangeUser(0,20);
	data_instlumi_eff[0] -> GetYaxis()->SetRangeUser(0.95,1);
	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		data_instlumi_eff[layer_index]->SetMarkerColor(layer_index==0 ? 633 : layer_index==1 ? 618 : 601);
		data_instlumi_eff[layer_index]->Draw(layer_index? "P SAME" : "P");
	}
	leg = new TLegend(0.35, 0.2, 0.55, 0.4, "2232 Colliding Bunches");
	leg -> AddEntry(data_instlumi_eff[0], "Layer 1", "P");
	leg -> AddEntry(data_instlumi_eff[1], "Layer 2", "P");
	leg -> AddEntry(data_instlumi_eff[2], "Layer 3", "P");
	leg -> SetFillColor(0);
	leg -> SetFillStyle(0);
	leg -> SetBorderSize(0);
	leg -> SetTextSize(0.04);
	leg -> Draw("SAMEP1");

	canvas = custom_can_(instlumi_vs_dcoleff[0].histogram, "DColEff_vs_InstLumi_With2016Pred", 1, 1, 500, 500, 80, 20);
	instlumi_vs_dcoleff[0].histogram -> SetTitle("2016 Prediciton");
	instlumi_vs_dcoleff[0].histogram -> GetXaxis() -> SetRangeUser(0,20);
	instlumi_vs_dcoleff[0].histogram -> GetYaxis() -> SetTitle("DCol factor");
	instlumi_vs_dcoleff[0].histogram -> GetYaxis() -> SetRangeUser(0.86,1);
	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		instlumi_vs_dcoleff[layer_index].histogram -> SetMarkerColor(layer_index == 0 ? 633 : layer_index == 1 ? 618 : 601);
		instlumi_vs_dcoleff[layer_index].fit -> SetLineColor(layer_index == 0 ? 633 : layer_index == 1 ? 618 : 601);
		instlumi_vs_dcoleff[layer_index].histogram -> Draw(layer_index? "P SAME" : "P");
		instlumi_vs_dcoleff[layer_index].fit -> Draw("SAME");
	}
	leg = new TLegend(0.35, 0.2, 0.5, 0.4, "");
	leg -> AddEntry(instlumi_vs_dcoleff[0].histogram,   "Layer 1", "P");
	leg -> AddEntry(instlumi_vs_dcoleff[1].histogram,   "Layer 2", "P");
	leg -> AddEntry(instlumi_vs_dcoleff[2].histogram,   "Layer 3", "P");
	leg -> SetFillColor(0);
	leg -> SetFillStyle(0);
	leg -> SetBorderSize(0);
	leg -> SetTextSize(0.04);
	leg -> Draw("SAMEP1");

	std::cin.get();
}

//////////////////////////////////
// Utility function definitions //
//////////////////////////////////

template <typename T>
T get_plot(std::string filename, std::string histoname)
{
	TFile file(filename.c_str(), "READ");
	if(!file.IsOpen())
	{
		std::cerr << error_prompt << "Error opening file: " << filename << std::endl;
		exit(-1);
	}
	T histo = (T)file.Get(histoname.c_str());
	if(!histo)
	{
		std::cerr << error_prompt << "Error fetching the histogram: " << histoname << " in " << filename << std::endl;
		exit(-1);
	}
	T cloned_histo = (T)histo -> Clone();
	cloned_histo -> SetDirectory(0);
	return cloned_histo;
}

template <typename T>
void print_vector(std::ostream& stream, std::vector<T> vector, const std::string& delimiter)
{
	for(const auto element: vector)
	{
		stream << element << delimiter;
	}
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::stringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

std::string generate_filename_suffix_by_efficiency(double efficiency)
{
	int precision = 3;
	if(efficiency < 1 && 0.99 < efficiency)
	{
	    precision = 4;
	}
	std::string number = to_string_with_precision(efficiency * 100, precision);
	std::replace(number.begin(), number.end(), '.', 'p');
	return number;
}

std::string generate_file_name_by_efficiency(const std::string& filename_base, double efficiency)
{
	std::string filename  = filename_base;
	filename             += generate_filename_suffix_by_efficiency(efficiency);
	filename             += ".root";
	return filename;
}

std::vector<Fitted_TH1D> create_hiteff_vs_dcoleff_histograms(const std::vector<double>& dcol_efficiencies, const std::vector<TH1D*>& mc_det_eff, const std::vector<int>& layers)
{
	std::cout << process_prompt << "Creating hit efficiency vs dcol efficiency histograms..." << std::endl;
	std::vector<Fitted_TH1D> hiteff_vs_dcoleff_histograms(layers.size());
	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		Fitted_TH1D& histo_data = hiteff_vs_dcoleff_histograms[layer_index];
		histo_data.num_layer    = lay;
		histo_data.name         = "hiteff_vs_dcol_l" + lay;
		histo_data.histogram    = new TH1D(histo_data.name.c_str(), ";DCol Efficiency;Hit Efficiency", 251, 0.7495, 1.0005);
		for(unsigned int num_eff = 0, dcol_efficiencies_size = dcol_efficiencies.size(); num_eff < dcol_efficiencies_size; ++num_eff)
		{
			int bin = histo_data.histogram -> FindBin(dcol_efficiencies[num_eff]);
			histo_data.histogram -> SetBinContent(bin, mc_det_eff[num_eff] -> GetBinContent(layer_index + 1));
			histo_data.histogram -> SetBinError(  bin, mc_det_eff[num_eff] -> GetBinError(  layer_index + 1));
		}
		std::cout << process_prompt << "Histogram for layer " << lay << " created. Entries between: [" 
		                            << histo_data.histogram -> GetMinimum() << ", " 
		                            << histo_data.histogram -> GetMaximum() << "]." << std::endl;
		histo_data.fit = new TF1(histo_data.name.c_str(), "pol2", 0.75, 1);
		histo_data.histogram -> Fit(histo_data.fit, "RQ0");
	}
	return hiteff_vs_dcoleff_histograms;
}


std::vector<Fitted_TH1D> create_instlumi_vs_dcoleff_histograms(const std::vector<Fitted_TH1D>& hiteff_vs_dcoleff_histograms, const std::vector<TH1D*>& data_instlumi_eff, const std::vector<int>& layers, const std::vector<std::string>& laynames)
{
	std::cout << process_prompt << "Creating instant luminosity vs dcol efficiency histograms..." << std::endl;
	std::vector<Fitted_TH1D> instlumi_vs_dcoleff(layers.size());
	for(const int& lay: layers)
	{
		const int layer_index = lay - 1;
		Fitted_TH1D& histo_data = instlumi_vs_dcoleff[layer_index];
		histo_data.num_layer    = lay;
		// Instlumi binning
		histo_data.name         = "instlumi_vs_dcol_l" + std::to_string(lay) + ";DCol Efficiency;Instlumi";
		histo_data.histogram    = (TH1D*)data_instlumi_eff[layer_index] -> Clone();
		int num_instlumi_bins = data_instlumi_eff[layer_index]->GetNbinsX();
		for(int i = 1; i < num_instlumi_bins; ++i)
		{
			double eff  = data_instlumi_eff[layer_index]                -> GetBinContent(i);
			double dcol = hiteff_vs_dcoleff_histograms[layer_index].fit -> GetX(eff);
			histo_data.histogram->SetBinContent(i,dcol);
		}
		std::string fit_name = std::string("instlumi_fit") + laynames[layer_index].c_str();
		histo_data.fit = new TF1(fit_name.c_str(), "pol1", 0, 20);

		// Fix 1st parameter so that dcol(0PU) = 1
		histo_data.fit -> FixParameter(0, 1);
		histo_data.histogram -> Fit(histo_data.fit, "RQ0");
		std::cout << "Layer " << lay + 1 << " (const pu[0]=0.997) chi^2/Ndof = " 
		          << histo_data.fit -> GetChisquare() / histo_data.fit -> GetNDF() << std::endl;
	}
	return instlumi_vs_dcoleff;
}
