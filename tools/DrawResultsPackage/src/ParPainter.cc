#include "ParPainter.h"

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

bool ParPainter()
{
  //List of all parameters
  vector <int> parindexlist = parlist();
  if (parindexlist.size() == 0)
    return true;

  int ndata = parindexlist.size();

  float par[opts.labels.size()][ndata];
  float unc[opts.labels.size()][ndata];
  string dirname[opts.labels.size()];
  for (int i = 0; i < opts.labels.size(); i++)
    for (int d = 0; d < ndata; d++)
      {
	par[i][d] = 0;
	unc[i][d] = 0;
      }

  for (int d = 0; d < opts.labels.size(); d++)
    {
      dirname[d] = opts.labels[d];
      while (dirname[d].find("/") != string::npos)
	dirname[d].replace(dirname[d].find("/"), 1, " ");
      while (dirname[d].find("_") != string::npos)
	dirname[d].replace(dirname[d].find("_"), 1, " ");
    }

  //loop on parameters
  int p = 0;
  for (vector<int>::iterator pit = parindexlist.begin(); pit != parindexlist.end(); pit++)
    {
      //loop on labels (directories)
      int dir = 0;
      for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
	{
	  par[dir][p] = parmap[*itl].parlist[*pit].value;
	  unc[dir][p] = parmap[*itl].parlist[*pit].error;
	  dir++;
	}
      p++;
    }

  //write table
  FILE *ftab = fopen((opts.outdir + "par.tex").c_str(), "w");
  if (!ftab)
    {
      cout << "Cannot open par tex file" << endl;
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

  float cm = 0.035277778 * points * 9;

  fprintf(ftab,"\\documentclass[%dpt]{report}\n", points);
  fprintf(ftab,"\\usepackage{extsizes}\n");
  fprintf(ftab,"\\usepackage[paperwidth=21cm,paperheight=21cm,left=0.2cm,right=0.2cm]{geometry}\n");
  fprintf(ftab,"\\usepackage[scaled]{helvet}\n");
  fprintf(ftab,"\\renewcommand\\familydefault{\\sfdefault}\n");
  fprintf(ftab,"\\pagestyle{empty}\n");

  fprintf(ftab,"\\usepackage{booktabs}\n");
  fprintf(ftab,"\\usepackage[table]{xcolor}\n");
  fprintf(ftab,"\\definecolor{lightgray}{gray}{0.85}\n");
  fprintf(ftab,"\\usepackage{color}\n");
  fprintf(ftab,"\\begin{document}\n");

  fprintf(ftab,"\\begin{table}\n");
  fprintf(ftab,"  \\begin{center}\n");
  fprintf(ftab,"  \\rowcolors{2}{}{lightgray}\n");
  fprintf(ftab,"    \\begin{tabular}{l");
  for (int i = 0; i < opts.labels.size(); i++)
    fprintf(ftab,"p{%.2fcm}", cm);
  fprintf(ftab,"}\n");
  fprintf(ftab,"      \\toprule\n");
  fprintf(ftab," Parameter  ");
  for (int i = 0; i < opts.labels.size(); i++)
    fprintf(ftab," & %s", dirname[i].c_str());
  fprintf(ftab,"  \\\\ \n");
  fprintf(ftab,"      \\midrule\n");

  for (vector<int>::iterator pit = parindexlist.begin(); pit != parindexlist.end(); pit++)
    {
      fprintf(ftab,"  %s ", findparname(*pit).c_str());
      for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
	{
	  int l = itl-opts.labels.begin();
	  int p = pit-parindexlist.begin();
	  if (unc[l][p] != 0 && par[l][p] != 0 )
	    {
	      if (parmap[*itl].fitstatus != "pos-def-forced")
		fprintf(ftab,"& %s $\\pm$ %s", Round(par[l][p], unc[l][p])[0].c_str(), Round(unc[l][p], unc[l][p])[1].c_str());
	      else
		fprintf(ftab,"& %s $\\pm$ {\\color{red}{ %s }}", Round(par[l][p], unc[l][p])[0].c_str(), Round(unc[l][p], unc[l][p])[1].c_str());
	    }
	  else if (par[l][p] != 0)
	    fprintf(ftab,"& \\textcolor{blue}{ %s }", Round(par[l][p], par[l][p]/100.)[0].c_str());
	  else
	    fprintf(ftab,"& - ");
	}
      fprintf(ftab,"  \\\\ \n");
    }

  fprintf(ftab,"      \\midrule\n");
  fprintf(ftab,"  \\rowcolor{white}\n");
  fprintf(ftab," Fit status  ");
  for (vector<string>::iterator itl = opts.labels.begin(); itl != opts.labels.end(); itl++)
    fprintf(ftab," & %s", parmap[*itl].fitstatus.c_str());
  fprintf(ftab,"  \\\\ \n");

  fprintf(ftab,"      \\bottomrule\n");
  fprintf(ftab,"    \\end{tabular}\n");
  fprintf(ftab,"  \\end{center}\n");
  fprintf(ftab,"\\end{table}\n");

  fprintf(ftab,"\n");
  fprintf(ftab,"\\end{document}\n");
  fclose(ftab);

  string command = "pdflatex -interaction=batchmode -output-directory=" + opts.outdir + " " + opts.outdir + "par.tex >/dev/null";
  //debug latex
  //string command = "pdflatex  -output-directory=" + opts.outdir + " " + opts.outdir + "par.tex";
  bool latexcmd = system(command.c_str());
  if (latexcmd)
    {
      cout << "pdflatex error in making: par.pdf" << endl;
      cout << "check the error with:" << endl;
      cout << command << endl;
    }

  //cleanup
  string clean = "rm -f " 
    + opts.outdir + "par.aux " 
    + opts.outdir + "par.log " 
    + opts.outdir + "par.nav " 
    + opts.outdir + "par.out " 
    + opts.outdir + "par.snm " 
    + opts.outdir + "par.toc ";
  system(clean.c_str());

  return latexcmd;
}
