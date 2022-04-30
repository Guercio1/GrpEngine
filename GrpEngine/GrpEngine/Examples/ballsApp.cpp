
#include "..\GrpEngine\VSbridge.h"
#include "..\Examples\BALLSAPP.h"



void CirclePhysics::AddBall(float x, float y, float r)
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


	egtkFillCircle(
		pBall->px,
		pBall->py,
		pBall->radius,
		RGB565_RED);

/*
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
*/
}

void CirclePhysics::DrawLineSegment(sLineSegment *pLineSeg)
{
	egtkFillCircle(pLineSeg->sx, pLineSeg->sy, pLineSeg->radius, RGB565_GREY_50);
	egtkFillCircle(pLineSeg->ex, pLineSeg->ey, pLineSeg->radius, RGB565_GREY_50);

	float nx = -(pLineSeg->ey - pLineSeg->sy);
	float ny = (pLineSeg->ex - pLineSeg->sx);
	float d = sqrt(nx * nx + ny * ny);
	nx /= d;
	ny /= d;

	egtkDrawLine((pLineSeg->sx + nx * pLineSeg->radius), (pLineSeg->sy + ny * pLineSeg->radius), (pLineSeg->ex + nx * pLineSeg->radius), (pLineSeg->ey + ny * pLineSeg->radius),RGB565_GREY_50);
	egtkDrawLine((pLineSeg->sx - nx * pLineSeg->radius), (pLineSeg->sy - ny * pLineSeg->radius), (pLineSeg->ex - nx * pLineSeg->radius), (pLineSeg->ey - ny * pLineSeg->radius),RGB565_GREY_50);
}

bool CirclePhysics::OnUserCreate(void)
{
	float fDefaultRad = 20.0f;

	egtkInitScreen(1000, 800, RGB565_WHITE, RGB565_BLACK);

	// Add 10 Random Balls
	for (int i = 0; i < 300; i++)
		AddBall(rand() % egtkGetScreenW(), rand() % egtkGetScreenH(), 8/*rand() % 25 + 10*/);

	float fLineRadius = 10.0f;

	vecLines.push_back({ 20.0f, 20.0f, 100.0f, 20.0f, fLineRadius });
	vecLines.push_back({ 100.0f, 100.0f, 200.0f, 100.0f, fLineRadius });
	vecLines.push_back({ 300.0f, 200.0f, 400.0f, 200.0f, fLineRadius });
	//vecLines.push_back({ 76.0f, 68.0f, 132.0f, 68.0f, fLineRadius });
	//vecLines.push_back({ 4.0f, 12.0f, 4.0f, 60.0f, fLineRadius });
	//vecLines.push_back({ 140.0f, 12.0f, 140.0f, 60.0f, fLineRadius });

	return true;
}

bool CirclePhysics::OnUserUpdate(float fElapsedTime)
{
	vector<pair<sBall*, sBall*>> vecCollidingPairs;
	vector<sBall*> vecFakeBalls;

	if (fElapsedTime > 1000)
		fElapsedTime = 0;

	auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
	};


	// Multiple simulation updates with small time steps permit more accurate physics
	// and realistic results at the expense of CPU time of course
	int nSimulationUpdates = 4;



	// Multiple collision trees require more steps to resolve. Normally we would
	// continue simulation until the object has no simulation time left for this
	// epoch, however this is risky as the system may never find stability, so we
	// can clamp it here
	int nMaxSimulationSteps = 15;

	// Break up the frame elapsed time into smaller deltas for each simulation update
	float fSimElapsedTime = fElapsedTime / (float)nSimulationUpdates;

	// Main simulation loop
	for (int i = 0; i < nSimulationUpdates; i++)
	{
		// Set all balls time to maximum for this epoch
		for (auto& ball : vecBalls)
			ball.fSimTimeRemaining = fSimElapsedTime;

		// Erode simulation time on a per objec tbasis, depending upon what happens
		// to it during its journey through this epoch
		for (int j = 0; j < nMaxSimulationSteps; j++)
		{

			// Update Ball Positions
			for (auto& ball : vecBalls)
			{

				if (ball.fSimTimeRemaining > 0.0f)
				{

					ball.ox = ball.px;								// Store original position this epoch
					ball.oy = ball.py;


					// Add Drag to emulate rolling friction
					ball.ax = -ball.vx * 0.8f;
					ball.ay = -ball.vy * 0.8f + 100.0f;

					// Update ball physics
					ball.vx += ball.ax * ball.fSimTimeRemaining;

					ball.vy += ball.ay * ball.fSimTimeRemaining;

					ball.px += ball.vx * ball.fSimTimeRemaining;
					ball.py += ball.vy * ball.fSimTimeRemaining;

					// Wrap the balls around screen
					//if (ball.px < 0)
					while(ball.px < 0)
						//ball.px = egtkGetScreenW();
						ball.px += (float)egtkGetScreenW();

					while(ball.px >= egtkGetScreenW())
					//if (ball.px >= egtkGetScreenW()) 
						ball.px -= (float)egtkGetScreenW();
						//ball.px = 0;

					while (ball.py < 0) 
				    //if (ball.py < 0)
						//ball.py = (float)egtkGetScreenH();
						ball.py += (float)egtkGetScreenH();

					while(ball.py >= egtkGetScreenH())
					//if (ball.py >= egtkGetScreenH())
						//ball.py = 0;
						ball.py -= (float)egtkGetScreenH();

					// Clamp velocity near zero
					float velocity = fabs(ball.vx * ball.vx + ball.vy * ball.vy);
					if (velocity < 0.01f)
					{
						ball.vx = 0;
						ball.vy = 0;
					}
				}
			}

			// Static collisions, i.e. overlap
			for (auto& ball : vecBalls)
			{

				// Against Edges
				for (auto& edge : vecLines)
				{
					// Check that line formed by velocity vector, intersects with line segment
					float fLineX1 = edge.ex - edge.sx;
					float fLineY1 = edge.ey - edge.sy;

					float fLineX2 = ball.px - edge.sx;
					float fLineY2 = ball.py - edge.sy;

					float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

					// This is nifty - It uses the DP of the line segment vs the line to the object, to work out
					// how much of the segment is in the "shadow" of the object vector. The min and max clamp
					// this to lie between 0 and the line segment length, which is then normalised. We can
					// use this to calculate the closest point on the line segment
					float t = MAX(0, MIN(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;

					// Which we do here
					float fClosestPointX = edge.sx + t * fLineX1;
					float fClosestPointY = edge.sy + t * fLineY1;

					// And once we know the closest point, we can check if the ball has collided with the segment in the
					// same way we check if two balls have collided
					float fDistance = sqrtf((ball.px - fClosestPointX) * (ball.px - fClosestPointX) + (ball.py - fClosestPointY) * (ball.py - fClosestPointY));

					if (fDistance <= (ball.radius + edge.radius))
					{
						// Collision has occurred - treat collision point as a ball that cannot move. To make this
						// compatible with the dynamic resolution code below, we add a fake ball with an infinite mass
						// so it behaves like a solid object when the momentum calculations are performed
						sBall* fakeball = new sBall();
						fakeball->radius = edge.radius;
						fakeball->mass = ball.mass * 0.8f;
						fakeball->px = fClosestPointX;
						fakeball->py = fClosestPointY;
						fakeball->vx = -ball.vx;	// We will use these later to allow the lines to impart energy into ball
						fakeball->vy = -ball.vy;	// if the lines are moving, i.e. like pinball flippers

						// Store Fake Ball
						vecFakeBalls.push_back(fakeball);

						// Add collision to vector of collisions for dynamic resolution
						vecCollidingPairs.push_back({ &ball, fakeball });

						// Calculate displacement required
						float fOverlap = 1.0f * (fDistance - ball.radius - fakeball->radius);

						// Displace Current Ball away from collision
						ball.px -= fOverlap * (ball.px - fakeball->px) / fDistance;
						ball.py -= fOverlap * (ball.py - fakeball->py) / fDistance;
					}
				}

				//Againt other balls
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

				// Time displacement - we knew the velocity of the ball, so we can estimate the distance it should have covered
				// however due to collisions it could not do the full distance, so we look at the actual distance to the collision
				// point and calculate how much time that journey would have taken using the speed of the object. Therefore
				// we can now work out how much time remains in that timestep.
				float fIntendedSpeed = sqrtf(ball.vx * ball.vx + ball.vy * ball.vy);
				float fIntendedDistance = fIntendedSpeed * ball.fSimTimeRemaining;
				float fActualDistance = sqrtf((ball.px - ball.ox) * (ball.px - ball.ox) + (ball.py - ball.oy) * (ball.py - ball.oy));
				float fActualTime = fActualDistance / fIntendedSpeed;

				// After static resolution, there may be some time still left for this epoch, so allow simulation to continue
				ball.fSimTimeRemaining = ball.fSimTimeRemaining - fActualTime;

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



			// Remove collisions
			vecCollidingPairs.clear();


			// Remove fake balls
			for (auto& b : vecFakeBalls) delete b;
				vecFakeBalls.clear();
		}
	}
	egtkClearScreen();



	// Draw Lines
	for (auto line : vecLines)
		DrawLineSegment(&line);


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
	{
		pSelectedBall = NULL;
		pSelectedLine = nullptr;
	}
	else if ((MouseAct == MOUSE_LFT_PSH) || (MouseAct == MOUSE_RGH_PSH))
	{
		//Check for selected ball
		pSelectedBall = NULL;
		for (auto& ball : vecBalls)
		{
			if (IsPointInCircle(ball.px, ball.py, ball.radius, xpos, ypos))
			{
				pSelectedBall = &ball;
				break;
			}
		}

		// Check for selected line segment end
		pSelectedLine = nullptr;
		for (auto& line : vecLines)
		{
			if (IsPointInCircle(line.sx, line.sy, line.radius, xpos, ypos))
			{
				pSelectedLine = &line;
				bSelectedLineStart = true;
				break;
			}

			if (IsPointInCircle(line.ex, line.ey, line.radius, xpos, ypos))
			{
				pSelectedLine = &line;
				bSelectedLineStart = false;
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

		if (pSelectedLine != nullptr)
		{
			if (bSelectedLineStart)
			{
				pSelectedLine->sx = xpos;
				pSelectedLine->sy = ypos;
			}
			else
			{
				pSelectedLine->ex = xpos;
				pSelectedLine->ey = ypos;
			}
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
