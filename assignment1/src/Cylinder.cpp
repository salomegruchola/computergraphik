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

    size_t nsol = solveQuadratic(dot(d,d)-dot(d,h)*dot(d,h)*dot(h,h),
                                 +2*dot(z,d)-2*dot(z,h)*dot(d,h)*dot(h,h),
                                 dot(z,z)-dot(z,h)*dot(z,h)*dot(h,h)-radius*radius,t);
    
    _intersection_t = NO_INTERSECTION;
    
    // Find the closest valid solution (in front of the viewer)
    int no_sol = 0;
    
    for (size_t i = 0; i < nsol; ++i) {
        if (t[i] > 0){
            // if there is only one solution, check if higher than cylinder, otherwise take this as intersection point
            if(nsol == 1){
                if(abs(dot((_ray(t[i]) - center),axis)) > 1){}
                else{_intersection_t = t[i];}
            }
            // if there are two solutions, check which one is closer to the viewer, but check also, if the solutions are not above the hight of the cylinder
            if (nsol == 2){
                if(i == 0){
                    if(abs(dot((_ray(t[i]) - center),axis)) > 1){no_sol = 1;}
                    else{_intersection_t = std::min(_intersection_t,t[i]);}
                }
                if(i == 1){
                    if(no_sol ==0){
                        if(abs(dot((_ray(t[i]) - center),axis)) > 1){_intersection_t = _intersection_t;}
                        else{_intersection_t = std::min(_intersection_t,t[i]);}
                    }
                    if(no_sol ==1){
                        if(abs(dot((_ray(t[i]) - center),axis)) > 1){_intersection_t = NO_INTERSECTION;}
                        else{_intersection_t = t[i];}
                    }
                }
            }
            else{_intersection_t = NO_INTERSECTION;}
        }
        //printf("%f", t[i]);
    }
    
    if (_intersection_t == NO_INTERSECTION) return false;
    _intersection_point  = _ray(_intersection_t);
    
    // compute normal
    if(nsol == 1){
        _intersection_normal = ((_intersection_point - center) -
                                          dot(_intersection_point - center,h)*h)/radius;
    }
    // if there are two solutions and the one further away from the spectator is chosen, the normal needs to point into the other direction --> add a minus
    else{
        if(_intersection_t == std::max(t[0],t[1])){_intersection_normal = -((_intersection_point - center) -
                                                                            dot(_intersection_point - center,h)*h)/radius;}
        else{_intersection_normal = ((_intersection_point - center) -
                                     dot(_intersection_point - center,h)*h)/radius;}
    }
    
    return true;
}
// make
// ./raytrace ../scenes/cylinders/cylinders.sce cylinders_output.tga

//blablabla
// blabliblablu

/*

 for (size_t i = 0; i < nsol; ++i) {
 if (t[i] > 0){
 if(nsol == 1){
 if(abs(dot((_ray(t[i]) - center),axis)) > 1){
 _intersection_t = NO_INTERSECTION;
 }
 else{_intersection_t = t[i];}
 }
 if (nsol == 2){
 if(abs(dot((_ray(t[i]) - center),axis)) > 1){}
 else{_intersection_t = std::min(_intersection_t,t[i]);}
 }
 else{
 _intersection_t = NO_INTERSECTION;
 }
 }
 //printf("%f", t[i]);
 }
 */
