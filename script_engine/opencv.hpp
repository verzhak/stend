
#ifndef __OPENCV_HPP
#define __OPENCV_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "matrix/matrix.hpp"

void opencv_register(lua_State * state);

int lua_threshold(lua_State * state);
int lua_split(lua_State * state);
int lua_merge(lua_State * state);
int lua_bitwise_and(lua_State * state);

int lua_bitwise_not(lua_State * state);
int lua_bitwise_or(lua_State * state);
int lua_bitwise_xor(lua_State * state);
int lua_adaptiveThreshold(lua_State * state);
int lua_abs(lua_State * state);
int lua_dct(lua_State * state);
int lua_idct(lua_State * state);
int lua_dft(lua_State * state);
int lua_idft(lua_State * state);
int lua_approxPolyDP(lua_State * state);
int lua_arcLength(lua_State * state);
int lua_bilateralFilter(lua_State * state);

int lua_matrix_create(lua_State * state);
int lua_matrix_delete(lua_State * state);
int lua_matrix_get_value(lua_State * state);
int lua_matrix_set_value(lua_State * state);

int lua_erode(lua_State * state);
int lua_dilate(lua_State * state);
int lua_morphologyEx(lua_State * state);

int lua_blur(lua_State * state);
int lua_cvtColor(lua_State * state);
int lua_GaussianBlur(lua_State * state);
int lua_medianBlur (lua_State * state);
int lua_Canny (lua_State * state);
int lua_Sobel (lua_State * state);
int lua_Scharr (lua_State * state);

int lua_filter2D (lua_State * state);
int lua_integral (lua_State * state);
int lua_Laplacian (lua_State * state);
int lua_mulSpectrums (lua_State * state);
int lua_sepFilter2D (lua_State * state);
int lua_setTo (lua_State * state);
int lua_clone (lua_State * state);
int lua_Size (lua_State * state);
int lua_imread (lua_State * state);
int lua_imwrite (lua_State * state);
int lua_normalize (lua_State * state);

#endif

