// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkF3DComputeDepthCS.h"

const char *vtkF3DComputeDepthCS =
"#version 430\n"
"layout(local_size_x = 32) in;\n"
"layout(std430) buffer;\n"
"\n"
"struct vertex\n"
"{\n"
"  float x;\n"
"  float y;\n"
"  float z;\n"
"};\n"
"\n"
"layout(binding = 0) readonly buffer Points\n"
"{\n"
"  vertex point[];\n"
"};\n"
"\n"
"layout(binding = 1) readonly buffer Indices\n"
"{\n"
"  uint index[];\n"
"};\n"
"\n"
"layout(binding = 2) writeonly buffer Depths\n"
"{\n"
"  float depth[];\n"
"};\n"
"\n"
"layout (location = 0) uniform vec3 viewDirection;\n"
"layout (location = 1) uniform int count;\n"
"\n"
"void main()\n"
"{\n"
"  uint i = gl_GlobalInvocationID.x;\n"
"  if (i < count)\n"
"  {\n"
"    vertex v = point[index[i]];\n"
"    depth[i] = dot(viewDirection, vec3(v.x, v.y, v.z));\n"
"  }\n"
"}\n"
"";
