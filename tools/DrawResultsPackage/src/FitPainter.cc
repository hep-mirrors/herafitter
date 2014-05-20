#include "FitPainter.h"

#include "Outdir.h"
#include "CommandParser.h"
#include "DrawLogo.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <stdlib.h>

#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TLine.h>
#include <math.h>
#include <TMath.h>

bool FitPainter()
{
  //List of all datasets
  vector <int> dataindexlist = chi2list();

  if (chi2map.size() < 1)
    {
      cout << "Error: Chi2 data not found, cannot make chi2 table" << endl;
      return true;
    }

  int ndata = dataindexlist.size() + 2;

  float chi2[opts.labels.size()][ndata];
  float chi2_00[opts.labels.size()][ndata];
  int dof[opts.labels.size()][ndata];
  string dirname[opts.labels.size()];
  for (int i = 0; i < opts.labels.size(); i++)
    for (int d = 0; d < ndata; d++)
      {
	chi2[i][d] = 0;
	chi2_00[i][d] = 0;
	dof[i][d] = 0;
      }

  for (int d = 0; d < opts.labels.size(); d++)
    {
      dirname[d] = opts.labels[d];
      while (dirname[d].find("/") != string::npos)
	dirname[d].replace(dirname[d].find("/"), 1, " ");
      while (dirname[d].find("_") != string::npos)
	dirname[d].replace(dirname[d].find("_"), 1, " ");
    }

  //loop on dataset indexes
  int dat = 0;
  for (vector<int>::iterator dit = dataindexlist.begin(); dit != dataindexlist.end(); dit++)
    {
      //loop on labels (directories)
      int dir = 0;
      for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
	{
	  chi2[dir][dat] = chi2map[*itl].chi2list[*dit].chi2;
	  chi2_00[dir][dat] = chi2map[*itl].chi2list[*dit].chi2_00;
	  dof[dir][dat] = chi2map[*itl].chi2list[*dit].dof;
	  dir++;
	}
      dat++;
    }

  //Correlated chi2
  //loop on labels (directories)
  int dir = 0;
  for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
    {
      chi2[dir][dat] = chi2map[*itl].chi2corr.chi2;
      chi2_00[dir][dat] = chi2map[*itl].chi2corr.chi2_00;
      dof[dir][dat] = chi2map[*itl].chi2corr.dof;
      dir++;
    }
  dat++;

  //Total chi2
  //loop on labels (directories)
  dir = 0;
  for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
    {
      chi2[dir][dat] = chi2map[*itl].chi2tot.chi2;
      chi2_00[dir][dat] = chi2map[*itl].chi2tot.chi2_00;
      dof[dir][dat] = chi2map[*itl].chi2tot.dof;
      dir++;
    }

  //write table
  FILE *ftab = fopen((opts.outdir + "chi2.tex").c_str(), "w");
  if (!ftab)
    {
      cout << "Cannot open chi2 tex file" << endl;
      return true;
    }

  int points;
  if (opts.labels.size() >= 1)
    points = 14;
  if (opts.labels.size() >= 3)
    points = 12;
  if (opts.labels.size() >= 4)
    points = 11;
  if (opts.labels.size() >= 5)
    points = 9;

  //points to cm conversion
  float cm = 0.035277778 * points * 5.2;

  fprintf(ftab,"\\documentclass[%dpt]{report}\n", points);
  fprintf(ftab,"\\usepackage{extsizes}\n");
  fprintf(ftab,"\\usepackage[paperwidth=21cm,paperheight=21cm,left=0.2cm,right=0.2cm]{geometry}\n");
  fprintf(ftab,"\\usepackage[scaled]{helvet}\n");
  fprintf(ftab,"\\renewcommand\\familydefault{\\sfdefault}\n");
  fprintf(ftab,"\\pagestyle{empty}\n");

  fprintf(ftab,"\\usepackage{booktabs}\n");
  fprintf(ftab,"\\usepackage[table]{xcolor}\n");
  fprintf(ftab,"\\definecolor{lightgray}{gray}{0.85}\n");
  fprintf(ftab,"\\begin{document}\n");

  fprintf(ftab,"\\begin{table}\n");
  fprintf(ftab,"  \\begin{center}\n");
  fprintf(ftab,"  \\rowcolors{2}{}{lightgray}\n");
  fprintf(ftab,"    \\begin{tabular}{l");
  for (int i = 0; i < opts.labels.size(); i++)
    fprintf(ftab,"p{%.2fcm}", cm);
  fprintf(ftab,"}\n");
  fprintf(ftab,"      \\toprule\n");

  fprintf(ftab,"  Dataset  ");
  for (int i = 0; i < opts.labels.size(); i++)
    fprintf(ftab,"   & %s", dirname[i].c_str());
  fprintf(ftab,"  \\\\ \n");
  fprintf(ftab,"      \\midrule\n");

  for (vector<int>::iterator dit = dataindexlist.begin(); dit != dataindexlist.end(); dit++)
    {
      fprintf(ftab,"  %s ", finddataname(*dit).c_str());
      for (int i = 0; i < opts.labels.size(); i++)
	if (dof[i][dit-dataindexlist.begin()] != 0 || chi2[i][dit-dataindexlist.begin()] != 0 )
	  {
	    if (chi2_00[i][dit-dataindexlist.begin()] >= 0)
	      fprintf(ftab,"& %s (%s) / %d", Round(chi2[i][dit-dataindexlist.begin()])[0].c_str(), Round(chi2_00[i][dit-dataindexlist.begin()])[0].c_str(), dof[i][dit-dataindexlist.begin()]);
	    else
	      fprintf(ftab,"& %s / %d", Round(chi2[i][dit-dataindexlist.begin()])[0].c_str(), dof[i][dit-dataindexlist.begin()]);
	  }
	else
	  fprintf(ftab,"& - ");
      fprintf(ftab,"  \\\\ \n");
    }

  fprintf(ftab,"  Correlated $\\chi^2$  ");
  for (int i = 0; i < opts.labels.size(); i++)
    if (chi2_00[i][ndata-2] >= 0)
      fprintf(ftab,"& %s (%s)", Round(chi2[i][ndata-2])[0].c_str(), Round(chi2_00[i][ndata-2])[0].c_str());
    else
      fprintf(ftab,"& %s", Round(chi2[i][ndata-2])[0].c_str());
  fprintf(ftab,"  \\\\ \n");

  fprintf(ftab,"  \\rowcolor{white}\n");
  bool pdfunc = false;
  fprintf(ftab,"      \\midrule\n");
  fprintf(ftab,"  Total $\\chi^2$ / dof  ");
  for (int i = 0; i < opts.labels.size(); i++)
    if (chi2_00[i][ndata-1] >= 0)
      {
	fprintf(ftab,"& %s (%s) / %d", Round(chi2[i][ndata-1])[0].c_str(), Round(chi2_00[i][ndata-1])[0].c_str(), dof[i][ndata-1]);
	pdfunc = true;
      }
    else
      fprintf(ftab,"& %s / %d", Round(chi2[i][ndata-1])[0].c_str(), dof[i][ndata-1]);
  fprintf(ftab,"  \\\\ \n");

  fprintf(ftab,"      \\midrule\n");
  fprintf(ftab,"  $\\chi^2$ p-value  ");
  for (int i = 0; i < opts.labels.size(); i++)
    fprintf(ftab,"& %.2f ", TMath::Prob(chi2[i][ndata-1], dof[i][ndata-1]));
  fprintf(ftab,"  \\\\ \n");


  fprintf(ftab,"      \\bottomrule\n");
  fprintf(ftab,"    \\end{tabular}\n");
  fprintf(ftab,"  \\end{center}\n");
  fprintf(ftab,"\\end{table}\n");
  if (pdfunc)
    fprintf(ftab,"$\\chi^2$ with (w/o) PDF uncertainties  \n");


  fprintf(ftab,"\n");
  fprintf(ftab,"\\end{document}\n");
  fclose(ftab);

  string command = "pdflatex -interaction=batchmode -output-directory=" + opts.outdir + " " + opts.outdir + "chi2.tex > /dev/null";
  //debug latex
  //string command = "pdflatex  -output-directory=" + opts.outdir + " " + opts.outdir + "chi2.tex";
  bool latexcmd = system(command.c_str());
  if (latexcmd)
    {
      cout << "pdflatex error in making: chi2.pdf" << endl;
      cout << "check the error with:" << endl;
      cout << command << endl;
    }

  //cleanup
  string clean = "rm -f " 
    + opts.outdir + "chi2.aux " 
    + opts.outdir + "chi2.log " 
    + opts.outdir + "chi2.nav " 
    + opts.outdir + "chi2.out " 
    + opts.outdir + "chi2.snm " 
    + opts.outdir + "chi2.toc ";
  system(clean.c_str());

  return latexcmd;
}
