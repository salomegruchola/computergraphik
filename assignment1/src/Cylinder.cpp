//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================

#include "Cylinder.h"
#include "SolveQuadratic.h"

#include <array>
#include <cmath>

//== IMPLEMENTATION =========================================================
//Cylinder::Cylinder(const vec3& _center, double _radius, const vec3& _axis, double _height)
//: center(_center), radius(_radius), axis(_axis), height(_height)
//{
//}

//-----------------------------------------------------------------------------


bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{
    /** \todo
     * - compute the first valid intersection `_ray` with the cylinder
     *   (valid means in front of the viewer: t > 0)
     * - store intersection point in `_intersection_point`
     * - store ray parameter in `_intersection_t`
     * - store normal at _intersection_point in `_intersection_normal`.
     * - return whether there is an intersection with t > 0
    */
    const vec3 &d = _ray.direction;
    const vec3 &z = _ray.origin  - center;
    const vec3 &h = axis;
    //const vec3  nor = _normal;
    

    
    // do ray tracing with an infinite cylinder
    // then determine whether the ray goes through in-between the two planes
    
    std::array<double, 2> t;
    
    double a;
    double b;
    
   // a = dot(z,z) + 2*t* dot(d,z) + t*t * dot(d,d);
   // b = dot(z,axis)*dot(z,axis) + 2*t * dot(z,axis) * dot(d,axis) + t * t * dot(d,axis) * dot(d,axis);
    
    double k,l,m;
    k = dot(d,d) - dot(d,axis) * dot(d,axis);
    l = 2 * dot(d,z) - 2 * dot(z,axis) * dot(d,axis);
    m =dot(z,z) - dot(z,axis)*dot(z,axis) - radius * radius;
    
    size_t nsol = solveQuadratic(k,l,m ,t);
    
    
  
    _intersection_t = NO_INTERSECTION;
    
    
    // Find the closest valid solution (in front of the viewer)
    for (size_t i = 0; i < nsol; ++i) {
        if (t[i] > 0) _intersection_t = std::min(_intersection_t,t[i]);
        //printf("%f", t[i]);
    }
    

    
    if (_intersection_t == NO_INTERSECTION) return false;

    _intersection_point  = _ray(_intersection_t);
    
    
    
    double height_ofIntersectionPoint = dot(_intersection_point - center,axis);
    
    if (height_ofIntersectionPoint * height_ofIntersectionPoint > 1  ) return false;
    printf("%f", height_ofIntersectionPoint);
    _intersection_normal = normalize( (_intersection_point - center) -
                                     dot(_intersection_point - center,axis)*axis);
    
    return true;
    
}
// make
// ./raytrace ../scenes/cylinders/cylinders.sce cylinders_output.tga

//blablabla
// blabliblablu

