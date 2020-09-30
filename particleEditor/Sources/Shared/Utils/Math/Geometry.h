#ifndef MathKit_h
#define MathKit_h

#if defined PROJECT_TARGET_OS_MAC || defined PROJECT_TARGET_OS_IPHONE || defined PROJECT_TARGET_OS_WIN
typedef unsigned int uint;
#endif
#include "math.h"
#if defined PROJECT_TARGET_OS_WIN
#define __inline__ inline
#endif


template <typename T> static __inline__
T ____min(T a, T b)
{
    return a<b?a:b;
}

template <typename T> static __inline__
T ____max(T a, T b)
{
    return a>b?a:b;
}

template <typename T> static __inline__
T ____abs(T a)
{
    return a>0?a:-a;
}

template <typename T> static __inline__
T ____lerp(T from, T to, float thetta)
{
    return from + (to - from) * thetta;
}

struct __GPoint2D {
    float x;
    float y;
    
    __GPoint2D() :x(0), y(0) {}
    __GPoint2D(float _x, float _y) :x(_x), y(_y) {}
    
    float getX() const {return x;}
    float getY() const {return y;}
    
    void setX(float value) {x = value;}
    void setY(float value) {y = value;}
};

typedef struct __GPoint2D GPoint2D;

static __inline__ GPoint2D
GPoint2DMult(const GPoint2D& v, const float s)
{
    return GPoint2D(v.x*s, v.y*s);
}

static __inline__ GPoint2D
GPoint2DAdd(const GPoint2D& v1, const GPoint2D& v2)
{
    return GPoint2D(v1.x + v2.x, v1.y + v2.y);
}

static __inline__ GPoint2D
GPoint2DSub(const GPoint2D& v1, const GPoint2D& v2)
{
    return GPoint2D(v1.x - v2.x, v1.y - v2.y);
}

static __inline__ float
GPoint2DDot(const GPoint2D& v1, const GPoint2D& v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

static __inline__ float
GPoint2DLengthSQ(const GPoint2D& v)
{
    return GPoint2DDot(v, v);
}

static __inline__ float
GPoint2DLength(const GPoint2D& v)
{
    return sqrtf(GPoint2DLengthSQ(v));
}

struct __GSize2D {
    float height;
    float width;
    
    __GSize2D() :width(0), height(0) {}
    __GSize2D(float _w, float _h) :width(_w), height(_h) {}
    
    float getW() const {return width;}
    float getH() const {return height;}
    
    void setW(float value) {width = value;}
    void setH(float value) {height = value;}
};

typedef struct __GSize2D GSize2D;


struct __GCircle2D {
    GPoint2D center;
    float radius;
    
    __GCircle2D() :center(GPoint2D()), radius(0) {}
    __GCircle2D(GPoint2D _c, float _r) :center(_c), radius(_r) {}
    
    GPoint2D getCenter() const {return center;}
    float getRadius() const {return radius;}
    
    void setCenter(GPoint2D value) {center = value;}
    void setRadius(float value) {radius = value;}
};

typedef struct __GCircle2D GCircle2D;

struct __GRect2D {
    GPoint2D   origin;
    GSize2D    size;
    
    __GRect2D() :origin(GPoint2D()), size(GSize2D()) {}
    __GRect2D(float x, float y, float width, float height) :origin(GPoint2D(x, y)), size(GSize2D(width, height)) {}
    
    float getX() const {return origin.x;}
    float getY() const {return origin.y;}
    
    float getW() const {return size.width;}
    float getH() const {return size.height;}
    
    void setX(float value) {origin.x = value;}
    void setY(float value) {origin.y = value;}
    
    void setW(float value) {size.width = value;}
    void setH(float value) {size.height = value;}
    
    GPoint2D GetCenter() const { return GPoint2D(origin.x + size.width/2, origin.y + size.height/2);}
};

typedef struct __GRect2D GRect2D;



static __inline__ GRect2D GRect2DMake(float x, float y, float width, float height) {
    GRect2D rect;
    rect.origin.x = x;
    rect.origin.y = y;
    rect.size.width = width;
    rect.size.height = height;
    return rect;
};



struct __BoundingBox2D {
    GPoint2D min;
    GPoint2D max;
    
    GPoint2D GetCenter() const {return GPoint2D((min.x + max.x)/2.0f, (min.y + max.y)/2.0f); }
    GPoint2D GetSize() const {return GPoint2D(- (min.x - max.x), -(min.y - max.y)); }
    
    void AddBoundingBox(__BoundingBox2D bb)
    {
        min.x = ____min(min.x, bb.min.x);
        min.y = ____min(min.y, bb.min.y);
        max.x = ____max(max.x, bb.max.x);
        max.y = ____max(max.y, bb.max.y);
    }
    
};
typedef struct __BoundingBox2D BoundingBox2D;




struct __GIntPoint2D {
    float x;
    float y;
    
    __GIntPoint2D() :x(0), y(0) {}
    __GIntPoint2D(float _x, float _y) :x(_x), y(_y) {}
    
    float getX() const {return x;}
    float getY() const {return y;}
    
    void setX(float value) {x = value;}
    void setY(float value) {y = value;}
};

typedef struct __GIntPoint2D GIntPoint2D;



struct __IntBoundingBox2D {
    GIntPoint2D min;
    GIntPoint2D max;
    
    GIntPoint2D GetSize() const {return GIntPoint2D(- (min.x - max.x), -(min.y - max.y)); }
    
    void addBoundingBox(__IntBoundingBox2D bb)
    {
        min.x = ____min(min.x, bb.min.x);
        min.y = ____min(min.y, bb.min.y);
        max.x = ____max(max.x, bb.max.x);
        max.y = ____max(max.y, bb.max.y);
    }

    void clamp(const __IntBoundingBox2D &bb) 
    {
        min.x = ____max(min.x, bb.min.x);
        min.y = ____max(min.y, bb.min.y);
        max.x = ____min(max.x, bb.max.x);
        max.y = ____min(max.y, bb.max.y);
    }
    
};
typedef struct __IntBoundingBox2D IntBoundingBox2D;

//static __inline__ BoundingBox2D BoundingBox2DMake(float x1, float y1, float x2, float y2) {
//    BoundingBox2D rect;
//    rect.min.x = x1;
//    rect.min.y = y1;
//    rect.max.x = x2;
//    rect.max.y = y2;
//    return rect;
//};
//
//static __inline__ BoundingBox2D BoundingBox2DMake(GPoint2D min, GPoint2D max) {
//    BoundingBox rect;
//    rect.min = min;
//    rect.max = max;
//    return rect;
//};
//
//static __inline__ BoundingBox BoundingBoxMake(GRect2D rect) {
//    BoundingBox result;
//    result.min = GPoint2D(rect.origin.x, rect.origin.y);
//    result.max = GPoint2D(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
//    return result;
//};


enum __ContainmentType
{
    ContainmentType_Disjoint,
    ContainmentType_Contains,
    ContainmentType_Intersects
};

typedef enum __ContainmentType ContainmentType;


static __inline__ ContainmentType GetContainmentType(BoundingBox2D first, BoundingBox2D second)
{
    if (first.max.x < second.min.x || first.max.y < second.min.y ||
        first.min.x > second.max.x || first.min.y > second.max.y) {
        return ContainmentType_Disjoint;
    }
    
    if (second.min.x > first.min.x && second.min.y > first.min.y &&
        second.max.x < first.max.x && second.max.y < first.max.y) {
        return ContainmentType_Contains;
    }
    
    return ContainmentType_Intersects;
}

static __inline__ bool GRect2DContainsPoint(const GRect2D &rect, const GPoint2D &point) {
    return point.x > rect.origin.x &&
    point.y > rect.origin.y &&
    point.x < rect.origin.x + rect.size.width &&
    point.y < rect.origin.y + rect.size.height;
}

static __inline__ bool GCircle2DContainsPoint(const GCircle2D &circle, const GPoint2D &point) {
    float distanceSq = GPoint2DLengthSQ(GPoint2DSub(circle.center, point));
    return distanceSq <= pow(circle.radius, 2);
}

//http://www.opengl.org/wiki/GluProject_and_gluUnProject_code

#endif
