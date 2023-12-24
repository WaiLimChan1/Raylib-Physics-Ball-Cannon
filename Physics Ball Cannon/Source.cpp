#include"raylib.h"
#include<string>
using namespace std;

/*
* Keybinds
* W - Increase Angle
* S - Decrease Angle
* A - Decrease Initial Velocity
* D - Increase Initial Velocity
* F - Shoot
* Left Mouse Button - Shoot
* R - Reset
*/


//Global Constant Variables
//-------------------------------------------------
const float SCREEN_WIDTH = 1500;
const float SCREEN_HEIGHT = 900;
#define PI 3.14159265

const float GRAVITY = 1000;

const float collisionLost = 0.4;

const float LineLengthMaxCap = 1000;
const float LineLengthMinCap = 50;
const float lineVelRatio = 10;
//-------------------------------------------------

class Ball {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float accX;
	float accY;

	float mass;
	Color color;

public:
	Ball();

	float getPosX();
	float getPosY();
	float getVelX();
	float getVelY();
	float getAccX();
	float getAccY();

	void setPosX(float posX);
	void setPosY(float posY);
	void setVelX(float velX);
	void setVelY(float velY);
	void setAccX(float accX);
	void setAccY(float accY);

	void Draw();
	void PrintStats();
	void CalculateAcceleration();
	void CalculateVelocity();
	void CalculatePosition();
	void ReverseDirection();
	void BoundCheck();
};

Ball::Ball()
	: posX(GetScreenWidth() / 2), posY(GetScreenHeight() / 2), velX(0), velY(0), accX(0), accY(0), mass(30), color(RED) {}

float Ball::getPosX() { return posX; }
float Ball::getPosY() { return posY; }
float Ball::getVelX() { return velX; }
float Ball::getVelY() { return velY; }
float Ball::getAccX() { return accX; }
float Ball::getAccY() { return accY; }

void Ball::setPosX(float posX) { this->posX = posX; }
void Ball::setPosY(float posY) { this->posY = posY; }
void Ball::setVelX(float velX) { this->velX = velX; }
void Ball::setVelY(float velY) { this->velY = velY; }
void Ball::setAccX(float accX) { this->accX = accX; }
void Ball::setAccY(float accY) { this->accY = accY; }

void Ball::Draw() { DrawCircle(posX, posY, mass, color); }
void Ball::PrintStats()
{
	char message[50];
	int fontSize = 30;
	int textPosX = GetScreenWidth() / 2;
	int textPosY = fontSize / 2;

	strcpy_s(message, "Position: (");
	strcat_s(message, to_string(static_cast<int>(posX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(GetScreenHeight() - posY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Velocity: (");
	strcat_s(message, to_string(static_cast<int>(velX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(velY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Acceleration: (");
	strcat_s(message, to_string(static_cast<int>(accX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(-1 * accY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);
}
void Ball::CalculateAcceleration()
{
	accX = 0;
	//accY = 0 //No Gravity
	accY = GRAVITY; //Yes Gravity
}
void Ball::CalculateVelocity()
{
	velX += accX * GetFrameTime();
	velY += accY * GetFrameTime();
}
void Ball::CalculatePosition()
{
	posX += velX * GetFrameTime();
	posY += velY * GetFrameTime();
}
void Ball::ReverseDirection()
{
	velX *= -1;
	velY *= -1;
}

//mass is used as radius
void Ball::BoundCheck()
{
	if (posX <= mass)
	{
		posX = mass;
		velX *= -1 * collisionLost;
	}
	else if (posX >= GetScreenWidth() - mass)
	{
		posX = GetScreenWidth() - mass;
		velX *= -1 * collisionLost;
	}

	if (posY <= mass)
	{
		posY = mass;
		velY *= -1 * collisionLost;
	}
	else if (posY >= GetScreenHeight() - mass)
	{
		posY = GetScreenHeight() - mass;
		velY *= -1 * collisionLost;
	}
}

//Global Variables
//-------------------------------------------------
float endAngle;
float lineLength;

Ball ball;
bool Started;
bool Stopped;
//-------------------------------------------------


void DrawCannon()
{
	DrawCircleSector({ 0, static_cast<float>(GetScreenHeight()) / 2 }, 30, 0, endAngle, 100, GREEN);
	DrawLineEx({ 0, static_cast<float>(GetScreenHeight()) / 2 }, { 0 + lineLength * static_cast<float>(sin(endAngle * PI / 180)), static_cast<float>(GetScreenHeight()) / 2 + lineLength * static_cast<float>(cos(endAngle * PI / 180)) }, 3, WHITE);
	DrawLineEx({ 0, static_cast<float>(GetScreenHeight()) / 2 }, { 0, static_cast<float>(GetScreenHeight()) / 2 + lineLength}, 3, WHITE);
	//Circle at the center
	/*float startX = static_cast<float>(GetScreenWidth()) / 2;
	float startY = static_cast<float>(GetScreenHeight()) / 2;
	DrawCircleSector({ startX, startY }, 30, 0, endAngle, 100, GREEN);
	DrawLineEx({ startX, startY }, { startX + lineLength * static_cast<float>(sin(endAngle * PI / 180)), startY + lineLength * static_cast<float>(cos(endAngle * PI / 180)) }, 3, WHITE);*/
}

void PrintCannonStats()
{
	char message[50];
	int fontSize = 20;
	int border = 40;

	strcpy_s(message, "");
	strcat_s(message, to_string(static_cast<int>(endAngle)).c_str());
	strcat_s(message, " Degrees");
	DrawText(message, border, GetScreenHeight() / 2, fontSize, YELLOW);

	strcpy_s(message, "");
	strcat_s(message, to_string(static_cast<int>(lineLength * lineVelRatio)).c_str());
	strcat_s(message, " Pixels/Seconds");
	DrawText(message, border, GetScreenHeight() / 2 + fontSize, fontSize, YELLOW);

}

void PrintManuel()
{
	int fontSize = 30;
	int border = 40;
	int textPosX = GetScreenWidth() / 2;
	int textPosY = GetScreenHeight() / 3;

	DrawText("Press W - Increase Angle", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press S - Decrease Angle", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press D - Increase Initial Velocity", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press A - Decrease Initial Velocity", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press F - Launch Ball", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press Left Mouse Button - Launch Ball", textPosX, textPosY, fontSize, YELLOW);
	textPosY += fontSize + 10;
	DrawText("Press R - Reset", textPosX, textPosY, fontSize, YELLOW);

}

void SetUp()
{
	endAngle = 90;
	lineLength = 100;

	ball = Ball();
	ball.setPosX(0);
	ball.setPosY(GetScreenHeight() / 2);

	Started = false;
	ball.BoundCheck();
}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
	DrawCannon();
	PrintCannonStats();
	PrintManuel();
	if (Started)
		ball.Draw();
	ball.PrintStats();

	EndDrawing();
}

void Input()
{
	if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) //Increase angle
	{
		endAngle += 1;
		if (endAngle > 180)
			endAngle = 180;
	}
	if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) //Decrease angle
	{
		endAngle -= 1;
		if (endAngle < 0)
			endAngle = 0;
	}
	if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) //Increase initial velocity
	{
		lineLength += 1;
		if (lineLength > LineLengthMaxCap)
			lineLength = LineLengthMaxCap;
	}
	if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) //Decrease initial velocity
	{
		lineLength -= 1;
		if (lineLength < LineLengthMinCap)
			lineLength = LineLengthMinCap;
	}
	if (IsKeyPressed(KEY_F) || IsKeyDown(KEY_F) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) //Launch
	{
		if (!Started)
		{
			Started = true;
			ball.setVelX(lineLength * lineVelRatio * static_cast<float>(sin(endAngle * PI / 180)));
			ball.setVelY(lineLength * lineVelRatio * static_cast<float>(cos(endAngle * PI / 180)));
		}
		else
		{
			ball = Ball();
			ball.setPosX(0);
			ball.setPosY(GetScreenHeight() / 2);

			ball.BoundCheck();
			ball.setVelX(lineLength * lineVelRatio * static_cast<float>(sin(endAngle * PI / 180)));
			ball.setVelY(lineLength * lineVelRatio * static_cast<float>(cos(endAngle * PI / 180)));
		}
	}
	if (IsKeyPressed(KEY_R) || IsKeyDown(KEY_R)) //Reset
	{
		SetUp();
	}
}

void Logic()
{
	if (Started)
	{
		ball.CalculateAcceleration();
		ball.CalculateVelocity();
		ball.CalculatePosition();
		ball.BoundCheck();
	}
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Ball Cannon");
	SetWindowState(FLAG_VSYNC_HINT);

	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		Input();
		Logic();
	}
	CloseWindow();
	return 0;
}