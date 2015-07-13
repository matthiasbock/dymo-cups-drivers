#include <cups/cups.h>
#include <cups/ppd.h>
#include <string>
#include <stdio.h>
#include <map>
#include <cairo/cairo.h>
#include <exception>
#include "CairoUtils.h"

using namespace std;



class Error: public exception
{
public:
  Error(const string& Message): exception(), Message_(Message) {}
  virtual ~Error() throw() {}
  virtual const char* what() const throw() { return Message_.c_str(); }
private:
  string Message_;
};


static int
GetPrinterResolution(ppd_group_t* group, int num_groups)
{
  for (int i = 0; i < num_groups; ++i)
  {
    ppd_group_t g = group[i];
    for (int j = 0; j < g.num_options; ++j)
    {
      ppd_option_t o = g.options[j];

      if (!strcmp(o.keyword, "Resolution"))
      {
        ppd_choice_t c = o.choices[0];

        return atoi(c.choice);
      }
    }  
  }

  return 0;
}

void
CreateBoundsImage(int Width, int Height, const string& Text, string& FileName)
{
  FileName = "test.png";
  
  CairoSurfacePtr Surface(cairo_image_surface_create(CAIRO_FORMAT_RGB24, Width, Height));
  if (!*Surface)
    throw Error("Unable to create cairo surface");


  CairoPtr c(cairo_create(Surface));
  if (!*c)
    throw Error("Unable to create cairo_t");

  //setup Cairo
  cairo_set_antialias(c, CAIRO_ANTIALIAS_NONE);
  
  
  // clear image
  cairo_set_source_rgb(c, 1, 1, 1);
  cairo_paint(c);

  // draw rect
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 3);
  cairo_rectangle(c, 3, 3, Width - 6, Height - 6);
  cairo_move_to(c, 3, 3);
  cairo_line_to(c, Width - 3, Height - 3);
  cairo_move_to(c, 3, Height - 3);
  cairo_line_to(c, Width - 3, 3);
  cairo_stroke(c);


  // draw text
  cairo_select_font_face(c, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);

  cairo_text_extents_t te;
  for(int FontSize = 100; ;FontSize -= 10)
  {
    cairo_set_font_size(c, FontSize);

    cairo_text_extents(c, Text.c_str(), &te);

    if (Width > te.width)
      break;
  }
  
  //cairo_move_to(c, 10, 10);
  //cairo_show_text(c, "Hello");
  cairo_move_to(c, (Width - te.width) / 2, (Height + te.height) / 2);
  cairo_text_path(c, Text.c_str());
  
  cairo_stroke(c);

  // save to file
  if (cairo_surface_write_to_png(Surface, FileName.c_str()) != CAIRO_STATUS_SUCCESS)
    throw Error("Unable to write to PNG file");
}

int main(int argc, char** argv)
{
  try
  {
    if (argc < 2)
      throw Error("Usage: customPaper <PrinterName>");


    const char* ppdFileName = cupsGetPPD(argv[1]);
    if (!ppdFileName)
      throw Error(string("Unknown printer '") + argv[1] + "'");

    ppd_file_t* ppd = ppdOpenFile(ppdFileName);
    if (!ppd)
      throw Error(string("Unable to open ppd file '") + ppdFileName + "'");

    
    int Resolution = GetPrinterResolution(ppd->groups, ppd->num_groups);

    // custom paper definition
    // use 12 mm paper size in points with 5 inch long
    float CustomPaperPrintableWidth = 28.8;
    float CustomPaperPrintableHeight = 5 * 72;
  
    int Width = int(CustomPaperPrintableWidth * Resolution / 72);
    int Height = int(CustomPaperPrintableHeight * Resolution / 72);
    bool Landscape = false;
    
    string FileName;
    if (Height > Width)
    {
      Landscape = true;
      int t = Width;
      Width = Height;
      Height = t;
    }
    CreateBoundsImage(Width, Height, "Custom paper", FileName);

    int             num_options = 0;
    cups_option_t*  options = NULL;

    // PageSize value for custom paper
    char PageSizeValue[250];
    sprintf(PageSizeValue, "Custom.%fx%f", CustomPaperPrintableWidth, CustomPaperPrintableHeight);

  
    num_options = cupsAddOption("PageSize", PageSizeValue, num_options, &options);
    num_options = cupsAddOption("MediaType", "12mm", num_options, &options);
    num_options = cupsAddOption("scaling", "100", num_options, &options);
    if (Landscape)
      num_options = cupsAddOption("landscape", "yes", num_options, &options);
      

    cupsPrintFile(argv[1], FileName.c_str(), "Test print with Cairo", num_options, options);
    cupsFreeOptions(num_options, options);
  

    ppdClose(ppd);
  
    return 0;
  }
  catch(std::exception& e)
  {
    fputs(e.what(), stderr);
    fputs("\n", stderr);
    return 1;
  }
}





