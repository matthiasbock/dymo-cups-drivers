#ifndef __CAIRO_UTILS__
#define __CAIRO_UTILS__

#include <cairo/cairo.h>


class CairoSurfacePtr
{
public:
  CairoSurfacePtr(cairo_surface_t* ptr): ptr_(ptr) {}
  ~CairoSurfacePtr() { cairo_surface_destroy(ptr_); }

  cairo_surface_t* operator*() { return ptr_; }
  operator cairo_surface_t* () { return ptr_; }
private:
  cairo_surface_t* ptr_;
  
};

class CairoPtr
{
public:
  CairoPtr(cairo_t* ptr): ptr_(ptr) {}
  ~CairoPtr() { cairo_destroy(ptr_); }

  cairo_t* operator*() { return ptr_; }
  operator cairo_t* () { return ptr_; }

private:
  cairo_t* ptr_;
  
};

#endif
