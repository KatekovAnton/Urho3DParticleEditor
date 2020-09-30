 
#include "Convert.h"

const JSONValue& at(const JSONArray& v, unsigned index){
    if (index < v.Size()) {
        return v[index];
    }
    return JSONValue::EMPTY;
}

const JSONValue& at(const JSONValue& v, unsigned index){
    return at(v.GetArray(), index);
}

template <> std::string to<std::string, int>(const int& v){ return std::to_string(v); }
template <> std::string to<std::string, float>(const float& v){ return std::to_string(v); }
template <> std::string to<std::string, double>(const double& v){ return std::to_string(v); }
template <> std::string to<std::string, bool>(const bool& v){ return v ? "true" : "false"; }
template <> std::string to<std::string, unsigned>(const unsigned& v){ return std::to_string(v); }

template <> String to<String, int>(const int& v){ return String(v); }
template <> String to<String, unsigned>(const unsigned& v){ return String(v); }
template <> String to<String, int64_t>(const int64_t& v){ return String(v); }
template <> String to<String, bool>(const bool& v){ return String(v); }
template <> String to<String, float>(const float& v){ return String(v); }
template <> String to<String, double>(const double& v){ return String(v); }

template <> String to<String, std::string>(const std::string& v){ return String(v.c_str()); }
template <> std::string to<std::string, String>(const String& v){ return std::string(v.CString()); }


template <> int to<int, String>(const String& v){ return atoi(v.CString()); }
template <> float to<float, String>(const String& v){ return atof(v.CString()); }
template <> double to<double, String>(const String& v){ return std::stod(v.CString()); }
template <> bool to<bool, String>(const String& v){ return v == "true" ? true : v == "1" ? true : false; }
template <> unsigned to<unsigned, String>(const String& v){ return atoi(v.CString()); }

template <> int to<int, std::string>(const std::string& v){ return atoi(v.c_str()); }
template <> float to<float, std::string>(const std::string& v){ return atof(v.c_str()); }
template <> double to<double, std::string>(const std::string& v){ return std::stod(v); }
template <> bool to<bool, std::string>(const std::string& v){ return v == "true" ? true : v == "1" ? true : false; }
template <> unsigned to<unsigned, std::string>(const std::string& v){ return atoi(v.c_str()); }

template <> double to<double, int>(const int& v){ return v; }
template <> double to<double, bool>(const bool& v){ return v ? 1.0 : 0.0; }
template <> double to<double, float>(const float& v){ return v; }
template <> double to<double, unsigned>(const unsigned& v){ return v; }

template <> unsigned to<unsigned, int>(const int& v){ return v; }
template <> unsigned to<unsigned, bool>(const bool& v){ return v ? 1 : 0; }
template <> unsigned to<unsigned, float>(const float& v){ return v; }
template <> unsigned to<unsigned, double>(const double& v){ return v; }

template <> float to<float, int>(const int& v){ return v; }
template <> float to<float, bool>(const bool& v){ return v ? 1.0f : 0.0f; }
template <> float to<float, double>(const double& v){ return v; }
template <> float to<float, unsigned>(const unsigned& v){ return v; }

template <> int to<int, double>(const double& v){ return v; }
template <> int to<int, bool>(const bool& v){ return v ? 1 : 0; }
template <> int to<int, float>(const float& v){ return v; }
template <> int to<int, unsigned>(const unsigned& v){ return v; }

template <> bool to<bool, double>(const double& v){ return v; }
template <> bool to<bool, int>(const int& v){ return v; }
template <> bool to<bool, float>(const float& v){ return v; }
template <> bool to<bool, unsigned>(const unsigned& v){ return v; }

template <> int to<int, char>(const char* v){ return atoi(v); }
template <> float to<float, char>(const char* v){ return atof(v); }
template <> double to<double, char>(const char* v){ return std::stod(v); }
template <> unsigned to<unsigned, char>(const char* v){ return atoi(v); }


template <> bool to<bool, JSONValue>(const JSONValue& v){ return v.GetBool(); }
template <> int to<int, JSONValue>(const JSONValue& v){ return v.GetInt(); }
template <> unsigned to<unsigned, JSONValue>(const JSONValue& v){ return v.GetUInt(); }
template <> float to<float, JSONValue>(const JSONValue& v){ return v.GetFloat(); }
template <> double to<double, JSONValue>(const JSONValue& v){ return v.GetDouble(); }
template <> String to<String, JSONValue>(const JSONValue& v){ return v.GetString(); }
    
template <> JSONValue to<JSONValue, bool>(const bool& v){ return v; }
template <> JSONValue to<JSONValue, int>(const int& v){ return v; }
template <> JSONValue to<JSONValue, unsigned>(const unsigned& v){ return v; }
template <> JSONValue to<JSONValue, float>(const float& v){ return v; }
template <> JSONValue to<JSONValue, double>(const double& v){ return v; }
template <> JSONValue to<JSONValue, String>(const String& v){ return v; }


template <> bool to<bool, Variant>(const Variant& v){ return v.GetBool(); }
template <> int to<int, Variant>(const Variant& v){ return v.GetInt(); }
template <> unsigned to<unsigned, Variant>(const Variant& v){ return v.GetUInt(); }
template <> float to<float, Variant>(const Variant& v){ return v.GetFloat(); }
template <> double to<double, Variant>(const Variant& v){ return v.GetDouble(); }
template <> String to<String, Variant>(const Variant& v){ return v.GetString(); }
template <> long long to<long long , Variant>(const Variant& v){ return v.GetInt64(); }
template <> unsigned long long to<unsigned long long, Variant>(const Variant& v){ return v.GetUInt64(); }
template <> StringHash to<StringHash, Variant>(const Variant& v){ return v.GetStringHash(); }
template <> Vector2 to<Vector2, Variant>(const Variant& v){ return v.GetVector2(); }
template <> Vector3 to<Vector3, Variant>(const Variant& v){ return v.GetVector3(); }
template <> Vector4 to<Vector4, Variant>(const Variant& v){ return v.GetVector4(); }
template <> Quaternion to<Quaternion, Variant>(const Variant& v){ return v.GetQuaternion(); }
template <> Color to<Color, Variant>(const Variant& v){ return v.GetColor(); }
/*
template <> PODVector<unsigned char>& to<PODVector<unsigned char>&, Variant>(const Variant& v){ return v.GetBuffer(); }
template <> VectorBuffer to<VectorBuffer, Variant>(const Variant& v){ return v.GetVectorBuffer(); }
template <> void* to<void*, Variant>(const Variant& v){ return v.GetVoidPtr(); }
template <> ResourceRef& to<ResourceRef&, Variant>(const Variant& v){ return v.GetResourceRef(); }
template <> ResourceRefList& to<ResourceRefList&, Variant>(const Variant& v){ return v.GetResourceRefList(); }
template <> VariantVector& to<VariantVector&, Variant>(const Variant& v){ return v.GetVariantVector(); }
template <> StringVector&  to<StringVector& , Variant>(const Variant& v){ return v.GetStringVector(); }
template <> VariantMap& to<VariantMap&, Variant>(const Variant& v){ return v.GetVariantMap(); }
template <> Rect& to<Rect&, Variant>(const Variant& v){ return v.GetRect(); }
template <> IntRect& to<IntRect&, Variant>(const Variant& v){ return v.GetIntRect(); }
template <> IntVector2& to<IntVector2&, Variant>(const Variant& v){ return v.GetIntVector2(); }
template <> IntVector3& to<IntVector3&, Variant>(const Variant& v){ return v.GetIntVector3(); }
template <> RefCounted* to<RefCounted*, Variant>(const Variant& v){ return v.GetPtr(); }
template <> Matrix3& to<Matrix3&, Variant>(const Variant& v){ return v.GetMatrix3(); }
template <> Matrix3x4& to<Matrix3x4&, Variant>(const Variant& v){ return v.GetMatrix3x4(); }
template <> Matrix4& to<Matrix4&, Variant>(const Variant& v){ return v.GetMatrix4(); }
*/
    
template <> Variant to<Variant, bool>(const bool& v){ return v; }
template <> Variant to<Variant, int>(const int& v){ return v; }
template <> Variant to<Variant, unsigned>(const unsigned& v){ return v; }
template <> Variant to<Variant, float>(const float& v){ return v; }
template <> Variant to<Variant, double>(const double& v){ return v; }
template <> Variant to<Variant, String>(const String& v){ return v; }

template <> Variant to<Variant, JSONValue>(const JSONValue& v){
    switch (v.GetValueType())
    {
        case JSON_BOOL: return Variant(v.GetBool()); break;
        case JSON_NUMBER: return Variant(v.GetDouble()); break;
        case JSON_STRING: return Variant(v.GetString()); break;
        
        case JSON_ARRAY: {
            VariantVector variantVector;
            const JSONArray& a = v.GetArray();
            for (auto i = a.Begin(); i != a.End(); ++i)
                 variantVector.Push( to<Variant>(*i) );
            return Variant(variantVector); break;
        }
    
        case JSON_OBJECT: {
            VariantMap variantMap;
            for (auto i = v.Begin(); i != v.End(); ++i)
                 variantMap[i->first_] = to<Variant>(i->second_);
            return Variant(variantMap); break;
        }
        
        default: break;
    }
    return Variant();
}


template <> Vector4 to<Vector4, String>(const String& v){ Vector4 r; SSCANF(v.CString(),"%g %g %g %g", &r.x_, &r.y_, &r.z_, &r.w_ ); return r; }
template <> Vector3 to<Vector3, String>(const String& v){ Vector3 r; SSCANF(v.CString(),"%g %g %g", &r.x_, &r.y_, &r.z_ ); return r; }
template <> Vector2 to<Vector2, String>(const String& v){ Vector2 r; SSCANF(v.CString(),"%g %g", &r.x_, &r.y_ ); return r; }
template <> IntVector3 to<IntVector3, String>(const String& v){ IntVector3 r; SSCANF(v.CString(),"%d %d %d", &r.x_, &r.y_, &r.z_ ); return r; }
template <> IntVector2 to<IntVector2, String>(const String& v){ IntVector2 r; SSCANF(v.CString(),"%d %d", &r.x_, &r.y_ ); return r; }


// массивы хранятся и парсятся лучше
 
template <> JSONValue to<JSONValue, Vector2>(const Vector2& v){
    return JSONArray({  v.x_, v.y_ });
}
template <> Vector2 to<Vector2, JSONValue>(const JSONValue& v){ 
    if (v.IsArray()) return Vector2(at(v, 0).GetFloat(), at(v, 1).GetFloat());
    if (v.IsObject()) return Vector2(v["x"].GetFloat(), v["y"].GetFloat());
    if (v.IsNumber()) { float n = v.GetFloat(); return Vector2(n, n); }
    return Vector2(0.0f, 0.0f);
}

template <> JSONValue to<JSONValue, IntVector2>(const IntVector2& v){
    return JSONArray({  v.x_, v.y_ });
}
template <> IntVector2 to<IntVector2, JSONValue>(const JSONValue& v){ 
    if (v.IsArray()) return IntVector2(at(v, 0).GetInt(), at(v, 1).GetInt());
    if (v.IsObject()) return IntVector2(v["x"].GetInt(), v["y"].GetInt());
    if (v.IsNumber()) { int n = v.GetInt(); return IntVector2(n, n); }
    return IntVector2(0, 0);
}

template <> JSONValue to<JSONValue, Vector3>(const Vector3& v){ 
    return JSONArray({ v.x_, v.y_, v.z_ });
}

template <> Vector3 to<Vector3, JSONValue>(const JSONValue& v){ 
    if (v.IsArray()) return Vector3(at(v, 0).GetFloat(), at(v, 1).GetFloat(), at(v, 2).GetFloat());
    if (v.IsObject()) return Vector3(v["x"].GetFloat(), v["y"].GetFloat(), v["z"].GetFloat());
    if (v.IsNumber()) { float n = v.GetFloat(); return Vector3(n, n, n); }
    return Vector3(0.0f, 0.0f, 0.0f);
}

template <> JSONValue to<JSONValue, Quaternion>(const Quaternion& v){ 
    return JSONArray({ v.x_, v.y_, v.z_, v.w_ });
}

template <> JSONValue to<JSONValue, Vector4>(const Vector4& v){
    return JSONArray({  v.x_, v.y_, v.z_, v.w_ });
}

template <> Vector4 to<Vector4, JSONValue>(const JSONValue& v){ 
    if (v.IsArray()) return Vector4(at(v, 0).GetFloat(), at(v, 1).GetFloat(), at(v, 2).GetFloat(), at(v, 3).GetFloat());
    if (v.IsObject()) return Vector4(v["x"].GetFloat(), v["y"].GetFloat(), v["z"].GetFloat(), v["w"].GetFloat());
    if (v.IsNumber()) { float n = v.GetFloat(); return Vector4(n, n, n, n); }
    return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

template <> Quaternion to<Quaternion, JSONValue>(const JSONValue& v){ 
    if (v.IsArray()) return Quaternion(at(v, 0).GetFloat(), at(v, 1).GetFloat(), at(v, 2).GetFloat(), at(v, 3).GetFloat());
    if (v.IsObject()) return Quaternion(v["x"].GetFloat(), v["y"].GetFloat(), v["z"].GetFloat(), v["w"].GetFloat());
    return Quaternion(0.0f,0.0f,0.0f,0.0f);
}

template <> Color to<Color, unsigned>(const unsigned& v){ 
    // ARGB
    return Color(
        ((float)((v >> 16) & 255u)) / 255.0f,
        ((float)((v >>  8) & 255u)) / 255.0f,
        ((float)((v >>  0) & 255u)) / 255.0f,
        ((float)((v >> 24) & 255u)) / 255.0f
    );
}

template <> unsigned to<unsigned, Color>(const Color& c){
    // ARGB
    return (((unsigned)(255u * c.r_) & 255u) << 16) |
           (((unsigned)(255u * c.g_) & 255u) << 8 ) |
           (((unsigned)(255u * c.b_) & 255u) << 0 ) |
           (((unsigned)(255u * c.a_) & 255u) << 24);
}




template <> Color to<Color, String>(const String& s){ 
    char* cs = (char*)s.CString();
    if (*cs == '#') cs++;
    switch ( strlen(cs) ){
        case 8: {
            unsigned r, g, b, a;
            SSCANF( cs, "%2x%2x%2x%2x", &r, &g, &b, &a); 
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }
        break;
        case 6: {
            unsigned r, g, b;
            SSCANF( cs, "%2x%2x%2x", &r, &g, &b);
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        }
        default: 
            unsigned c; 
            SSCANF( cs, "%x", &c );
            return to<Color>(c);
    }
    return Color(1.0f,1.0f,1.0f,1.0f);
}

inline float alphaConv(const JSONValue& v){ return v.IsNull() ? 1.0f : v.GetFloat(); }
template <> Color to<Color, JSONValue>(const JSONValue& v){ 
    if (v.IsString()) return to<Color>(v.GetString());
    if (v.IsArray()) return Color(at(v, 0).GetFloat(), at(v, 1).GetFloat(), at(v, 2).GetFloat(), alphaConv(at(v, 3)));
    if (v.IsObject()) return Color(v["r"].GetFloat(), v["g"].GetFloat(), v["b"].GetFloat(), alphaConv(v["a"]));
    if (v.IsNumber()) {
        float n = v.GetFloat(); if (n < 1){ return Color(n,n,n,n); }
        return to<Color>((unsigned)n);
    }
    return Color(1.0f,1.0f,1.0f,1.0f);
}




struct s_test { 
public:
    int i = 10;
    Vector3 vec3;
    String ToString() const { return "some"; }
    
    // inner converts sample
    
    void fromJson(const JSONValue& v){ 
        assign( v.Get("i"), i );
        assign( v.Get("vec3"), vec3 );
    }
    
    JSONValue toJson() const { 
        JSONValue v;
        assign( i, v["i"] );
        assign( vec3, v["vec3"] );
        return v; 
    }
};

// foreign converts sample
// template <> JSONValue to<JSONValue, s_test>(const s_test& v){ return JSONValue(); }
// template <> s_test to<s_test, JSONValue>(const JSONValue& v){ return s_test(); }


void test_to_converts(){
    s_test a;
    // будет вызван a.ToString()
    auto ss = to<String>(a);
    a.i = to<int>("123");
    a = to<s_test>(  to<JSONValue>(a) );
}
 
 
#include <Urho3D/Graphics/GraphicsDefs.h>
// enum to string

ENUM_CONVERT_HELPER( Urho3D::PrimitiveType, TRIANGLE_LIST, LINE_LIST, POINT_LIST, TRIANGLE_STRIP, LINE_STRIP, TRIANGLE_FAN );

/// %Geometry type for vertex shader geometry variations.
ENUM_CONVERT_HELPER( GeometryType, GEOM_STATIC, GEOM_SKINNED, GEOM_INSTANCED, GEOM_BILLBOARD, GEOM_DIRBILLBOARD, GEOM_TRAIL_FACE_CAMERA, GEOM_TRAIL_BONE, MAX_GEOMETRYTYPES, GEOM_STATIC_NOINSTANCING );

/// Blending mode.
ENUM_CONVERT_HELPER( BlendMode,
    BLEND_REPLACE, BLEND_ADD, BLEND_MULTIPLY, BLEND_ALPHA, BLEND_ADDALPHA, BLEND_PREMULALPHA, BLEND_INVDESTALPHA, BLEND_SUBTRACT, BLEND_SUBTRACTALPHA, MAX_BLENDMODES );

/// Depth or stencil compare mode.
ENUM_CONVERT_HELPER( CompareMode,
    CMP_ALWAYS, CMP_EQUAL, CMP_NOTEQUAL, CMP_LESS, CMP_LESSEQUAL, CMP_GREATER, CMP_GREATEREQUAL, MAX_COMPAREMODES );

/// Culling mode.
ENUM_CONVERT_HELPER( CullMode,
    CULL_NONE, CULL_CCW, CULL_CW, MAX_CULLMODES );

/// Fill mode.
ENUM_CONVERT_HELPER( FillMode,
    FILL_SOLID, FILL_WIREFRAME, FILL_POINT );

/// Stencil operation.
ENUM_CONVERT_HELPER( StencilOp,
    OP_KEEP, OP_ZERO, OP_REF, OP_INCR, OP_DECR );

/// Vertex/index buffer lock state.
ENUM_CONVERT_HELPER( LockState,
    LOCK_NONE, LOCK_HARDWARE, LOCK_SHADOW, LOCK_SCRATCH );

/// Hardcoded legacy vertex elements.
ENUM_CONVERT_HELPER( LegacyVertexElement,
    ELEMENT_POSITION, ELEMENT_NORMAL, ELEMENT_COLOR, ELEMENT_TEXCOORD1, ELEMENT_TEXCOORD2, ELEMENT_CUBETEXCOORD1, ELEMENT_CUBETEXCOORD2, ELEMENT_TANGENT, ELEMENT_BLENDWEIGHTS, ELEMENT_BLENDINDICES, ELEMENT_INSTANCEMATRIX1, ELEMENT_INSTANCEMATRIX2, ELEMENT_INSTANCEMATRIX3, ELEMENT_OBJECTINDEX, MAX_LEGACY_VERTEX_ELEMENTS );

/// Arbitrary vertex declaration element datatypes.
ENUM_CONVERT_HELPER( VertexElementType,
    TYPE_INT, TYPE_FLOAT, TYPE_VECTOR2, TYPE_VECTOR3, TYPE_VECTOR4, TYPE_UBYTE4, TYPE_UBYTE4_NORM, MAX_VERTEX_ELEMENT_TYPES );

/// Arbitrary vertex declaration element semantics.
ENUM_CONVERT_HELPER( VertexElementSemantic,
    SEM_POSITION, SEM_NORMAL, SEM_BINORMAL, SEM_TANGENT, SEM_TEXCOORD, SEM_COLOR, SEM_BLENDWEIGHTS, SEM_BLENDINDICES, SEM_OBJECTINDEX, MAX_VERTEX_ELEMENT_SEMANTICS );

/// Texture filtering mode.
ENUM_CONVERT_HELPER( TextureFilterMode,
    FILTER_NEAREST, FILTER_BILINEAR, FILTER_TRILINEAR, FILTER_ANISOTROPIC, FILTER_NEAREST_ANISOTROPIC, FILTER_DEFAULT, MAX_FILTERMODES );

/// Texture addressing mode.
ENUM_CONVERT_HELPER( TextureAddressMode,
    ADDRESS_WRAP, ADDRESS_MIRROR, ADDRESS_CLAMP, ADDRESS_BORDER, MAX_ADDRESSMODES );

/// Texture coordinates.
ENUM_CONVERT_HELPER( TextureCoordinate,
    COORD_U, COORD_V, COORD_W, MAX_COORDS );

/// Texture usage types.
ENUM_CONVERT_HELPER( TextureUsage,
    TEXTURE_STATIC, TEXTURE_DYNAMIC, TEXTURE_RENDERTARGET, TEXTURE_DEPTHSTENCIL );

/// Cube map faces.
ENUM_CONVERT_HELPER( CubeMapFace,
    FACE_POSITIVE_X, FACE_NEGATIVE_X, FACE_POSITIVE_Y, FACE_NEGATIVE_Y, FACE_POSITIVE_Z, FACE_NEGATIVE_Z, MAX_CUBEMAP_FACES );

/// Cubemap single image layout modes.
ENUM_CONVERT_HELPER( CubeMapLayout,
    CML_HORIZONTAL, CML_HORIZONTALNVIDIA, CML_HORIZONTALCROSS, CML_VERTICALCROSS, CML_BLENDER );

/// Update mode for render surface viewports.
ENUM_CONVERT_HELPER( RenderSurfaceUpdateMode,
    SURFACE_MANUALUPDATE, SURFACE_UPDATEVISIBLE, SURFACE_UPDATEALWAYS );

/// Shader types.
ENUM_CONVERT_HELPER( ShaderType, VS, PS );

/// Shader parameter groups for determining need to update. On APIs that support constant buffers, these correspond to different constant buffers.
ENUM_CONVERT_HELPER( ShaderParameterGroup,
    SP_FRAME, SP_CAMERA, SP_ZONE, SP_LIGHT, SP_MATERIAL, SP_OBJECT, SP_CUSTOM, MAX_SHADER_PARAMETER_GROUPS );

/// Texture units.
// ENUM_CONVERT_HELPER( TextureUnit,
//     TU_DIFFUSE, TU_ALBEDOBUFFER, TU_NORMAL, TU_NORMALBUFFER, TU_SPECULAR, TU_EMISSIVE, TU_ENVIRONMENT ,#ifdef DESKTOP_GRAPHICS
//     TU_VOLUMEMAP, TU_CUSTOM1, TU_CUSTOM2, TU_LIGHTRAMP, TU_LIGHTSHAPE, TU_SHADOWMAP, TU_FACESELECT, TU_INDIRECTION, TU_DEPTHBUFFER, TU_LIGHTBUFFER, TU_ZONE, MAX_MATERIAL_TEXTURE_UNITS, MAX_TEXTURE_UNITS = 16
// #else
//     TU_LIGHTRAMP, TU_LIGHTSHAPE, TU_SHADOWMAP, MAX_MATERIAL_TEXTURE_UNITS, MAX_TEXTURE_UNITS = 8
// #endif
// );

/// Billboard camera facing modes.
ENUM_CONVERT_HELPER( FaceCameraMode,
    FC_NONE, FC_ROTATE_XYZ, FC_ROTATE_Y, FC_LOOKAT_XYZ, FC_LOOKAT_Y, FC_LOOKAT_MIXED, FC_DIRECTION );

/// Shadow type.
ENUM_CONVERT_HELPER( ShadowQuality,
    SHADOWQUALITY_SIMPLE_16BIT, SHADOWQUALITY_SIMPLE_24BIT, SHADOWQUALITY_PCF_16BIT, SHADOWQUALITY_PCF_24BIT, SHADOWQUALITY_VSM, SHADOWQUALITY_BLUR_VSM );

ENUM_CONVERT_HELPER( MaterialQuality,
    QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH, QUALITY_MAX );

ENUM_CONVERT_HELPER( ClearTarget,
    CLEAR_COLOR, CLEAR_DEPTH, CLEAR_STENCIL );

// Legacy vertex element bitmasks.
ENUM_CONVERT_HELPER( VertexMask,
    MASK_NONE, MASK_POSITION, MASK_NORMAL, MASK_COLOR, MASK_TEXCOORD1, MASK_TEXCOORD2, MASK_CUBETEXCOORD1, MASK_CUBETEXCOORD2, MASK_TANGENT, MASK_BLENDWEIGHTS, MASK_BLENDINDICES, MASK_INSTANCEMATRIX1, MASK_INSTANCEMATRIX2, MASK_INSTANCEMATRIX3, MASK_OBJECTINDEX );
   
