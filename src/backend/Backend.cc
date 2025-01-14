#include "Backend.h"
#include <string>

Backend::Backend(std::string in_name, Napi::CallbackInfo& info)
  : name(in_name)
  , width(info[0].IsNumber() ? info[0].As<Napi::Number>().Int32Value() : 0)
  , height(info[1].IsNumber() ? info[1].As<Napi::Number>().Int32Value() : 0)
{}

Backend::~Backend()
{
  Backend::destroySurface();
}


void Backend::setCanvas(Canvas* _canvas)
{
  this->canvas = _canvas;
}


cairo_surface_t* Backend::recreateSurface()
{
  this->destroySurface();

  return this->createSurface();
}

DLL_PUBLIC cairo_surface_t* Backend::getSurface() {
  if (!surface) createSurface();
  return surface;
}

void Backend::destroySurface()
{
  if(this->surface)
  {
    cairo_surface_destroy(this->surface);
    this->surface = NULL;
  }
}


std::string Backend::getName()
{
  return name;
}

int Backend::getWidth()
{
  return this->width;
}
void Backend::setWidth(int width_)
{
  this->width = width_;
  this->recreateSurface();
}

int Backend::getHeight()
{
  return this->height;
}
void Backend::setHeight(int height_)
{
  this->height = height_;
  this->recreateSurface();
}

bool Backend::isSurfaceValid(){
  bool hadSurface = surface != NULL;
  bool isValid = true;

  cairo_status_t status = cairo_surface_status(getSurface());

  if (status != CAIRO_STATUS_SUCCESS) {
    error = cairo_status_to_string(status);
    isValid = false;
  }

  if (!hadSurface)
    destroySurface();

  return isValid;
}


BackendOperationNotAvailable::BackendOperationNotAvailable(Backend* backend,
  std::string operation_name)
  : backend(backend)
  , operation_name(operation_name)
{
  msg = "operation " + operation_name +
    " not supported by backend " + backend->getName();
};

BackendOperationNotAvailable::~BackendOperationNotAvailable() throw() {};

const char* BackendOperationNotAvailable::what() const throw()
{
  return msg.c_str();
};
