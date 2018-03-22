#include <cups/cups.h>
#include <cups/ppd.h>
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

map <string, string> gPaperNames;
typedef pair<string, string> str_pair;

static void
FindPapersOptions(ppd_option_t& o)
{
  //  cout << "Option " << o.keyword << endl;
  if (!strcmp(o.keyword, "PageSize"))
  {
    for (int i = 0; i < o.num_choices; ++i)
    {
      ppd_choice_t& c = o.choices[i];

      gPaperNames.insert(str_pair(c.choice, c.text));
      //      cout << c.choice << " " << c.text << endl;
    }
  }
}

static void
FindPapersGroup(ppd_group_t& g)
{
  //  cout << g.name << " " << g.text << " " << g.num_options << endl;

  for (int i = 0; i < g.num_options; ++i)
    FindPapersOptions(g.options[i]);
}

static void
FindPapers(ppd_group_t* group, int num_groups)
{
  for (int i = 0; i < num_groups; ++i)
    FindPapersGroup(group[i]);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cerr << "Usage: paperlist <PrinterName>" << endl;
    return 1;
  }


  const char* ppdFileName = cupsGetPPD(argv[1]);
  if (!ppdFileName)
  {
    cerr << "Unknown printer '" << argv[1] << "'" << endl;
    return 2;
  }

  ppd_file_t* ppd = ppdOpenFile(ppdFileName);
  if (!ppd)
  {
    cerr << "unable to open ppd file '" << ppdFileName << "'" << endl;
    return 3;
  }

  FindPapers(ppd->groups, ppd->num_groups);


  cout << setw(30) << setiosflags(ios_base::left) << "Paper Name" << resetiosflags(ios_base::left)
       << setw(10) << "Width"
       << setw(10) << "Height"
       << setw(10) << "Left"
       << setw(10) << "Bottom"
       << setw(10) << "Right"
       << setw(10) << "Top"
       << endl;

  cout << setw(30) << setiosflags(ios_base::left) << "----- ----" << resetiosflags(ios_base::left)
       << setw(10) << "-----"
       << setw(10) << "------"
       << setw(10) << "----"
       << setw(10) << "---"
       << setw(10) << "-----"
       << setw(10) << "------"
       << endl;
    
    
  for (int i = 0; i < ppd->num_sizes; ++i)
  {
    ppd_size_t size = ppd->sizes[i];

    cout << setw(30) << setiosflags(ios_base::left) << gPaperNames[size.name] << setprecision(2) << setiosflags(ios_base::fixed) << resetiosflags(ios_base::left)
         << setw(10) << size.width
         << setw(10) << size.length
         << setw(10) << size.left
         << setw(10) << size.bottom
         << setw(10) << size.right
         << setw(10) << size.top
         << endl;

  }
  

  ppdClose(ppd);
  
  return 0;
}
