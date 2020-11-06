//
//  Vector3D.cpp
//  TestOpenglES
//
//  Created by Neil on 26/8/14.
//  Copyright (c) 2014 neil. All rights reserved.
//

#include "Vector3D.h"

#include <math.h>

Vector3D::Vector3D(const Vector3D &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

Vector3D::~Vector3D() {
}

Vector3D::Vector3D(float x, float y, float z, float w)  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vector3D::setTo(float x, float y, float z, float w) {
	this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vector3D::setTo(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D& Vector3D::operator=(const Vector3D &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

bool Vector3D::operator == (const Vector3D &v) {
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector3D::operator != (const Vector3D &v) {
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

Vector3D& Vector3D::operator += (const Vector3D &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3D& Vector3D::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

Vector3D& Vector3D::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

void Vector3D::copyFrom(const Vector3D &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

float Vector3D::length() {
    return sqrt(x * x + y * y + z * z);
}

float Vector3D::lengthSquared() {
    return x * x + y * y + z * z;
}

float Vector3D::dot(const Vector3D &v) {
    return x * v.x + y * v.y + z * v.z;
}

Vector3D& Vector3D::crossProduct(const Vector3D &v) {
    float x1 = y * v.z - z * v.y;
    float y1 = z * v.x - x * v.z;
    float z1 = x * v.y - y * v.x;
    x = x1;
    y = y1;
    z = z1;
    return *this;
}

void Vector3D::crossProduct(const Vector3D &v1, const Vector3D &v2, Vector3D *dst) {
    dst->x = v1.y * v2.z - v1.z * v2.y;
    dst->y = v1.z * v2.x - v1.x * v2.z;
    dst->z = v1.x * v2.y - v1.y * v2.x;
}

Vector3D& Vector3D::add(const Vector3D &v) {
    x = x + v.x;
    y = y + v.y;
    z = z + v.z;
    return *this;
}

void Vector3D::add(const Vector3D &v1, const Vector3D &v2, Vector3D *dst) {
    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
    dst->z = v1.z + v2.z;
}

void Vector3D::subtract(const Vector3D &v1, const Vector3D &v2, Vector3D *dst) {
    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

Vector3D& Vector3D::subtract(const Vector3D &v) {
    x = x - v.x;
    y = y - v.y;
    z = z - v.z;
    return *this;
}

void Vector3D::negate() {
    x = -x;
    y = -y;
    z = -z;
}

void Vector3D::normalize() {
    float invLenght = 0.0;
    if (length() == 0) {
        invLenght = 0;
    } else {
        invLenght = 1.0 / length();
    }
    x *= invLenght;
    y *= invLenght;
    z *= invLenght;
}

void Vector3D::scaleBy(float f) {
    x *= f;
    y *= f;
    z *= f;
}

const Vector3D Vector3D::X_AXIS = Vector3D(1.0, 0.0, 0.0, 1.0);
const Vector3D Vector3D::Y_AXIS = Vector3D(0.0, 1.0, 0.0, 1.0);
const Vector3D Vector3D::Z_AXIS = Vector3D(0.0, 0.0, 1.0, 1.0);
const Vector3D Vector3D::ZERO   = Vector3D(0.0, 0.0, 0.0, 1.0);
