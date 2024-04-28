// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CanvasGradient.h"

#include "Canvas.h"
#include "color.h"
 
#include "InstanceData.h"

/*
 * Initialize CanvasGradient.
 */

void
Gradient::Initialize(Napi::Env& env, Napi::Object& exports) {
  Napi::HandleScope scope(env);
  InstanceData* data = env.GetInstanceData<InstanceData>();

  Napi::Function ctor = DefineClass(env, "CanvasGradient", {
    InstanceMethod<&Gradient::AddColorStop>("addColorStop")
  });

  exports.Set("CanvasGradient", ctor);
  data->CanvasGradientCtor = Napi::Persistent(ctor);
}
/*
 * Initialize a new CanvasGradient.
 */
Gradient::Gradient(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Gradient>(info) {
  _env = info.Env();
  // Linear
  if (
    4 == info.Length() &&
    info[0].IsNumber() &&
    info[1].IsNumber() &&
    info[2].IsNumber() &&
    info[3].IsNumber()
  ) {
    _pattern = cairo_pattern_create_linear(
        info[0].As<Napi::Number>().DoubleValue()
      , info[1].As<Napi::Number>().DoubleValue()
      , info[2].As<Napi::Number>().DoubleValue()
      , info[3].As<Napi::Number>().DoubleValue());
    return;
  }
 // Radial
  if (
    6 == info.Length() &&
    info[0].IsNumber() &&
    info[1].IsNumber() &&
    info[2].IsNumber() &&
    info[3].IsNumber() &&
    info[4].IsNumber() &&
    info[5].IsNumber()
  ) {
    _pattern = cairo_pattern_create_radial(
        info[0].As<Napi::Number>().DoubleValue()
      , info[1].As<Napi::Number>().DoubleValue()
      , info[2].As<Napi::Number>().DoubleValue()
      , info[3].As<Napi::Number>().DoubleValue()
      , info[4].As<Napi::Number>().DoubleValue()
      , info[5].As<Napi::Number>().DoubleValue());
    return;
  }

  Napi::TypeError::New(Env(), "invalid arguments").ThrowAsJavaScriptException();
}

/*
 * Add color stop.
 */

void Gradient::AddColorStop(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "offset required").ThrowAsJavaScriptException();
    return;
  }

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "color string required").ThrowAsJavaScriptException();
    return;
  }

  short ok;
  std::string str = info[1].As<Napi::String>();
  uint32_t rgba = rgba_from_string(str.c_str(), &ok);

  if (ok) {
    rgba_t color = rgba_create(rgba);
    cairo_pattern_add_color_stop_rgba(
        _pattern
      , info[0].As<Napi::Number>().DoubleValue()
      , color.r
      , color.g
      , color.b
      , color.a);
  } else {
    Napi::TypeError::New(env, "parse color failed").ThrowAsJavaScriptException();
  }
}
/*
 * Destroy the pattern.
 */

Gradient::~Gradient() {
  cairo_pattern_destroy(_pattern);
}
