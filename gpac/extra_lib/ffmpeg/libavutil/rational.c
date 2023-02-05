/*
 * Rational numbers
 * Copyright (c) 2003 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

/**
 * @file rational.c
 * Rational numbers
 * @author Michael Niedermayer <michaelni@gmx.at>
 */

//#include <math.h>
#include <limits.h>

#include "common.h"
#include "mathematics.h"
#include "rational.h"

int av_reduce(int *dst_nom, int *dst_den, int64_t nom, int64_t den, int64_t max){
    AVRational a0={0,1}, a1={1,0};
    int sign= (nom<0) ^ (den<0);
    int64_t gcd= ff_gcd(FFABS(nom), FFABS(den));

    nom = FFABS(nom)/gcd;
    den = FFABS(den)/gcd;
    if(nom<=max && den<=max){
        a1= _AVRational((int)nom,(int)den);
        den=0;
    }
    
    while(den){
        int64_t x       = nom / den;
        int64_t next_den= nom - den*x;
        int64_t a2n= x*a1.num + a0.num;
        int64_t a2d= x*a1.den + a0.den;

        if(a2n > max || a2d > max) break;

        a0= a1;
        a1= _AVRational((int)a2n, (int)a2d);
        nom= den;
        den= next_den;
    }
    assert(ff_gcd(a1.num, a1.den) == 1);
    
    *dst_nom = sign ? -a1.num : a1.num;
    *dst_den = a1.den;
    
    return den==0;
}

/**
 * returns b*c.
 */
AVRational av_mul_q(AVRational b, AVRational c){
    av_reduce(&b.num, &b.den, b.num * (int64_t)c.num, b.den * (int64_t)c.den, INT_MAX);
    return b;
}

/**
 * returns b/c.
 */
AVRational av_div_q(AVRational b, AVRational c){
    return av_mul_q(b, _AVRational(c.den, c.num) );
}

/**
 * returns b+c.
 */
AVRational av_add_q(AVRational b, AVRational c){
    av_reduce(&b.num, &b.den, b.num * (int64_t)c.den + c.num * (int64_t)b.den, b.den * (int64_t)c.den, INT_MAX);
    return b;
}

/**
 * returns b-c.
 */
AVRational av_sub_q(AVRational b, AVRational c){
    return av_add_q(b, _AVRational(-c.num, c.den) );
}

/**
 * Converts a double precission floating point number to a AVRational.
 * @param max the maximum allowed numerator and denominator
 */
AVRational av_d2q(double d, int max){
    AVRational a;
#define LOG2  0.69314718055994530941723212145817656807550013436025
    int exponent= FFMAX( (int)(log(fabs(d) + 1e-20)/LOG2), 0);
    int64_t den= INT64_C(1) << (61 - exponent);
    av_reduce(&a.num, &a.den, (int64_t)(d * den + 0.5), den, max);

    return a;
}
