//
//  Aabb.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 02.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "utils/Models/Aabb.h"

AABB AABB::UnitBox(Vector(-1,-1,-1), Vector(1,1,1));

AABB::AABB()
{
    
}
AABB::AABB(const Vector& min, const Vector& max) : Min(min), Max(max)
{
    
}
AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : Min(minX, minY, minZ), Max(maxX, maxY, maxZ)
{
    
}

Vector AABB::size() const
{
    return Max-Min;
}

AABB AABB::transform(const Matrix& m) const
{
	Vector c[8];
	corners(c);
	for (int i = 0; i < 8; ++i)
		c[i] = m * c[i];
	AABB r;
	r.fromPoints(c, 8);
	return r;
}

AABB AABB::merge(const AABB& a, const AABB& b) const
{
	AABB r;

	r.Min.X = a.Min.X < b.Min.X ? a.Min.X : b.Min.X;
	r.Min.Y = a.Min.Y < b.Min.Y ? a.Min.Y : b.Min.Y;
	r.Min.Z = a.Min.Z < b.Min.Z ? a.Min.Z : b.Min.Z;

	r.Max.X = a.Max.X > b.Max.X ? a.Max.X : b.Max.X;
	r.Max.Y = a.Max.Y > b.Max.Y ? a.Max.Y : b.Max.Y;
	r.Max.Z = a.Max.Z > b.Max.Z ? a.Max.Z : b.Max.Z;

	return r;
}

AABB& AABB::merge(const AABB& a)
{
	Min.X = a.Min.X < Min.X ? a.Min.X : Min.X;
	Min.Y = a.Min.Y < Min.Y ? a.Min.Y : Min.Y;
	Min.Z = a.Min.Z < Min.Z ? a.Min.Z : Min.Z;

	Max.X = a.Max.X > Max.X ? a.Max.X : Max.X;
	Max.Y = a.Max.Y > Max.Y ? a.Max.Y : Max.Y;
	Max.Z = a.Max.Z > Max.Z ? a.Max.Z : Max.Z;

	return *this;
}

void AABB::corners(Vector c[8]) const
{
	c[0].X = Min.X; c[0].Y = Min.Y; c[0].Z = Min.Z;
	c[1].X = Max.X; c[1].Y = Min.Y; c[1].Z = Min.Z;
	c[2].X = Max.X; c[2].Y = Max.Y; c[2].Z = Min.Z;
	c[3].X = Max.X; c[3].Y = Min.Y; c[3].Z = Min.Z;

	c[4].X = Min.X; c[4].Y = Min.Y; c[4].Z = Max.Z;
	c[5].X = Max.X; c[5].Y = Min.Y; c[5].Z = Max.Z;
	c[6].X = Max.X; c[6].Y = Max.Y; c[6].Z = Max.Z;
	c[7].X = Max.X; c[7].Y = Min.Y; c[7].Z = Max.Z;
}

void AABB::fromPoints(const Vector* Points, unsigned int PointCount)
{
	Max = Vector(-1e20f, -1e20f, -1e20f);
	Min = Vector(1e20f, 1e20f, 1e20f);

	for (unsigned int i = 0; i < PointCount; ++i)
	{
		if (Min.X > Points[i].X) Min.X = Points[i].X;
		if (Min.Y > Points[i].Y) Min.Y = Points[i].Y;
		if (Min.Z > Points[i].Z) Min.Z = Points[i].Z;
		if (Max.X < Points[i].X) Max.X = Points[i].X;
		if (Max.Y < Points[i].Y) Max.Y = Points[i].Y;
		if (Max.Z < Points[i].Z) Max.Z = Points[i].Z;
	}

}

Vector AABB::center() const
{
	return (Max + Min)*0.5f;
}

const AABB& AABB::unitBox()
{
    return UnitBox;
}

