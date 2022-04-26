
#include "..\GrpEngine\VSbridge.h"
#include "..\Examples\balls2.h"


CirclePhysics::CirclePhysics() 
{

}
void CirclePhysics::AddBall(float x, float y, float r )
{
	sBall b;
	b.px = x; b.py = y;
	b.vx = 0; b.vy = 0;
	b.ax = 0; b.ay = 0;
	b.radius = r;
	b.mass = r * 10.0f;

	b.id = vecBalls.size();
	vecBalls.emplace_back(b);
}

void CirclePhysics::DrawBall(sBall* pBall)
{
	float VelAng = atan2f(pBall->vy, pBall->vx);

	egtkDrawCircle(
		pBall->px,
		pBall->py,
		pBall->radius,
		RGB565_WHITE);

	egtkDrawLine(
		pBall->px,
		pBall->py,
		pBall->px + pBall->radius * cos(VelAng),
		pBall->py + pBall->radius * sin(VelAng),
		RGB565_WHITE);
}

bool CirclePhysics::OnUserCreate(void)
{
	float fDefaultRad = 20.0f;

	egtkInitScreen(600, 400, RGB565_WHITE, RGB565_BLACK);

	// Add 10 Random Balls
	for (int i = 0; i < 10; i++)
		AddBall(rand() % egtkGetScreenW(), rand() % egtkGetScreenH(), rand() % 25 + 10);

	return true;
}

bool CirclePhysics::OnUserUpdate(float fElapsedTime)
{
	vector<pair<sBall*, sBall*>> vecCollidingPairs;

	auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	};

	// Update Ball Positions
	for (auto& ball : vecBalls)
	{
		// Add Drag to emulate rolling friction
		ball.ax = -ball.vx * 0.8f;
		ball.ay = -ball.vy * 0.8f;

		// Update ball physics
		ball.vx += ball.ax * fElapsedTime;
		ball.vy += ball.ay * fElapsedTime;
		ball.px += ball.vx * fElapsedTime;
		ball.py += ball.vy * fElapsedTime;

		// Wrap the balls around screen
		if (ball.px < 0) ball.px += (float)egtkGetScreenW();
		if (ball.px >= egtkGetScreenW()) ball.px -= (float)egtkGetScreenW();
		if (ball.py < 0) ball.py += (float)egtkGetScreenH();
		if (ball.py >= egtkGetScreenH()) ball.py -= (float)egtkGetScreenH();

		// Clamp velocity near zero
		if (fabs(ball.vx * ball.vx + ball.vy * ball.vy) < 0.01f)
		{
			ball.vx = 0;
			ball.vy = 0;
		}
	}


	// Static collisions, i.e. overlap
	for (auto& ball : vecBalls)
	{
		for (auto& target : vecBalls)
		{
			if (ball.id != target.id)
			{
				if (DoCirclesOverlap(ball.px, ball.py, ball.radius, target.px, target.py, target.radius))
				{
					// Collision has occured
					vecCollidingPairs.push_back({ &ball, &target });

					// Distance between ball centers
					float fDistance = sqrtf((ball.px - target.px) * (ball.px - target.px) + (ball.py - target.py) * (ball.py - target.py));

					// Calculate displacement required
					float fOverlap = 0.5f * (fDistance - ball.radius - target.radius);

					// Displace Current Ball away from collision
					ball.px -= fOverlap * (ball.px - target.px) / fDistance;
					ball.py -= fOverlap * (ball.py - target.py) / fDistance;

					// Displace Target Ball away from collision
					target.px += fOverlap * (ball.px - target.px) / fDistance;
					target.py += fOverlap * (ball.py - target.py) / fDistance;
				}
			}
		}
	}


	// Now work out dynamic collisions
	for (auto c : vecCollidingPairs)
	{
		sBall* b1 = c.first;
		sBall* b2 = c.second;

		// Distance between balls
		float fDistance = sqrtf((b1->px - b2->px) * (b1->px - b2->px) + (b1->py - b2->py) * (b1->py - b2->py));

		// Normal
		float nx = (b2->px - b1->px) / fDistance;
		float ny = (b2->py - b1->py) / fDistance;


		// Tangent
		float tx = -ny;
		float ty = nx;

		// Dot Product Tangent
		float dpTan1 = b1->vx * tx + b1->vy * ty;
		float dpTan2 = b2->vx * tx + b2->vy * ty;

		// Dot Product Normal
		float dpNorm1 = b1->vx * nx + b1->vy * ny;
		float dpNorm2 = b2->vx * nx + b2->vy * ny;

		// Conservation of momentum in 1D
		float m1 = (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
		float m2 = (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

		// Update ball velocities
		b1->vx = tx * dpTan1 + nx * m1;
		b1->vy = ty * dpTan1 + ny * m1;
		b2->vx = tx * dpTan2 + nx * m2;
		b2->vy = ty * dpTan2 + ny * m2;

	}

	egtkClearScreen();

	// Draw Balls
	for (auto ball : vecBalls)
		DrawBall(&ball);

	// Draw static collisions
	for (auto c : vecCollidingPairs)
		egtkDrawLine(c.first->px, c.first->py, c.second->px, c.second->py, RGB565_RED);

	// Draw Cue
	if (pSelectedBall != nullptr)
		egtkDrawLine(pSelectedBall->px, pSelectedBall->py, MouseX, MouseY, RGB565_BLUE);


	return true;

}

void CirclePhysics::OnUserMouse(int xpos, int ypos, MOUSE_TYPE MouseAct)
{
	auto IsPointInCircle = [](float x1, float y1, float r1, float px, float py)
	{
		return fabs((x1 - px) * (x1 - px) + (y1 - py) * (y1 - py)) < (r1 * r1);
	};

	MouseX = xpos;
	MouseY = ypos;

	if (MouseAct == MOUSE_LFT_REL)
		pSelectedBall = NULL;
	else if ((MouseAct == MOUSE_LFT_PSH) || (MouseAct == MOUSE_RGH_PSH))
	{
		pSelectedBall = NULL;

		for (auto& ball : vecBalls)
		{
			if (IsPointInCircle(ball.px, ball.py, ball.radius, xpos, ypos))
			{
				pSelectedBall = &ball;
				break;
			}
		}
	}
	else if (MouseAct == MOUSE_MOV_L_PSH)
	{
		if (pSelectedBall != nullptr)
		{
			pSelectedBall->px = xpos;
			pSelectedBall->py = ypos;
		}
	}
	else if (MouseAct == MOUSE_RGH_REL)
	{

		if (pSelectedBall != nullptr)
		{
			// Apply velocity
			pSelectedBall->vx = 5.0f * ((pSelectedBall->px) - (float)xpos);
			pSelectedBall->vy = 5.0f * ((pSelectedBall->py) - (float)ypos);
		}

		pSelectedBall = nullptr;

	}
}

void CirclePhysics::OnUserKeyboard(int code, bool push)
{



}
