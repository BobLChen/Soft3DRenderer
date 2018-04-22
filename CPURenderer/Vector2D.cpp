//
//  Vector2D.cpp
//  Opengl2D
//
//  Created by Neil on 14/9/14.
//  Copyright (c) 2014 neil. All rights reserved.
//

#include "Vector2D.h"

#include <math.h>


Vector2D::Vector2D() {
    x = 0.0;
    y = 0.0;
}

Vector2D::Vector2D(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D(const Vector2D &v) {
    x = v.x;
    y = v.y;
}

Vector2D::~Vector2D() {
    
}

void Vector2D::setTo(float x, float y) {
    this->x = x;
    this->y = y;
}

void Vector2D::copyFrom(const Vector2D &v) {
    x = v.x;
    y = v.y;
}

void Vector2D::normalize() {
    float length = sqrt(x * x + y * y);
    x = x / length;
    y = y / length;
}

void Vector2D::scaleBy(float value) {
    x *= value;
    y *= value;
}
