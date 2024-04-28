// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#pragma once

#include <napi.h>
#include <cairo.h>

class Gradient: public Napi::ObjectWrap<Gradient> {
  public:
    static void Initialize(Napi::Env& env, Napi::Object& target);
    Gradient(const Napi::CallbackInfo& info);
    ~Gradient();
    void AddColorStop(const Napi::CallbackInfo& info);
    inline cairo_pattern_t *pattern(){ return _pattern; }
  private:
    cairo_pattern_t *_pattern;
};
