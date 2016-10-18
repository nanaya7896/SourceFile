xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}

template AnimTicksPerSecond {
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}

template Animation {
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}

template AnimationSet {
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template FloatKeys {
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}


Material Material__96 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Model\\Graph\\kabe01.jpg";
 }
}

Frame ______001 {
 

 FrameTransformMatrix {
  4.070799,0.000000,0.000000,0.000000,0.000000,4.070799,0.000000,0.000000,0.000000,0.000000,4.070799,0.000000,-0.071342,22.916149,-0.062733,1.000000;;
 }

 Mesh ______001 {
  28;
  49.999994;-3.738038;49.999994;,
  10.937497;3.738038;49.999994;,
  49.999994;3.738038;49.999994;,
  10.937498;-3.738038;49.999994;,
  -49.999994;3.738038;49.999994;,
  -10.937500;-3.738038;49.999994;,
  -49.999994;-3.738038;49.999994;,
  -10.937498;3.738038;49.999994;,
  -49.999994;-3.738038;-49.999994;,
  -10.937498;3.738037;-49.999994;,
  -49.999994;3.738037;-49.999994;,
  -10.937500;-3.738038;-49.999994;,
  49.999994;3.738037;-49.999994;,
  10.937498;-3.738038;-49.999994;,
  49.999994;-3.738038;-49.999994;,
  10.937497;3.738037;-49.999994;,
  -49.999994;3.738037;-49.999994;,
  -49.999994;-3.738038;49.999994;,
  -49.999994;-3.738038;-49.999994;,
  -49.999994;3.738038;49.999994;,
  49.999994;-3.738038;-49.999994;,
  49.999994;3.738038;-10.937498;,
  49.999994;3.738037;-49.999994;,
  49.999994;-3.738038;-10.937499;,
  49.999994;3.738038;49.999994;,
  49.999994;-3.738038;10.937499;,
  49.999994;-3.738038;49.999994;,
  49.999994;3.738037;10.937498;;
  14;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,8,11;,
  3;12,13,14;,
  3;13,12,15;,
  3;16,17,18;,
  3;17,16,19;,
  3;20,21,22;,
  3;21,20,23;,
  3;24,25,26;,
  3;25,24,27;;

  MeshMaterialList {
   1;
   14;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { Material__96 }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   140;
   0,
   0,
   3212836864,
   1049669110,
   1065223871,
   0,
   0,
   3212836864,
   1031754564,
   1064223759,
   0,
   0,
   3212836864,
   1049669110,
   1064223759,
   0,
   0,
   3212836864,
   1031754564,
   1065223871,
   0,
   0,
   3212836864,
   1060254519,
   1063178432,
   0,
   0,
   3212836864,
   1065137492,
   1064190510,
   0,
   0,
   3212836864,
   1060254519,
   1064190510,
   0,
   0,
   3212836864,
   1065137492,
   1063178432,
   0,
   0,
   1065353216,
   1048912119,
   1064190510,
   0,
   0,
   1065353216,
   1025497600,
   1063178432,
   0,
   0,
   1065353216,
   1048912119,
   1063178432,
   0,
   0,
   1065353216,
   1025497600,
   1064190510,
   0,
   0,
   1065353216,
   1060855949,
   1064227081,
   0,
   0,
   1065353216,
   1064550997,
   1065227194,
   0,
   0,
   1065353216,
   1060855949,
   1065227194,
   0,
   0,
   1065353216,
   1064550997,
   1064227081,
   1065353216,
   0,
   0,
   1048912119,
   1063178432,
   1065353216,
   0,
   0,
   1060254519,
   1064190510,
   1065353216,
   0,
   0,
   1048912119,
   1064190510,
   1065353216,
   0,
   0,
   1060254519,
   1063178432,
   3212836864,
   0,
   0,
   1060855949,
   1065227194,
   3212836864,
   0,
   0,
   1057160901,
   1064227081,
   3212836864,
   0,
   0,
   1060855949,
   1064227081,
   3212836864,
   0,
   0,
   1057160901,
   1065227194,
   3212836864,
   0,
   0,
   1049669110,
   1064223759,
   3212836864,
   0,
   0,
   1057011908,
   1065223871,
   3212836864,
   0,
   0,
   1049669110,
   1065223871,
   3212836864,
   0,
   0,
   1057011908,
   1064223759;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { ______001 }

  AnimationKey S {
   1;
   1;
   0;3;4.070799,4.070799,4.070799;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;-0.071342,22.916149,-0.062733;;;
  }
 }
}