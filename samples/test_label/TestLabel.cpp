#include <cups/cups.h>
#include <cups/ppd.h>
#include <string>
#include <stdio.h>
#include <map>
#include <cairo/cairo.h>
#include <exception>
#include "CairoUtils.h"

using namespace std;

const char* PNGFileName = "label.png";

class Error: public exception
{
public:
  Error(const string& Message): exception(), Message_(Message) {}
  virtual ~Error() throw() {}
  virtual const char* what() const throw() { return Message_.c_str(); }
private:
  string Message_;
};

map <string, string> gPaperNames;
typedef pair<string, string> str_pair;

static void
FindPapersOptions(ppd_option_t& o)
{
  if (!strcmp(o.keyword, "PageSize"))
  {
    for (int i = 0; i < o.num_choices; ++i)
    {
      ppd_choice_t& c = o.choices[i];

      gPaperNames.insert(str_pair(c.choice, c.text));
    }
  }
}

static void
FindPapersGroup(ppd_group_t& g)
{
  for (int i = 0; i < g.num_options; ++i)
    FindPapersOptions(g.options[i]);
}

static void
FindPapers(ppd_group_t* group, int num_groups)
{
  for (int i = 0; i < num_groups; ++i)
    FindPapersGroup(group[i]);
}


void
CreateLabelImage(int Width, int Height)
{
  CairoSurfacePtr Surface(cairo_image_surface_create(CAIRO_FORMAT_RGB24, Width, Height));
  if (!*Surface)
    throw Error("Unable to create cairo surface");


  CairoPtr c(cairo_create(Surface));
  if (!*c)
    throw Error("Unable to create cairo_t");

  // setup cairo
  cairo_set_antialias(c, CAIRO_ANTIALIAS_NONE);
  
  // clear image
  cairo_set_source_rgb(c, 1, 1, 1);
  cairo_paint(c);

  // draw boundrect
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 5);
  cairo_rectangle(c, 5, 5, Width - 10, Height - 10);
  cairo_stroke(c);

  // load an image from the file
  cairo_save(c);
  CairoSurfacePtr Image(cairo_image_surface_create_from_png("tel.png"));
  if (!*Image)
    throw Error("Unable to load image file");

  cairo_scale(c, 0.5, 0.5);
  cairo_set_source_surface(c, Image, 30, 30);
  cairo_paint(c);
  cairo_restore(c);

  // draw text
  cairo_select_font_face(c, "courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(c, 50);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);
  const char* lines[4] = {
    "Jim Best",
    "Good, Bedder, and Best",
    "900 Park Ave",
    "New York, NY 10021-0231" };

  cairo_text_extents_t te;
  double x = 250;
  double y = 50;
  for(int i = 0; i < 4; ++i)
  {
    cairo_text_extents(c, lines[i], &te);

    x = 1080 - te.width;
    cairo_move_to(c, x, y);
    cairo_show_text(c, lines[i]);
    cairo_stroke(c);

    y += te.height + 10;
  }

  // draw horiz line
  cairo_set_line_width(c, 5);
  cairo_move_to(c, 30, 200);
  cairo_line_to(c, 1080, 200);
  cairo_stroke(c);

  // draw barcode - load from fle also
  cairo_save(c);
  CairoSurfacePtr Barcode(cairo_image_surface_create_from_png("barcode.png"));
  if (!*Barcode)
    throw Error("Unable to load barcode file");

  cairo_set_source_surface(c, Barcode, 30, 230);
  cairo_paint(c);
  cairo_restore(c);

  
  // draw address 2
  cairo_select_font_face(c, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(c, 50);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);
  const char* lines2[4] = {
    "Dr. Robert J. Block",
    "Alta Mira Hospital",
    "200 Madison Ave",
    "Montgomery, AL 36104-3626" };

  x = 30;
  y = 320;
  for(int i = 0; i < 4; ++i)
  {
    cairo_text_extents(c, lines2[i], &te);

    cairo_move_to(c, x, y);
    cairo_show_text(c, lines2[i]);
    cairo_stroke(c);

    y += te.height;
  }

  // draw a photo
  cairo_save(c);
  CairoSurfacePtr Photo(cairo_image_surface_create_from_png("photo.png"));
  if (!*Photo)
    throw Error("Unable to load photo file");

  //cairo_scale(c, 0.5, 0.5);
  cairo_set_source_surface(c, Photo, 850, 330);
  cairo_paint(c);
  cairo_restore(c);
  
  // draw small description
  cairo_select_font_face(c, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(c, 20);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);
    cairo_move_to(c, 30, 640);
    cairo_show_text(c, "Printed on Linux using DYMO CUPS drivers and Cairo graphics library");
    cairo_stroke(c);
  
  
  // save to file
  if (cairo_surface_write_to_png(Surface, PNGFileName) != CAIRO_STATUS_SUCCESS)
    throw Error("Unable to write to PNG file");
}

int main(int argc, char** argv)
{
  try
  {
    if (argc < 2)
      throw Error("Usage: test_label <PrinterName>");


    printf("Please Insert '30256 Shipping' paper. Press 'c' to continue, 'a' to abort: ");
    int ch = 0;
    ch = getchar();

    if (ch == 'a')
      return 0;

    int Width = 1112;
    int Height = 664;
    
    CreateLabelImage(Width, Height);

    int             num_options = 0;
    cups_option_t*  options = NULL;

    num_options = cupsAddOption("PageSize", "w167h288", num_options, &options);
    num_options = cupsAddOption("scaling", "100", num_options, &options);
    //num_options = cupsAddOption("landscape", "yes", num_options, &options);
    num_options = cupsAddOption("orientation-requested", "4", num_options, &options);
    num_options = cupsAddOption("DymoHalftoning", "ErrorDiffusion", num_options, &options);
    num_options = cupsAddOption("DymoPrintQuality", "Graphics", num_options, &options);
    //    num_options = cupsAddOption("orientation-requested", "1", num_options, &options);
      

    cupsPrintFile(argv[1], PNGFileName, "Test print with Cairo", num_options, options);
    cupsFreeOptions(num_options, options);
  
    return 0;
  }
  catch(std::exception& e)
  {
    fprintf(stderr, e.what());
    fprintf(stderr, "\n");
    return 1;
  }
}
