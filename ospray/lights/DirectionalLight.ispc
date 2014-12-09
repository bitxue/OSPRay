/********************************************************************* *\
 * INTEL CORPORATION PROPRIETARY INFORMATION                            
 * This software is supplied under the terms of a license agreement or  
 * nondisclosure agreement with Intel Corporation and may not be copied 
 * or disclosed except in accordance with the terms of that agreement.  
 * Copyright (C) 2014 Intel Corporation. All Rights Reserved.           
 ********************************************************************* */

#include "DirectionalLight.ih"

//!< Construct a new ispc-side DirectionalLight object
extern void DirectionalLight_Constructor( uniform DirectionalLight *uniform THIS,
                                          void *uniform cppEquivalent,
                                          const uniform vec3f &color,
                                          const uniform vec3f &direction)
{
  Light_Constructor(&THIS->base, cppEquivalent);
  THIS->color = color;
  THIS->direction = direction;
  THIS->base.computeRadiance = DirectionalLight_computeRadiance;
}

//!< Destroy an ispc-side DirectionLight object
extern void DirectionalLight_Destructor(uniform DirectionalLight *uniform THIS)
{
  Light_Destructor(&THIS->base);
}

//! Compute the radiance at a point from a sample on the light sans occluders, return the extant light vector and distance.
varying vec3f DirectionalLight_computeRadiance(void *uniform pointer, const varying vec3f &coordinates, varying vec3f &direction, varying float &distance) {

    //! Cast to the actual Light subtype.
    const DirectionalLight *uniform light = (DirectionalLight *uniform) pointer;

    //! Normalized light vector.
    direction = negate(light->direction);

    //! Distance from the hit point to the sample point on the light.
    distance = inf;

    //! Radiance.
    return(light->color);

}

//!< Create an ispc_side DirectionalLight object (called from c++)
export void *uniform DirectionalLight_create(void *uniform cppEquivalent)
{
  uniform DirectionalLight *uniform light = uniform new uniform DirectionalLight;
  DirectionalLight_Constructor(light, cppEquivalent, make_vec3f(1.f, 1.f, 1.f), make_vec3f(0.f, -1.f, 0.f));
  return light;
}

//!< Destroy an ispc-side DirectionalLIght object (called from c++)
export void DirectionalLight_destroy(void *uniform _light) {
  uniform DirectionalLight *uniform light = uniform new uniform DirectionalLight;
  DirectionalLight_Destructor(light);
  delete light;
}

//!< Set the parameters of an ispc-side DirectionalLight object (called from c++ side)
export void DirectionalLight_set( void *uniform _light,
                                  const uniform vec3f &color,
                                  const uniform vec3f &direction)
{
  uniform DirectionalLight *uniform light = (uniform DirectionalLight *uniform)_light;
  light->color = color;
  light->direction = direction;
}