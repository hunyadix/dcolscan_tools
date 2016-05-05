/******************************************************************************************************/
/*                                                                                                    */
/*                      Program to perform dcolscan analysis.                                         */
/*                                                                                                    */
/*                      Date: 2015. CMSSW version: CMSSW_7_5_0                                        */
/*                      Author: Adam Hunyadi                                                          */
/*                      E-mail: hunyadix@gmail.com                                                    */
/*                                                                                                    */
/******************************************************************************************************/

#include "../Includes/Console_actor/common_actors.hh"

#include "../Includes/Fitter/my_fitter.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TApplication.h"
#include "TStyle.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

bool debug = false;
bool debug_prompt_destructors = true;

void set_process_name(std::string& process_name);

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                    Important function: main()                                      */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

int main(int argc, char** argv)
{
	// new TApplication( "Application", &argc, argv );
	try
	{

		/******************************************************************************************************/
		/*                                       Setting up the project                                       */
		/******************************************************************************************************/

		std::unique_ptr<Timer_colored> timer(new Timer_colored(timer_prompt));
		system("clear");
		std::string process_name(argv[0]);
		set_process_name(process_name);
		std::cout << process_prompt << process_name << " is being executed..." << std::endl;

		/////////////////////////////////////////////////
		// Print out where the program root is located //
		/////////////////////////////////////////////////
		
		char buffer[256];
		char *ppath = getcwd(buffer, sizeof(buffer));
		if(ppath)
		{
			std::cout << debug_prompt << "Buffer: " << buffer << std::endl;
		}

		fitter();
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << e.what() << std::endl;
	}
	return 0;
}

void set_process_name(std::string& process_name)
{
	if(process_name.find("./", 0, 2) != std::string::npos)
	{
		process_name = process_name.substr(process_name.find("./", 0, 2) + 2);
	}
	else
	{
		std::cerr << error_prompt << "Process name seems to be corrupted..." << std::endl;
	}
}