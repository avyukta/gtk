/* GTK - The GIMP Toolkit
 *   
 * Copyright © 2017 Benjamin Otte <otte@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gskslnativefunctionprivate.h"

#include "gskslenvironmentprivate.h"
#include "gskslfunctionprivate.h"
#include "gskslfunctiontypeprivate.h"
#include "gskslscopeprivate.h"
#include "gsksltypeprivate.h"

struct _GskSlNativeFunction
{
  const char *name;
  GskSlBuiltinType return_type;
  gsize n_arguments;
  const GskSlBuiltinType *argument_types;
};
 
#define NATIVE1(type, name, arg1) \
  { name, GSK_SL_BUILTIN_ ## type, 1, (GskSlBuiltinType[1]) { GSK_SL_BUILTIN_ ## arg1 } }
#define NATIVE2(type, name, arg1, arg2) \
  { name, GSK_SL_BUILTIN_ ## type, 2, (GskSlBuiltinType[2]) { GSK_SL_BUILTIN_ ## arg1, GSK_SL_BUILTIN_ ## arg2 } }
#define NATIVE3(type, name, arg1, arg2, arg3) \
  { name, GSK_SL_BUILTIN_ ## type, 3, (GskSlBuiltinType[3]) { GSK_SL_BUILTIN_ ## arg1, GSK_SL_BUILTIN_ ## arg2, GSK_SL_BUILTIN_ ## arg3, } }
static const GskSlNativeFunction gsk_glsl_functions[] = {
  NATIVE1 (FLOAT, "radians", FLOAT),
  NATIVE1 (VEC2, "radians", VEC2),
  NATIVE1 (VEC3, "radians", VEC3),
  NATIVE1 (VEC4, "radians", VEC4),
  NATIVE1 (FLOAT, "degrees", FLOAT),
  NATIVE1 (VEC2, "degrees", VEC2),
  NATIVE1 (VEC3, "degrees", VEC3),
  NATIVE1 (VEC4, "degrees", VEC4),
  NATIVE1 (FLOAT, "sin", FLOAT),
  NATIVE1 (VEC2, "sin", VEC2),
  NATIVE1 (VEC3, "sin", VEC3),
  NATIVE1 (VEC4, "sin", VEC4),
  NATIVE1 (FLOAT, "cos", FLOAT),
  NATIVE1 (VEC2, "cos", VEC2),
  NATIVE1 (VEC3, "cos", VEC3),
  NATIVE1 (VEC4, "cos", VEC4),
  NATIVE1 (FLOAT, "tan", FLOAT),
  NATIVE1 (VEC2, "tan", VEC2),
  NATIVE1 (VEC3, "tan", VEC3),
  NATIVE1 (VEC4, "tan", VEC4),
  NATIVE1 (FLOAT, "asin", FLOAT),
  NATIVE1 (VEC2, "asin", VEC2),
  NATIVE1 (VEC3, "asin", VEC3),
  NATIVE1 (VEC4, "asin", VEC4),
  NATIVE1 (FLOAT, "acos", FLOAT),
  NATIVE1 (VEC2, "acos", VEC2),
  NATIVE1 (VEC3, "acos", VEC3),
  NATIVE1 (VEC4, "acos", VEC4),
  NATIVE2 (FLOAT, "atan", FLOAT, FLOAT),
  NATIVE2 (VEC2, "atan", VEC2, VEC2),
  NATIVE2 (VEC3, "atan", VEC3, VEC3),
  NATIVE2 (VEC4, "atan", VEC4, VEC4),
  NATIVE1 (FLOAT, "atan", FLOAT),
  NATIVE1 (VEC2, "atan", VEC2),
  NATIVE1 (VEC3, "atan", VEC3),
  NATIVE1 (VEC4, "atan", VEC4),

  NATIVE2 (FLOAT, "pow", FLOAT, FLOAT),
  NATIVE2 (VEC2, "pow", VEC2, VEC2),
  NATIVE2 (VEC3, "pow", VEC3, VEC3),
  NATIVE2 (VEC4, "pow", VEC4, VEC4),
  NATIVE1 (FLOAT, "exp", FLOAT),
  NATIVE1 (VEC2, "exp", VEC2),
  NATIVE1 (VEC3, "exp", VEC3),
  NATIVE1 (VEC4, "exp", VEC4),
  NATIVE1 (FLOAT, "log", FLOAT),
  NATIVE1 (VEC2, "log", VEC2),
  NATIVE1 (VEC3, "log", VEC3),
  NATIVE1 (VEC4, "log", VEC4),
  NATIVE1 (FLOAT, "exp", FLOAT),
  NATIVE1 (VEC2, "exp", VEC2),
  NATIVE1 (VEC3, "exp", VEC3),
  NATIVE1 (VEC4, "exp", VEC4),
  NATIVE1 (FLOAT, "log2", FLOAT),
  NATIVE1 (VEC2, "log2", VEC2),
  NATIVE1 (VEC3, "log2", VEC3),
  NATIVE1 (VEC4, "log2", VEC4),
  NATIVE1 (FLOAT, "exp2", FLOAT),
  NATIVE1 (VEC2, "exp2", VEC2),
  NATIVE1 (VEC3, "exp2", VEC3),
  NATIVE1 (VEC4, "exp2", VEC4),
  NATIVE1 (FLOAT, "inversesqrt", FLOAT),
  NATIVE1 (VEC2, "inversesqrt", VEC2),
  NATIVE1 (VEC3, "inversesqrt", VEC3),
  NATIVE1 (VEC4, "inversesqrt", VEC4),

  NATIVE1 (FLOAT, "abs", FLOAT),
  NATIVE1 (VEC2, "abs", VEC2),
  NATIVE1 (VEC3, "abs", VEC3),
  NATIVE1 (VEC4, "abs", VEC4),
  NATIVE1 (FLOAT, "sign", FLOAT),
  NATIVE1 (VEC2, "sign", VEC2),
  NATIVE1 (VEC3, "sign", VEC3),
  NATIVE1 (VEC4, "sign", VEC4),
  NATIVE1 (FLOAT, "floor", FLOAT),
  NATIVE1 (VEC2, "floor", VEC2),
  NATIVE1 (VEC3, "floor", VEC3),
  NATIVE1 (VEC4, "floor", VEC4),
  NATIVE1 (FLOAT, "ceil", FLOAT),
  NATIVE1 (VEC2, "ceil", VEC2),
  NATIVE1 (VEC3, "ceil", VEC3),
  NATIVE1 (VEC4, "ceil", VEC4),
  NATIVE1 (FLOAT, "fract", FLOAT),
  NATIVE1 (VEC2, "fract", VEC2),
  NATIVE1 (VEC3, "fract", VEC3),
  NATIVE1 (VEC4, "fract", VEC4),
  NATIVE2 (FLOAT, "mod", FLOAT, FLOAT),
  NATIVE2 (VEC2, "mod", VEC2, FLOAT),
  NATIVE2 (VEC3, "mod", VEC3, FLOAT),
  NATIVE2 (VEC4, "mod", VEC4, FLOAT),
  NATIVE2 (VEC2, "mod", VEC2, VEC2),
  NATIVE2 (VEC3, "mod", VEC3, VEC3),
  NATIVE2 (VEC4, "mod", VEC4, VEC4),
  NATIVE2 (FLOAT, "min", FLOAT, FLOAT),
  NATIVE2 (VEC2, "min", VEC2, FLOAT),
  NATIVE2 (VEC3, "min", VEC3, FLOAT),
  NATIVE2 (VEC4, "min", VEC4, FLOAT),
  NATIVE2 (VEC2, "min", VEC2, VEC2),
  NATIVE2 (VEC3, "min", VEC3, VEC3),
  NATIVE2 (VEC4, "min", VEC4, VEC4),
  NATIVE2 (FLOAT, "max", FLOAT, FLOAT),
  NATIVE2 (VEC2, "max", VEC2, FLOAT),
  NATIVE2 (VEC3, "max", VEC3, FLOAT),
  NATIVE2 (VEC4, "max", VEC4, FLOAT),
  NATIVE2 (VEC2, "max", VEC2, VEC2),
  NATIVE2 (VEC3, "max", VEC3, VEC3),
  NATIVE2 (VEC4, "max", VEC4, VEC4),
  NATIVE3 (FLOAT, "clamp", FLOAT, FLOAT, FLOAT),
  NATIVE3 (VEC2, "clamp", VEC2, FLOAT, FLOAT),
  NATIVE3 (VEC3, "clamp", VEC3, FLOAT, FLOAT),
  NATIVE3 (VEC4, "clamp", VEC4, FLOAT, FLOAT),
  NATIVE3 (VEC2, "clamp", VEC2, VEC2, VEC2),
  NATIVE3 (VEC3, "clamp", VEC3, VEC3, VEC3),
  NATIVE3 (VEC4, "clamp", VEC4, VEC4, VEC4),
  NATIVE3 (FLOAT, "mix", FLOAT, FLOAT, FLOAT),
  NATIVE3 (VEC2, "mix", VEC2, VEC2, FLOAT),
  NATIVE3 (VEC3, "mix", VEC3, VEC3, FLOAT),
  NATIVE3 (VEC4, "mix", VEC4, VEC4, FLOAT),
  NATIVE3 (VEC2, "mix", VEC2, VEC2, VEC2),
  NATIVE3 (VEC3, "mix", VEC3, VEC3, VEC3),
  NATIVE3 (VEC4, "mix", VEC4, VEC4, VEC4),
  NATIVE2 (FLOAT, "step", FLOAT, FLOAT),
  NATIVE2 (VEC2, "step", FLOAT, VEC2),
  NATIVE2 (VEC3, "step", FLOAT, VEC3),
  NATIVE2 (VEC4, "step", FLOAT, VEC4),
  NATIVE2 (VEC2, "step", VEC2, VEC2),
  NATIVE2 (VEC3, "step", VEC3, VEC3),
  NATIVE2 (VEC4, "step", VEC4, VEC4),
  NATIVE3 (FLOAT, "smoothstep", FLOAT, FLOAT, FLOAT),
  NATIVE3 (VEC2, "smoothstep", VEC2, VEC2, FLOAT),
  NATIVE3 (VEC3, "smoothstep", VEC3, VEC3, FLOAT),
  NATIVE3 (VEC4, "smoothstep", VEC4, VEC4, FLOAT),
  NATIVE3 (VEC2, "smoothstep", VEC2, VEC2, VEC2),
  NATIVE3 (VEC3, "smoothstep", VEC3, VEC3, VEC3),
  NATIVE3 (VEC4, "smoothstep", VEC4, VEC4, VEC4),

  NATIVE1 (FLOAT, "length", FLOAT),
  NATIVE1 (FLOAT, "length", VEC2),
  NATIVE1 (FLOAT, "length", VEC3),
  NATIVE1 (FLOAT, "length", VEC4),
  NATIVE2 (FLOAT, "distance", FLOAT, FLOAT),
  NATIVE2 (FLOAT, "distance", VEC2, VEC2),
  NATIVE2 (FLOAT, "distance", VEC3, VEC3),
  NATIVE2 (FLOAT, "distance", VEC4, VEC4),
  NATIVE2 (FLOAT, "dot", FLOAT, FLOAT),
  NATIVE2 (FLOAT, "dot", VEC2, VEC2),
  NATIVE2 (FLOAT, "dot", VEC3, VEC3),
  NATIVE2 (FLOAT, "dot", VEC4, VEC4),
  NATIVE2 (VEC3, "cross", VEC3, VEC3),
  NATIVE1 (FLOAT, "normalize", FLOAT),
  NATIVE1 (VEC2, "normalize", VEC2),
  NATIVE1 (VEC3, "normalize", VEC3),
  NATIVE1 (VEC4, "normalize", VEC4),
  NATIVE3 (FLOAT, "faceforward", FLOAT, FLOAT, FLOAT),
  NATIVE3 (FLOAT, "faceforward", VEC2, VEC2, VEC2),
  NATIVE3 (FLOAT, "faceforward", VEC3, VEC3, VEC3),
  NATIVE3 (FLOAT, "faceforward", VEC4, VEC4, VEC4),
  NATIVE2 (FLOAT, "reflect", FLOAT, FLOAT),
  NATIVE2 (VEC2, "reflect", VEC2, VEC2),
  NATIVE2 (VEC3, "reflect", VEC3, VEC3),
  NATIVE2 (VEC4, "reflect", VEC4, VEC4),
  NATIVE3 (FLOAT, "refract", FLOAT, FLOAT, FLOAT),
  NATIVE3 (VEC2, "refract", VEC2, VEC2, FLOAT),
  NATIVE3 (VEC3, "refract", VEC3, VEC3, FLOAT),
  NATIVE3 (VEC4, "refract", VEC4, VEC4, FLOAT),

  NATIVE2 (MAT2, "matrixCompMult", MAT2, MAT2),
  NATIVE2 (MAT3, "matrixCompMult", MAT3, MAT3),
  NATIVE2 (MAT4, "matrixCompMult", MAT4, MAT4),

  NATIVE2 (BVEC2, "lessThan", VEC2, VEC2),
  NATIVE2 (BVEC3, "lessThan", VEC3, VEC3),
  NATIVE2 (BVEC4, "lessThan", VEC4, VEC4),
  NATIVE2 (BVEC2, "lessThan", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "lessThan", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "lessThan", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "lessThanEqual", VEC2, VEC2),
  NATIVE2 (BVEC3, "lessThanEqual", VEC3, VEC3),
  NATIVE2 (BVEC4, "lessThanEqual", VEC4, VEC4),
  NATIVE2 (BVEC2, "lessThanEqual", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "lessThanEqual", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "lessThanEqual", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "greaterThan", VEC2, VEC2),
  NATIVE2 (BVEC3, "greaterThan", VEC3, VEC3),
  NATIVE2 (BVEC4, "greaterThan", VEC4, VEC4),
  NATIVE2 (BVEC2, "greaterThan", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "greaterThan", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "greaterThan", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "greaterThanEqual", VEC2, VEC2),
  NATIVE2 (BVEC3, "greaterThanEqual", VEC3, VEC3),
  NATIVE2 (BVEC4, "greaterThanEqual", VEC4, VEC4),
  NATIVE2 (BVEC2, "greaterThanEqual", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "greaterThanEqual", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "greaterThanEqual", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "equal", VEC2, VEC2),
  NATIVE2 (BVEC3, "equal", VEC3, VEC3),
  NATIVE2 (BVEC4, "equal", VEC4, VEC4),
  NATIVE2 (BVEC2, "equal", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "equal", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "equal", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "equal", BVEC2, BVEC2),
  NATIVE2 (BVEC3, "equal", BVEC3, BVEC3),
  NATIVE2 (BVEC4, "equal", BVEC4, BVEC4),
  NATIVE2 (BVEC2, "notEqual", VEC2, VEC2),
  NATIVE2 (BVEC3, "notEqual", VEC3, VEC3),
  NATIVE2 (BVEC4, "notEqual", VEC4, VEC4),
  NATIVE2 (BVEC2, "notEqual", IVEC2, IVEC2),
  NATIVE2 (BVEC3, "notEqual", IVEC3, IVEC3),
  NATIVE2 (BVEC4, "notEqual", IVEC4, IVEC4),
  NATIVE2 (BVEC2, "notEqual", BVEC2, BVEC2),
  NATIVE2 (BVEC3, "notEqual", BVEC3, BVEC3),
  NATIVE2 (BVEC4, "notEqual", BVEC4, BVEC4),
  NATIVE1 (BVEC2, "any", BVEC2),
  NATIVE1 (BVEC3, "any", BVEC3),
  NATIVE1 (BVEC4, "any", BVEC4),
  NATIVE1 (BVEC2, "all", BVEC2),
  NATIVE1 (BVEC3, "all", BVEC3),
  NATIVE1 (BVEC4, "all", BVEC4),
  NATIVE1 (BVEC2, "not", BVEC2),
  NATIVE1 (BVEC3, "not", BVEC3),
  NATIVE1 (BVEC4, "not", BVEC4),
  { NULL }
};

static const GskSlNativeFunction gsk_glsl_functions_120[] = {
  NATIVE2 (MAT2, "outerProduct", VEC2, VEC2),
  NATIVE2 (MAT3, "outerProduct", VEC3, VEC3),
  NATIVE2 (MAT4, "outerProduct", VEC4, VEC4),
  NATIVE2 (MAT2X3, "outerProduct", VEC3, VEC2),
  NATIVE2 (MAT2X4, "outerProduct", VEC4, VEC2),
  NATIVE2 (MAT3X2, "outerProduct", VEC2, VEC3),
  NATIVE2 (MAT3X4, "outerProduct", VEC4, VEC3),
  NATIVE2 (MAT4X2, "outerProduct", VEC2, VEC4),
  NATIVE2 (MAT4X3, "outerProduct", VEC3, VEC4),
  NATIVE1 (MAT2, "transpose", MAT2),
  NATIVE1 (MAT3, "transpose", MAT3),
  NATIVE1 (MAT4, "transpose", MAT4),
  NATIVE1 (MAT2X3, "transpose", MAT3X2),
  NATIVE1 (MAT2X4, "transpose", MAT4X2),
  NATIVE1 (MAT3X2, "transpose", MAT2X3),
  NATIVE1 (MAT3X4, "transpose", MAT4X3),
  NATIVE1 (MAT4X2, "transpose", MAT2X4),
  NATIVE1 (MAT4X3, "transpose", MAT3X4),
  NATIVE2 (MAT2X3, "matrixCompMult", MAT2X3, MAT2X3),
  NATIVE2 (MAT2X4, "matrixCompMult", MAT2X4, MAT2X4),
  NATIVE2 (MAT3X2, "matrixCompMult", MAT3X2, MAT3X2),
  NATIVE2 (MAT3X4, "matrixCompMult", MAT3X4, MAT3X4),
  NATIVE2 (MAT4X2, "matrixCompMult", MAT4X2, MAT4X2),
  NATIVE2 (MAT4X3, "matrixCompMult", MAT4X3, MAT4X3),
  { NULL }
};

static const GskSlNativeFunction gsk_glsl_functions_130[] = {
  NATIVE1 (FLOAT, "sinh", FLOAT),
  NATIVE1 (VEC2, "sinh", VEC2),
  NATIVE1 (VEC3, "sinh", VEC3),
  NATIVE1 (VEC4, "sinh", VEC4),
  NATIVE1 (FLOAT, "cosh", FLOAT),
  NATIVE1 (VEC2, "cosh", VEC2),
  NATIVE1 (VEC3, "cosh", VEC3),
  NATIVE1 (VEC4, "cosh", VEC4),
  NATIVE1 (FLOAT, "tanh", FLOAT),
  NATIVE1 (VEC2, "tanh", VEC2),
  NATIVE1 (VEC3, "tanh", VEC3),
  NATIVE1 (VEC4, "tanh", VEC4),
  NATIVE1 (FLOAT, "asinh", FLOAT),
  NATIVE1 (VEC2, "asinh", VEC2),
  NATIVE1 (VEC3, "asinh", VEC3),
  NATIVE1 (VEC4, "asinh", VEC4),
  NATIVE1 (FLOAT, "acosh", FLOAT),
  NATIVE1 (VEC2, "acosh", VEC2),
  NATIVE1 (VEC3, "acosh", VEC3),
  NATIVE1 (VEC4, "acosh", VEC4),
  NATIVE1 (FLOAT, "atanh", FLOAT),
  NATIVE1 (VEC2, "atanh", VEC2),
  NATIVE1 (VEC3, "atanh", VEC3),
  NATIVE1 (VEC4, "atanh", VEC4),

  NATIVE1 (INT, "abs", INT),
  NATIVE1 (IVEC2, "abs", IVEC2),
  NATIVE1 (IVEC3, "abs", IVEC3),
  NATIVE1 (IVEC4, "abs", IVEC4),
  NATIVE1 (INT, "sign", INT),
  NATIVE1 (IVEC2, "sign", IVEC2),
  NATIVE1 (IVEC3, "sign", IVEC3),
  NATIVE1 (IVEC4, "sign", IVEC4),
  NATIVE1 (FLOAT, "trunc", FLOAT),
  NATIVE1 (VEC2, "trunc", VEC2),
  NATIVE1 (VEC3, "trunc", VEC3),
  NATIVE1 (VEC4, "trunc", VEC4),
  NATIVE1 (FLOAT, "round", FLOAT),
  NATIVE1 (VEC2, "round", VEC2),
  NATIVE1 (VEC3, "round", VEC3),
  NATIVE1 (VEC4, "round", VEC4),
  NATIVE1 (FLOAT, "roundEven", FLOAT),
  NATIVE1 (VEC2, "roundEven", VEC2),
  NATIVE1 (VEC3, "roundEven", VEC3),
  NATIVE1 (VEC4, "roundEven", VEC4),
  NATIVE2 (FLOAT, "modf", FLOAT, FLOAT), //OUT!
  NATIVE2 (VEC2, "modf", VEC2, VEC2), //OUT!
  NATIVE2 (VEC3, "modf", VEC3, VEC3), //OUT!
  NATIVE2 (VEC4, "modf", VEC4, VEC4), //OUT!
  NATIVE1 (INT, "min", INT),
  NATIVE1 (IVEC2, "min", INT),
  NATIVE1 (IVEC3, "min", INT),
  NATIVE1 (IVEC4, "min", INT),
  NATIVE1 (IVEC2, "min", IVEC2),
  NATIVE1 (IVEC3, "min", IVEC3),
  NATIVE1 (IVEC4, "min", IVEC4),
  NATIVE1 (UINT, "min", UINT),
  NATIVE1 (UVEC2, "min", UINT),
  NATIVE1 (UVEC3, "min", UINT),
  NATIVE1 (UVEC4, "min", UINT),
  NATIVE1 (UVEC2, "min", UVEC2),
  NATIVE1 (UVEC3, "min", UVEC3),
  NATIVE1 (UVEC4, "min", UVEC4),
  NATIVE1 (INT, "max", INT),
  NATIVE1 (IVEC2, "max", INT),
  NATIVE1 (IVEC3, "max", INT),
  NATIVE1 (IVEC4, "max", INT),
  NATIVE1 (IVEC2, "max", IVEC2),
  NATIVE1 (IVEC3, "max", IVEC3),
  NATIVE1 (IVEC4, "max", IVEC4),
  NATIVE1 (UINT, "max", UINT),
  NATIVE1 (UVEC2, "max", UINT),
  NATIVE1 (UVEC3, "max", UINT),
  NATIVE1 (UVEC4, "max", UINT),
  NATIVE1 (UVEC2, "max", UVEC2),
  NATIVE1 (UVEC3, "max", UVEC3),
  NATIVE1 (UVEC4, "max", UVEC4),
  NATIVE2 (INT, "clamp", INT, INT),
  NATIVE2 (IVEC2, "clamp", INT, INT),
  NATIVE2 (IVEC3, "clamp", INT, INT),
  NATIVE2 (IVEC4, "clamp", INT, INT),
  NATIVE2 (IVEC2, "clamp", IVEC2, IVEC2),
  NATIVE2 (IVEC3, "clamp", IVEC3, IVEC3),
  NATIVE2 (IVEC4, "clamp", IVEC4, IVEC4),
  NATIVE2 (UINT, "clamp", UINT, UINT),
  NATIVE2 (UVEC2, "clamp", UINT, UINT),
  NATIVE2 (UVEC3, "clamp", UINT, UINT),
  NATIVE2 (UVEC4, "clamp", UINT, UINT),
  NATIVE2 (UVEC2, "clamp", UVEC2, UVEC2),
  NATIVE2 (UVEC3, "clamp", UVEC3, UVEC3),
  NATIVE2 (UVEC4, "clamp", UVEC4, UVEC4),
  NATIVE2 (FLOAT, "mix", FLOAT, FLOAT),
  NATIVE2 (VEC2, "mix", VEC2, VEC2),
  NATIVE2 (VEC3, "mix", VEC3, VEC3),
  NATIVE2 (VEC4, "mix", VEC4, VEC4),
  NATIVE1 (BOOL, "isnan", FLOAT),
  NATIVE1 (BVEC2, "isnan", VEC2),
  NATIVE1 (BVEC3, "isnan", VEC3),
  NATIVE1 (BVEC4, "isnan", VEC4),
  NATIVE1 (BOOL, "isinf", FLOAT),
  NATIVE1 (BVEC2, "isinf", VEC2),
  NATIVE1 (BVEC3, "isinf", VEC3),
  NATIVE1 (BVEC4, "isinf", VEC4),

  NATIVE2 (BVEC2, "lessThan", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "lessThan", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "lessThan", UVEC4, UVEC4),
  NATIVE2 (BVEC2, "lessThanEqual", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "lessThanEqual", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "lessThanEqual", UVEC4, UVEC4),
  NATIVE2 (BVEC2, "greaterThan", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "greaterThan", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "greaterThan", UVEC4, UVEC4),
  NATIVE2 (BVEC2, "greaterThanEqual", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "greaterThanEqual", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "greaterThanEqual", UVEC4, UVEC4),
  NATIVE2 (BVEC2, "equal", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "equal", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "equal", UVEC4, UVEC4),
  NATIVE2 (BVEC2, "notEqual", UVEC2, UVEC2),
  NATIVE2 (BVEC3, "notEqual", UVEC3, UVEC3),
  NATIVE2 (BVEC4, "notEqual", UVEC4, UVEC4),
  { NULL }
};

static const GskSlNativeFunction gsk_glsl_functions_150[] = {
  NATIVE1 (FLOAT, "determinant", MAT2),
  NATIVE1 (FLOAT, "determinant", MAT3),
  NATIVE1 (FLOAT, "determinant", MAT4),
  NATIVE1 (MAT2, "inverse", MAT2),
  NATIVE1 (MAT3, "inverse", MAT3),
  NATIVE1 (MAT4, "inverse", MAT4),
  { NULL }
};

static GskSlFunctionType *
gsk_sl_native_functions_construct_type (const GskSlNativeFunction *native)
{
  GskSlFunctionType *type;
  gsize i;

  type = gsk_sl_function_type_new (gsk_sl_type_get_builtin (native->return_type));
  for (i = 0; i < native->n_arguments; i++)
    {
      type = gsk_sl_function_type_add_argument (type,
                                                GSK_SL_STORAGE_PARAMETER_IN,
                                                gsk_sl_type_get_builtin (native->argument_types[i]));
    }

  return type;
}

static guint32
gsk_sl_native_functions_write_spv_unimplemented (GskSpvWriter *writer,
                                                 guint32      *arguments,
                                                 gpointer      user_data)
{
  g_assert_not_reached ();

  return 0;
}

static void
gsk_sl_native_functions_add_list (GskSlScope                *scope,
                                  const GskSlNativeFunction *functions)
{
  guint i;

  for (i = 0; functions[i].name; i++)
    {
      GskSlFunctionType *type;
      GskSlFunction *function;

      type = gsk_sl_native_functions_construct_type (&functions[i]);

      function = gsk_sl_function_new_native (functions[i].name,
                                             type,
                                             NULL,
                                             gsk_sl_native_functions_write_spv_unimplemented,
                                             (gpointer) &functions[i],
                                             NULL);
      gsk_sl_scope_add_function (scope, function);

      gsk_sl_function_unref (function);
      gsk_sl_function_type_unref (type);
    }
}

void
gsk_sl_native_functions_add (GskSlScope       *scope,
                             GskSlEnvironment *environment)
{
  guint version = gsk_sl_environment_get_version (environment);

  gsk_sl_native_functions_add_list (scope, gsk_glsl_functions);

  if (version < 120)
    return;
  gsk_sl_native_functions_add_list (scope, gsk_glsl_functions_120);

  if (version < 130)
    return;
  gsk_sl_native_functions_add_list (scope, gsk_glsl_functions_130);

  if (version < 150)
    return;
  gsk_sl_native_functions_add_list (scope, gsk_glsl_functions_150);
}
