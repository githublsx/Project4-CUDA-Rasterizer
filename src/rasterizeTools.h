/**
 * @file      rasterizeTools.h
 * @brief     Tools/utility functions for rasterization.
 * @authors   Yining Karl Li
 * @date      2012-2015
 * @copyright University of Pennsylvania
 */

#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <util/utilityCore.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

/**
 * Multiplies a glm::mat4 matrix and a vec4.
 */
__host__ __device__ static
glm::vec3 multiplyMV(glm::mat4 m, glm::vec4 v) {
    return glm::vec3(m * v);
}

// CHECKITOUT
/**
 * Finds the axis aligned bounding box for a given triangle.
 */
__host__ __device__ static
AABB getAABBForTriangle(const glm::vec3 tri[3]) {
    AABB aabb;
    aabb.min = glm::vec3(
            min(min(tri[0].x, tri[1].x), tri[2].x),
            min(min(tri[0].y, tri[1].y), tri[2].y),
            min(min(tri[0].z, tri[1].z), tri[2].z));
    aabb.max = glm::vec3(
            max(max(tri[0].x, tri[1].x), tri[2].x),
            max(max(tri[0].y, tri[1].y), tri[2].y),
            max(max(tri[0].z, tri[1].z), tri[2].z));
    return aabb;
}

// CHECKITOUT
/**
 * Calculate the signed area of a given triangle.
 */
__host__ __device__ static
float calculateSignedArea(const glm::vec3 tri[3]) {
    return 0.5 * ((tri[2].x - tri[0].x) * (tri[1].y - tri[0].y) - (tri[1].x - tri[0].x) * (tri[2].y - tri[0].y));
}

// CHECKITOUT
/**
 * Helper function for calculating barycentric coordinates.
 */
__host__ __device__ static
float calculateBarycentricCoordinateValue(glm::vec2 a, glm::vec2 b, glm::vec2 c, const glm::vec3 tri[3]) {
    glm::vec3 baryTri[3];
    baryTri[0] = glm::vec3(a, 0);
    baryTri[1] = glm::vec3(b, 0);
    baryTri[2] = glm::vec3(c, 0);
    return calculateSignedArea(baryTri) / calculateSignedArea(tri);
}

// CHECKITOUT
/**
 * Calculate barycentric coordinates.
 */
__host__ __device__ static
glm::vec3 calculateBarycentricCoordinate(const glm::vec3 tri[3], glm::vec2 point) {
    float beta  = calculateBarycentricCoordinateValue(glm::vec2(tri[0].x, tri[0].y), point, glm::vec2(tri[2].x, tri[2].y), tri);
    float gamma = calculateBarycentricCoordinateValue(glm::vec2(tri[0].x, tri[0].y), glm::vec2(tri[1].x, tri[1].y), point, tri);
    float alpha = 1.0 - beta - gamma;
    return glm::vec3(alpha, beta, gamma);
}

// CHECKITOUT
/**
 * Check if a barycentric coordinate is within the boundaries of a triangle.
 */
__host__ __device__ static
bool isBarycentricCoordInBounds(const glm::vec3 barycentricCoord) {
    return barycentricCoord.x >= 0.0 && barycentricCoord.x <= 1.0 &&
           barycentricCoord.y >= 0.0 && barycentricCoord.y <= 1.0 &&
           barycentricCoord.z >= 0.0 && barycentricCoord.z <= 1.0;
}

// CHECKITOUT
/**
* Check if a barycentric coordinate is within the boundaries of a triangle.
*/
__host__ __device__ static
bool isBarycentricCoordOnBounds(const glm::vec3 barycentricCoord) {
	return barycentricCoord.x + barycentricCoord.y + barycentricCoord.z == 1.0;
}

// CHECKITOUT
/**
 * For a given barycentric coordinate, compute the corresponding z position
 * (i.e. depth) on the triangle.
 */
__host__ __device__ static
float getZAtCoordinate(const glm::vec3 barycentricCoord, const glm::vec3 tri[3]) {
    return -(barycentricCoord.x * tri[0].z
           + barycentricCoord.y * tri[1].z
           + barycentricCoord.z * tri[2].z);
}

__host__ __device__ static
float getPCZAtCoordinate(const glm::vec3 barycentricCoord, const glm::vec3 tri[3]) {
	float iz = barycentricCoord.x / tri[0].z + barycentricCoord.y / tri[1].z + barycentricCoord.z / tri[2].z;
	return 1.0f / iz;
}

template <class T>
__host__ __device__ static
T BCInterpolate(glm::vec3 bc, T v0, T v1, T v2)
{
	return bc.x*v0 + bc.y*v1 + bc.z*v2;
}

template <class T>
__host__ __device__ static
T PCBCInterpolate(glm::vec3 bc, glm::vec3 tri[3], T v0, T v1, T v2)
{
	return bc.x*v0 / tri[0].z + bc.y*v1 / tri[1].z + bc.z*v2 / tri[2].z;
}

__host__ __device__ static
glm::vec2 PCBCInterpolatetexcoord(glm::vec3 bc, float p0, float p1, float p2, glm::vec2 t0, glm::vec2 t1, glm::vec2 t2)
{
	glm::vec2 tz = bc.x * t0 / p0 + bc.y * t1 / p1 + bc.z * t2 / p2;
	float cz = bc.x / p0 + bc.y / p1 + bc.z / p2;
	return tz / cz;
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/single-vs-double-sided-triangle-backface-culling
__host__ __device__ static
bool isbackface(glm::vec3 tri[3])
{
	glm::vec3 e0 = tri[1] - tri[0];
	glm::vec3 e1 = tri[2] - tri[0];
	glm::vec3 N = glm::normalize(glm::cross(e0, e1));
	glm::vec3 dir = glm::vec3(0.f, 0.f, -1.f);
	if (glm::dot(dir, N) > 0)
		return false;
	return true;
}