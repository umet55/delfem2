/*
 * Copyright (c) 2019 Nobuyuki Umetani
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/**
 * @brief CColor class
 * @details this class does not have the OpenGL dependency
 */

#ifndef DFM2_COLOR_H
#define DFM2_COLOR_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <assert.h>

namespace delfem2{

void GetRGB_HSV(float&r, float& g, float& b,
                float h, float s, float v);

// -------------------------------------------------------------

class CColor
{
public:
  CColor(){
    this->r = 0.8f;
    this->g = 0.8f;
    this->b = 0.8f;
    this->a = 1.0f;
  }
  CColor(float r, float g, float b){
    this->r = r;
    this->g = g;
    this->b = b;
    a = 1.0;
  }
  CColor(float r, float g, float b, float a){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }
  CColor(const std::vector<double>& v){
    this->r = 1.0;
    this->g = 1.0;
    this->b = 1.0;
    this->a = 1.0;
    if( v.size() == 4 ){
      this->r = v[0];
      this->g = v[1];
      this->b = v[2];
      this->a = v[3];
    }
    else if( v.size() == 3 ){
      this->r = v[0];
      this->g = v[1];
      this->b = v[2];
    }
    else if( v.size() > 0 ){
      this->r = v[0];
      this->g = v[0];
      this->b = v[0];
    }
  }
  void setRandomColor(){
    r = (float)rand()/(RAND_MAX+1.0f);
    g = (float)rand()/(RAND_MAX+1.0f);
    b = (float)rand()/(RAND_MAX+1.0f);
  }
  void setRandomVividColor(){
    double h = (double)rand()/(RAND_MAX+1.0);
    GetRGB_HSV(r,g,b, (float)h,1,1);
  }
  /*
  void glColor() const;
  void glMaterialDiffuse() const;
  void glColorDiffuse() const {
    this->glColor();
    this->glMaterialDiffuse();
  }
   */
  void getRGBChar(int& cr, int& cg, int& cb) const {
    if( r < 0 ){ cr = 0; }
    else if( r > 1 ){ cr = 255; }
    else{ cr = (int)(255*r); }
    ///
    if( g < 0 ){ cg = 0; }
    else if( g > 1 ){ cg = 255; }
    else{ cg = (int)(255*g); }
    ////
    if( b < 0 ){ cb = 0; }
    else if( b > 1 ){ cb = 255; }
    else{ cb = (int)(255*b); }
  }
  static CColor Black() { return CColor(0,0,0); }
  static CColor Red() { return CColor(1,0,0); }
  static CColor Blue() { return CColor(0,0,1); }
  static CColor Green(){ return CColor(0,1,0); }
  static CColor Yellow(){ return CColor(1,1,0); }
  static CColor Orange(){ return CColor(1, 0.71f, 0.30f); }
  static CColor Purple(){ return CColor(1,0,1); }
  static CColor Cyan(){ return CColor(0,1,1); }
  static CColor White() { return CColor(1,1,1); }
  static CColor Gray(float f) { return CColor(f, f, f); }
  static CColor Gray() { return CColor(0.8f, 0.8f, 0.8f); }
public:
  float r;
  float g;
  float b;
  float a;
};

void interpolateColor(CColor& Cout, float r, const CColor& C0, const CColor& C1);

void heatmap(double input,double* color);
void heatmap_glColor(double input);
void heatmap_glDiffuse(double input);

// ------------------------------------------------------------
// std::vector from here

CColor getColor(double input, const std::vector<std::pair<double, CColor> >& colorMap);


void makeHeatMap_BlueGrayRed(std::vector<std::pair<double, CColor> >& colorMap,
                             float min, float max);
void makeHeatMap_BlueCyanGreenYellowRed(std::vector<std::pair<double, CColor> >& colorMap,
                                        float min, float max, float alpha=1);
void makeHeatMap_RedYellowGreenCyanBlue(std::vector<std::pair<double, CColor> >& colorMap,
                                        float min, float max);

class CColorMap
{
public:
  CColorMap(){}
  CColorMap(double min, double max, const std::string& str){
    if( str == "bgr" ){
      makeHeatMap_BlueGrayRed(aColor, min, max);
    }
    else{
      makeHeatMap_BlueCyanGreenYellowRed(aColor, min, max);
    }
  }
public:
  std::vector< std::pair<double,CColor> > aColor;
};

// ---------------------------------------------------------------

void Write_Ply_Tri2DMesh_HeightColor(const std::string& fname,
                                     const std::vector<int>& aTri1,
                                     const std::vector<double>& aXY1,
                                     const std::vector<double>& aVal,
                                     std::vector< std::pair<double,CColor> >& colorMap);
  
} // namespace delfem2

#endif