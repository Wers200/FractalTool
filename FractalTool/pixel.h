#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer global
// {
//
//   float4 zoom;                       // Offset:    0 Size:    16 [unused]
//   float4 prevZoom;                   // Offset:   16 Size:    16
//   float2 c;                          // Offset:   32 Size:     8 [unused]
//   uint2 size;                        // Offset:   40 Size:     8
//   uint maxIter;                      // Offset:   48 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// data                              texture    uint          2d             t0      1 
// global                            cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xy  
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[4], immediateIndexed
dcl_resource_texture2d (uint,uint,uint,uint) t0
dcl_input_ps_siv linear noperspective v0.xy, position
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 4
ftoi r0.xy, v0.xyxx
mov r0.zw, l(0,0,0,0)
ld_indexable(texture2d)(uint,uint,uint,uint) r0.x, r0.xyzw, t0.xyzw
utof r0.y, r0.x
mul r0.yz, r0.yyyy, l(0.000000, 0.100000, 0.016667, 0.000000)
round_ni r0.z, r0.z
mad r0.y, -r0.z, l(6.000000), r0.y
add r0.z, r0.y, r0.y
ge r0.z, r0.z, -r0.z
movc r0.zw, r0.zzzz, l(0,0,2.000000,0.500000), l(0,0,-2.000000,-0.500000)
mul r0.w, r0.w, r0.y
round_ni r0.y, r0.y
frc r0.w, r0.w
mad r0.z, r0.z, r0.w, l(-1.000000)
add r0.z, -|r0.z|, l(1.000000)
eq r1.xyzw, r0.yyyy, l(2.000000, 3.000000, 3.000000, 2.000000)
eq r2.xyzw, r0.yyyy, l(1.000000, 0.000000, 4.000000, 5.000000)
or r1.xyzw, r1.xyzw, r2.xyzw
or r0.yw, r2.wwwz, r2.yyyx
and r1.yw, r0.zzzz, r1.yyyw
and r0.z, r0.z, r0.w
movc r0.y, r0.y, l(1.000000), r0.z
movc r0.zw, r1.xxxz, l(0,0,1.000000,1.000000), r1.yyyw
iadd r1.x, cb0[3].x, l(-1)
ult r0.x, r0.x, r1.x
and r1.xyz, r0.yzwy, r0.xxxx
min r0.xy, cb0[1].xyxx, cb0[1].zwzz
add r0.zw, -r0.xxxy, v1.xxxy
ge r0.xy, v1.xyxx, r0.xyxx
utof r2.xy, cb0[2].zwzz
div r2.xy, l(2.000000, 2.000000, 0.000000, 0.000000), r2.xyxx
ge r0.zw, r2.xxxy, |r0.zzzw|
max r2.zw, cb0[1].xxxy, cb0[1].zzzw
add r3.xy, -r2.zwzz, v1.xyxx
ge r2.zw, r2.zzzw, v1.xxxy
ge r2.xy, r2.xyxx, |r3.xyxx|
or r0.z, r0.z, r2.x
or r0.z, r0.w, r0.z
or r0.z, r2.y, r0.z
movc r0.z, r0.z, l(0.800000), l(0.200000)
add r3.xyz, r0.zzzz, r1.xyzx
and r0.x, r0.x, r2.z
and r0.x, r0.y, r0.x
and r0.x, r2.w, r0.x
movc o0.xyz, r0.xxxx, r3.xyzx, r1.xyzx
mov o0.w, l(1.000000)
ret 
// Approximately 47 instruction slots used
#endif

const BYTE ps_bytes[] =
{
     68,  88,  66,  67, 126, 241, 
    231, 240,   5,  51, 182,  48, 
    117, 221, 235, 108, 110,  94, 
    252, 120,   1,   0,   0,   0, 
    196,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    152,   2,   0,   0, 240,   2, 
      0,   0,  36,   3,   0,   0, 
     40,   9,   0,   0,  82,  68, 
     69,  70,  92,   2,   0,   0, 
      1,   0,   0,   0, 136,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
     52,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   2,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 129,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 100,  97, 
    116,  97,   0, 103, 108, 111, 
     98,  97, 108,   0, 129,   0, 
      0,   0,   5,   0,   0,   0, 
    160,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 104,   1, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0, 116,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 152,   1,   0,   0, 
     16,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    116,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    161,   1,   0,   0,  32,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0, 172,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 208,   1, 
      0,   0,  40,   0,   0,   0, 
      8,   0,   0,   0,   2,   0, 
      0,   0, 220,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   0,   2,   0,   0, 
     48,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    122, 111, 111, 109,   0, 102, 
    108, 111,  97, 116,  52,   0, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 109,   1,   0,   0, 
    112, 114, 101, 118,  90, 111, 
    111, 109,   0,  99,   0, 102, 
    108, 111,  97, 116,  50,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 163,   1, 
      0,   0, 115, 105, 122, 101, 
      0, 117, 105, 110, 116,  50, 
      0, 171,   1,   0,  19,   0, 
      1,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 213,   1, 
      0,   0, 109,  97, 120,  73, 
    116, 101, 114,   0, 100, 119, 
    111, 114, 100,   0, 171, 171, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   8,   2,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78,  80,   0,   0,   0, 
      2,   0,   0,   0,   8,   0, 
      0,   0,  56,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   3, 
      0,   0,  68,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0, 171, 
    171, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  65,  82, 
     71,  69,  84,   0, 171, 171, 
     83,  72,  69,  88, 252,   5, 
      0,   0,  80,   0,   0,   0, 
    127,   1,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     68,  68,   0,   0, 100,  32, 
      0,   4,  50,  16,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  98,  16,   0,   3, 
     50,  16,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 104,   0,   0,   2, 
      4,   0,   0,   0,  27,   0, 
      0,   5,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,  16, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 194,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  45,   0,   0, 137, 
    194,   0,   0, 128,   3,  17, 
     17,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  86,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,  10,  98,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 205, 204, 204,  61, 
    137, 136, 136,  60,   0,   0, 
      0,   0,  65,   0,   0,   5, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10,  34,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 192,  64, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     29,   0,   0,   8,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  55,   0,   0,  15, 
    194,   0,  16,   0,   0,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,  64,   0,   0,   0,  63, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0, 192,   0,   0, 
      0, 191,  56,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     65,   0,   0,   5,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128, 191,   0,   0, 
      0,   8,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16, 128, 193,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     24,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  64,   0,   0, 
     64,  64,   0,   0,  64,  64, 
      0,   0,   0,  64,  24,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0, 128,  64,   0,   0, 
    160,  64,  60,   0,   0,   7, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     60,   0,   0,   7, 162,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  11,  16,   0,   2,   0, 
      0,   0,  86,   1,  16,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   7, 162,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     86,  13,  16,   0,   1,   0, 
      0,   0,   1,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,  12, 194,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   8,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,  86,  13, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10, 128,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,  64,   0,   0, 255, 255, 
    255, 255,  79,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      1,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    150,   7,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  51,   0, 
      0,   9,  50,   0,  16,   0, 
      0,   0,   0,   0,  70, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 230, 138, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8, 194,   0,  16,   0, 
      0,   0,   0,   0,   6,   4, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   6,  20, 
     16,   0,   1,   0,   0,   0, 
     29,   0,   0,   7,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  86,   0, 
      0,   6,  50,   0,  16,   0, 
      2,   0,   0,   0, 230, 138, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  14,   0, 
      0,  10,  50,   0,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  64, 
      0,   0,   0,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   2,   0, 
      0,   0,  29,   0,   0,   8, 
    194,   0,  16,   0,   0,   0, 
      0,   0,   6,   4,  16,   0, 
      2,   0,   0,   0, 166,  14, 
     16, 128, 129,   0,   0,   0, 
      0,   0,   0,   0,  52,   0, 
      0,   9, 194,   0,  16,   0, 
      2,   0,   0,   0,   6, 132, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 166, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8,  50,   0,  16,   0, 
      3,   0,   0,   0, 230,  10, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  70,  16, 
     16,   0,   1,   0,   0,   0, 
     29,   0,   0,   7, 194,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  14,  16,   0,   2,   0, 
      0,   0,   6,  20,  16,   0, 
      1,   0,   0,   0,  29,   0, 
      0,   8,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   0,  16, 128, 129,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  60,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  60,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    205, 204,  76,  63,   1,  64, 
      0,   0, 205, 204,  76,  62, 
      0,   0,   0,   7, 114,   0, 
     16,   0,   3,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,   1,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  55,   0, 
      0,   9, 114,  32,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 130,  32,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     47,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  22,   0, 
      0,   0,   1,   0,   0,   0, 
     12,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};