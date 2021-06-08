#pragma once
#include "EInstance.h"

struct Shape
{
	
	std::vector<Vec2d> points;
	std::vector<Vec2d> worldPnt;

	void formHedron(float r, int cnt)
	{
		points.clear();
		worldPnt.clear();

		
		float ang = 2 * EPI / cnt;

		for (int cv = 0; cv < cnt; cv++)
		{
			AddPnt(
				Vec2d(
					cos(ang * -cv), 
					sin(ang * -cv))
					*r 
				  );
		}
	}

	void formRect(float w, float h,bool centered)
	{
		points.clear();
		worldPnt.clear();
		
		if (centered)
		{
			AddPnt(Vec2d(-w / 2, -h / 2));
			AddPnt(Vec2d(-w / 2, h / 2));
			AddPnt(Vec2d(w / 2, h / 2));
			AddPnt(Vec2d(w / 2, -h / 2));
			
			
		}
		else
		{
			AddPnt(Vec2d(0, 0));
			AddPnt(Vec2d(w, 0));
			AddPnt(Vec2d(w, h));
			AddPnt(Vec2d(0, h));
		}


	}

	void AddPnt(Vec2d pnt)
	{
		points.push_back(pnt);
		worldPnt.push_back(pnt);
	}

	void Draw(Vec2d p, DirectGraphics* gfx, EColor clr = EColor(255, 255, 255))
	{
		for (int pn = 1; pn < points.size(); pn++)
		{
			gfx->DrawLine(
				p.x+points[pn-1].x, p.y + points[pn - 1].y,
				p.x + points[pn].x, p.y + points[pn].y, clr);
		}
		gfx->DrawLine(
			p.x + points[points.size()-1].x, p.y + points[points.size()-1].y,
			p.x + points[0].x, p.y + points[0].y, clr);


	}
	void Draw(Vec2d p, float angle,DirectGraphics* gfx, EColor clr = EColor(255, 255, 255))
	{
		Mat22 rot = Mat22::RotationMat(angle);

		Vec2d prevRot = rot * points[0];

		worldPnt[0] = prevRot + p;

		Vec2d firstRot = prevRot;

		Vec2d newRot;

		for (int pn = 1; pn < points.size(); pn++)
		{
			newRot = rot * points[pn];

			gfx->DrawLine(
				p.x + prevRot.x, p.y + prevRot.y,
				p.x + newRot.x,  p.y + newRot.y, clr);

			worldPnt[pn] = newRot + p;

			prevRot = newRot;
		}
		gfx->DrawLine(
			p.x + newRot.x,   p.y + newRot.y,
			p.x + firstRot.x, p.y + firstRot.y, clr);

		//gfx->DrawCircle(p.x,p.y,10,EColor(0,255,50));



	}
};


bool SATOverlap(Shape& shapeA, Shape& shapeB, DirectGraphics* gfx, bool show=false)
{
	
	if (show)
	{
		for (int vn = 0; vn < shapeA.points.size(); vn++)
		{
			int next = (vn + 1) % shapeA.worldPnt.size();
			Vec2d edge = shapeA.worldPnt[next] - shapeA.worldPnt[vn];
			Vec2d normal = edge.GetNormal().Normalized();
			if (show)
				gfx->DrawVector(shapeA.worldPnt[vn] + edge / 2, normal * 30, EColor(255, 100, 0));
		}
		for (int vn = 0; vn < shapeB.points.size(); vn++)
		{
			int next = (vn + 1) % shapeB.worldPnt.size();
			Vec2d edge = shapeB.worldPnt[next] - shapeB.worldPnt[vn];
			Vec2d normal = edge.GetNormal().Normalized();
			if (show)
				gfx->DrawVector(shapeB.worldPnt[vn] + edge / 2, normal * 30, EColor(255, 100, 0));
		}

	}

	for (int vn = 0; vn < shapeA.points.size(); vn++)
	{
		int next = (vn + 1) % shapeA.worldPnt.size();
		Vec2d edge = shapeA.worldPnt[next] - shapeA.worldPnt[vn];
		Vec2d normal = edge.GetNormal();

		float minV1 = INFINITY; float maxV1 = -INFINITY;
		float minV2 = INFINITY; float maxV2 = -INFINITY;

		for (int vp = 0; vp < shapeA.points.size(); vp++)
		{
			float proj = DotProduct(shapeA.worldPnt[vp], normal);

			maxV1 = std::fmax(maxV1, proj);
			minV1 = std::fmin(minV1, proj);

		}


		for (int vp = 0; vp < shapeB.points.size(); vp++)
		{
			float proj = DotProduct(shapeB.worldPnt[vp], normal);
			maxV2 = std::fmax(maxV2, proj);
			minV2 = std::fmin(minV2, proj);
		}

		if (!(maxV2 >= minV1 && maxV1 >= minV2))
			return false;

	}

	for (int vn = 0; vn < shapeB.points.size(); vn++)
	{
		int next = (vn + 1) % shapeB.worldPnt.size();
		Vec2d edge = shapeB.worldPnt[next] - shapeB.worldPnt[vn];
		Vec2d normal = edge.GetNormal();
		
		float minV1 = INFINITY; float maxV1 = -INFINITY;
		float minV2 = INFINITY; float maxV2 = -INFINITY;

		for (int vp = 0; vp < shapeA.points.size(); vp++)
		{
			float proj = DotProduct(shapeA.worldPnt[vp] , normal);
			maxV1 = std::fmax(maxV1, proj);
			minV1 = std::fmin(minV1, proj);
		}

		for (int vp = 0; vp < shapeB.points.size(); vp++)
		{
			float proj = DotProduct(shapeB.worldPnt[vp], normal);
			maxV2 = std::fmax(maxV2, proj);
			minV2 = std::fmin(minV2, proj);
		}

		if (!(maxV2 >= minV1 && maxV1 >= minV2))
			return false;

	}
	
	return true;
}


Vec2d GetSupportPoint(Shape s, Vec2d dir)
{

	Vec2d furthest = s.worldPnt[0];
	float dp = s.worldPnt[0] * dir;

	for (int sp = 1; sp < s.points.size(); sp++)
	{
		float ndp = s.worldPnt[sp] * dir;
		if (ndp > dp)
		{
			dp = ndp;
			furthest = s.worldPnt[sp];
		}
	}

	return furthest;

}





struct Body
{
	Shape* shape;
	Vec2d Position;
	Vec2d Velocity;

	void Draw(DirectGraphics* gfx, EColor clr = EColor(255,255,255))
	{
		shape->Draw(Position, angle, gfx, clr);
	}

	void SetMass(float m)
	{
		Mass = m;
		IMass = 1.0f / m;
	}

	float Mass;
	float IMass;
	float Inertia;
	float IInertia;

	float Torque;

	float Restitution;

	float angle;
};

Vec2d CenterImpulse(float m1, float v1, float m2, float v2)
{
	return Vec2d(
		(v1 * (m1 - m2) + 2.0f * m2 * v2) / (m1 + m2),//v1'
		(v2 * (m2 - m1) + 2.0f * m1 * v1) / (m1 + m2)// v2'
	);
}

void CircleImpulseResolution(Body* body1, Body* body2)
{
	Vec2d cToC = (body2->Position - body1->Position).Normalized();
	Vec2d tangent = cToC.GetNormal();


	float norVel1 = DotProduct(body1->Velocity , cToC);
	float norVel2 = DotProduct(body2->Velocity , cToC);

	Vec2d norVel = CenterImpulse(body1->Mass, norVel1, body2->Mass, norVel2);

	//Vec2d newVel1 = cToC * norVel.x;
	//Vec2d newVel2 = cToC * norVel.y;

	float tanVel1 = DotProduct(body1->Velocity , tangent);
	float tanVel2 = DotProduct(body2->Velocity , tangent);

	
	Vec2d newVel2 = tangent * tanVel2;

	body1->Velocity = tangent * tanVel1 + cToC * norVel.x;
	body2->Velocity = tangent * tanVel2 + cToC * norVel.y;

}

float FindLeastPenetration(Shape sA, Shape sB, int* ind)
{
	float bestDist = -INFINITY;
	int bestInd;

	for (int cv = 0; cv < sA.worldPnt.size(); cv++)
	{
		Vec2d norm = (sA.worldPnt[(cv + 1) % sA.worldPnt.size()] - sA.worldPnt[cv]).GetNormal().Normalized();
		Vec2d supp = GetSupportPoint(sB, -norm);
		float dist = norm * (supp - sA.worldPnt[cv]);
		if (dist > bestDist)
		{
			bestInd = cv;
			bestDist = dist;
		}
	}

	*ind = bestInd;

	return bestDist;
}

void ImpulseNormResolution(Body* b1, Body* b2, Vec2d norm, float lever1, float lever2)
{
	Vec2d relV = b2->Velocity - b1->Velocity;

	float proj = relV * norm;

	if (proj > 0)
		return;

	float e = std::fmin(b1->Restitution, b2->Restitution);

	float j = (-(1 + e) * proj)/(b1->IMass+b2->IMass);

	Vec2d impulse = norm * j;

	b1->Velocity -= (impulse * b1->IMass)*lever1*sqrtf(fabsf(b2->Torque));
	b2->Velocity += (impulse * b2->IMass)*lever2*sqrtf(fabsf(b1->Torque));

}

void ImpulseResolution(Body* b1, Body* b2)
{
	int faceInd;
	int faceInd2;
	bool secondB = false;
	float dst = FindLeastPenetration(*b1->shape, *b2->shape, &faceInd);

	float dst2 = FindLeastPenetration(*b2->shape, *b1->shape, &faceInd2);

	if (dst2 > dst)
	{
		dst = dst2;
		secondB = true;
	}
		

	



	Vec2d face;
	Vec2d norm;
	if (!secondB)
	{
		face = b1->shape->worldPnt[(faceInd + 1) % b1->shape->worldPnt.size()] - b1->shape->worldPnt[faceInd];
		norm = face.GetNormal().Normalized();


		Vec2d penNorm = norm.GetNormal();
		Vec2d supp = GetSupportPoint(*b2->shape, -norm);
		float lever = penNorm * (supp - b1->Position);
		b1->Torque-= sqrtf(fabsf(lever * b2->Mass * b1->IMass)) * esign(lever) * esign(dst);

		ImpulseNormResolution(b1, b2, norm,lever,0);
	}
	else
	{
		face = b2->shape->worldPnt[(faceInd2 + 1) % b2->shape->worldPnt.size()] - b2->shape->worldPnt[faceInd2];
		norm = face.GetNormal().Normalized();

		Vec2d penNorm = norm.GetNormal();
		Vec2d supp = GetSupportPoint(*b1->shape, -norm);
		float lever = penNorm * (supp - b2->Position);
		b2->Torque -= sqrtf(fabsf(lever * b1->Mass * b2->IMass)) * esign(lever) * esign(dst);

		ImpulseNormResolution(b2, b1, norm,0,lever);

	}



}

EColor clrs[10];

void DoRigid(Body* b1, Body* b2, DirectGraphics* dxg = nullptr)
{
	bool info = false;
	if (dxg != nullptr)
		info = true;
	float BpenetDist = -INFINITY;

	
	

	for (int pd = 0; pd < b1->shape->worldPnt.size(); pd++)
	{
		Vec2d face = b1->shape->worldPnt[(pd+1)%b1->shape->worldPnt.size()] - b1->shape->worldPnt[pd];
		Vec2d faceNorm = face.GetNormal().Normalized();
		//if (info)
		//	dxg->DrawVector(b1->shape->worldPnt[pd], faceNorm, EColor(255, 255, 0));

		Vec2d supp = GetSupportPoint(*b2->shape, faceNorm);

		if (info)
			dxg->DrawCircle(supp, 10, clrs[pd % 10]);

		float penetDist = (supp - b1->shape->worldPnt[pd]) * faceNorm;

		if (info)
			dxg->DrawVector(b1->shape->worldPnt[pd]+face/2, faceNorm*penetDist, clrs[pd%10]);


	}


}

void ApllyVelocities(Body* b, float dt)
{
	b->angle += b->Torque * dt;
	b->Position += b->Velocity * dt;
}







