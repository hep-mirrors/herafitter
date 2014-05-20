#ifndef CommandParser_h
#define CommandParser_h
#include <string>
#include <vector>
#include <map>
#include <iostream>

extern float txtsize;
extern float offset;
extern float lmarg;
extern float rmarg;
extern float tmarg;
extern float bmarg;
extern float marg0;

using namespace std;

//Service functions
extern vector<string> Round(double value, double error = 0);

class CommandParser
{
 public:
  CommandParser() {};
  CommandParser(int argc, char **argv);

  //pdf options
  bool dobands, filledbands, asymbands, logx;
  float rmin, rmax;
  double xmin, xmax;
  bool abserror, relerror;
  bool q2all;

  //data pulls options
  bool therr, points;
  string theorylabel;
  bool twopanels,threepanels;
  bool onlytheory;
  bool ratiototheory;
  bool diff;

  //shifts options
  int spp, shgth;
  bool adjshift;

  //tables options
  bool chi2nopdf;

  //general options
  bool splitplots;
  string ext;
  string format;
  bool root;
  string outdir;
  vector <string> dirs, labels;
  map <string, int> colors;
  map <string, int> styles;
  map <string, int> markers;
  int col[6];
  int styl[6];
  int mark[6];
  float lwidth;
  float resolution, pagewidth;
  bool nodata;
  bool nopdfs;
  bool noshifts;
  bool notables;
  bool version, drawlogo;
  bool cms, cmspreliminary;
  bool atlas, atlaspreliminary, atlasinternal;
  bool cdfiipreliminary;

 private:
  vector <string> allargs;
  void help(void)
  {
    cout << endl;
    cout << "program usage:" << endl;
    cout << allargs[0] << " [options] dir1[:label1] [dir2:[label2]] [...]" << endl;
    cout << endl;
    cout << "First directory is used as reference for PDF ratio plots" << endl;
    cout << "and to display data in data plots." << endl;
    cout << "Directory labels are used in the legends of the plots, to add spaces and" << endl;
    cout << "special characters to the labels use quotation marks '' (ex. dir:'HERA I')." << endl;
    cout << "To specify greek letters and latex commands in the labels use the ROOT notation" << endl;
    cout << "(#alpha #bar{u})." << endl;
    cout << "It is possible to specify up to 6 directories, you need to specify at least one directory." << endl;
    cout << endl;
    cout << "general options:" << endl;
    cout << "\t --help" << endl;
    cout << "\t \t Show this help" << endl;
    cout << "\t --outdir <output directory>" << endl;
    cout << "\t \t Specify output directory" << endl;
    cout << "\t --eps" << endl;
    cout << "\t \t Plots are saved in eps format" << endl;
    cout << "\t --root" << endl;
    cout << "\t \t Save all the plots in a root file" << endl;
    cout << "\t --splitplots-eps" << endl;
    cout << "\t \t Produce also additional eps files for each plot" << endl;
    cout << "\t --splitplots-pdf" << endl;
    cout << "\t \t Produce also additional eps and pdf files for each plot" << endl;
    cout << "\t --splitplots-png" << endl;
    cout << "\t \t Produce additional png files for each plot" << endl;
    cout << "\t --colorpattern <1-3>" << endl;
    cout << "\t \t Select among 3 additional color patterns" << endl;
    cout << "\t --thicklines" << endl;
    cout << "\t \t Thicker lines in all plots (better for small plots in slides)" << endl;
    cout << "\t --lowres" << endl;
    cout << "\t \t Low resolution logo (smaller file)" << endl;
    cout << "\t --highres" << endl;
    cout << "\t \t High resolution logo (paper quality)" << endl;
    cout << "\t --no-version" << endl;
    cout << "\t \t Do not show version on logo" << endl;
    cout << "options for pdf plots:" << endl;
    cout << "\t --no-pdfs" << endl;
    cout << "\t \t PDF plots are not produced" << endl;
    cout << "\t --bands" << endl;
    cout << "\t \t Draw PDF uncertainty band" << endl;
    cout << "\t --asymbands" << endl;
    cout << "\t \t PDF bands are not symmetrised" << endl;
    cout << "\t --filledbands" << endl;
    cout << "\t \t Filled uncertainty bands, usefull for sensitivity studies" << endl;
    cout << "\t --ratiorange min:max" << endl;
    cout << "\t \t Specify y axis range in PDF ratio plots" << endl;
    cout << "\t --xrange min:max" << endl;
    cout << "\t \t Specify x axis range in PDF plots," << endl;
    cout << "\t \t default minimum and maximum x are determined by settings" << endl;
    cout << "\t \t in the first reference directory" << endl;
    cout << "\t --no-logx" << endl;
    cout << "\t \t Linear x scale in PDF plots" << endl;
    cout << "\t --absolute-errors" << endl;
    cout << "\t \t Plot absolute pdf uncertainties centered around 0 in PDF ratio plots" << endl;
    cout << "\t --relative-errors" << endl;
    cout << "\t \t Plot relative pdf uncertainties centered around 1 in PDF ratio plots" << endl;
    cout << "\t --q2all" << endl;
    cout << "\t \t Plot PDF at all stored values of Q2. By default PDF are plotted only at the starting scale Q0" << endl;
    cout << "options for data plots:" << endl;
    cout << "\t --no-data" << endl;
    cout << "\t \t Data plots are not produced" << endl;
    cout << "\t --therr" << endl;
    cout << "\t \t Plot theory errors if availables" << endl;
    cout << "\t --points" << endl;
    cout << "\t \t Plot theory as displaced marker points (with vertical error bars) instead of continous lines (with dashed error area)" << endl;
    cout << "\t --theory <label>" << endl;
    cout << "\t \t Change the \"Theory\" legend to the specified label" << endl;
    cout << "\t --2panels" << endl;
    cout << "\t \t Plot additional right bottom panels with pulls" << endl;
    cout << "\t --3panels" << endl;
    cout << "\t \t Plot additional right mid panels with theory+shifts" << endl;
    cout << "\t --only-theory" << endl;
    cout << "\t \t Do not plot data" << endl;
    cout << "\t --ratio-to-theory" << endl;
    cout << "\t \t Use theory as reference for ratio plots" << endl;
    cout << "\t --diff" << endl;
    cout << "\t \t Plot difference of theory-data instead of ratio theory/data" << endl;
    cout << "options for shifts plots:" << endl;
    cout << "\t --no-shifts" << endl;
    cout << "\t \t Shifts plots are not produced" << endl;
    cout << "\t --shifts-per-plot <N>" << endl;
    cout << "\t \t Number of shifts shown in each plot, default is 30, maximum is 40" << endl;
    cout << "\t --shifts-heigth <N>" << endl;
    cout << "\t \t Heigth reserved for each shift in points, minimum is 20, maximum is 200" << endl;
    cout << "options for tables:" << endl;
    cout << "\t --no-tables" << endl;
    cout << "\t \t Tables are not produced" << endl;
    cout << "\t --chi2-nopdf-uncertainties" << endl;
    cout << "\t \t When chi2 is evaluated with the LHAPDFError routine, this option will add to the chi2 table the chi2 evaluated without PDF uncertainties within brackets" << endl;
    cout << endl;
    cout << "\t to set axis titles, axis range and log scales add PlotDesc options in the data file" << endl;
    cout << "\t Example:" << endl;
    cout << "\t &PlotDesc" << endl;
    cout << "\t    PlotN = 1 ! SubPlot index" << endl;
    cout << "\t    PlotDefColumn = 'y2' ! Variable used to divide the data in SubPlots" << endl;
    cout << "\t    PlotDefValue = 0., 5.! Ranges of PlotDefColumn used to divide the data in SubPlots" << endl;
    cout << "\t    PlotVarColumn = 'x'! Variable providing bin center information (use only if bin edges are missing)" << endl;
    cout << "\t    PlotOptions(1)  = 'Experiment:ATLAS@Title: pp #rightarrow Z@XTitle: y_{Z} @YTitle: d#sigma/dy_{Z} [pb]  @ExtraLabel:#int L = 100 fb^{-1}@Xmin:0.0@Xmax:2.5@Xlog@Ylog@YminR:0.91@YmaxR:1.09'" 
	 << endl;
    cout << "\t &End" << endl;
    cout << endl;
  };  
};

extern CommandParser opts;
#endif
