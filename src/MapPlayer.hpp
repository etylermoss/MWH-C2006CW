#ifndef __H_MAP_PLAYER__
#define __H_MAP_PLAYER__

#include <tuple>

#include "InputManager.hpp"
#include "Tick.hpp"

static constexpr float ACCELERATION = 0.03f;
static constexpr float DECELERATION = 0.1f;
static constexpr float VELOCITY_LIMIT = 0.6f;

float bring_towards_zero(float x, float step);

class MapPlayer : public Tick
{
public:
	explicit MapPlayer(InputManager& inputManager, int width, int height, int originX, int originY, int limitLeft, int limitUp, int limitRight, int limitDown) :
			Tick(),
			inputManager(inputManager),
			width(width),
			height(height),
			originX(originX),
			originY(originY),
			limitLeft(limitLeft),
			limitUp(limitUp),
			limitRight(limitRight),
			limitDown(limitDown),
			posX(0),
			posY(0),
			velX(0.0f),
			velY(0.0f)
	{
	}

	~MapPlayer() override = default;

	void reset(int newPosX, int newPosY, float newVelX = 0.0f, float newVelY = 0.0f)
	{
		posX = (float) newPosX;
		posY = (float) newPosY;
		velX = (float) newVelX;
		velY = (float) newVelY;
	}

	void tick() override
	{
		bool pressedUp = inputManager.getActionPressed(Action::MOVE_UP) == Press::DOWN;
		bool pressedDown = inputManager.getActionPressed(Action::MOVE_DOWN) == Press::DOWN;
		bool pressedLeft = inputManager.getActionPressed(Action::MOVE_LEFT) == Press::DOWN;
		bool pressedRight = inputManager.getActionPressed(Action::MOVE_RIGHT) == Press::DOWN;

		if (pressedUp && !pressedDown)
			velY -= (isless(velY, 0) ? ACCELERATION : DECELERATION);
		else if (!pressedUp && pressedDown)
			velY += (isgreaterequal(velY, 0) ? ACCELERATION : DECELERATION);
		else if (!pressedUp && !pressedDown)
			velY = bring_towards_zero(velY, DECELERATION);

		velY = velY < VELOCITY_LIMIT ? (velY > -VELOCITY_LIMIT ? velY : -VELOCITY_LIMIT) : VELOCITY_LIMIT;

		if (pressedLeft && !pressedRight)
			velX -= (isless(velX, 0) ? ACCELERATION : DECELERATION);
		else if (!pressedLeft && pressedRight)
			velX += (isgreaterequal(velX, 0) ? ACCELERATION : DECELERATION);
		else if (!pressedLeft && !pressedRight)
			velX = bring_towards_zero(velX, DECELERATION);

		velX = velX < VELOCITY_LIMIT ? (velX > -VELOCITY_LIMIT ? velX : -VELOCITY_LIMIT) : VELOCITY_LIMIT;

		posX += velX;
		posY += velY;

		if (((int) posX) - (width / 2) < limitLeft) {
			posX = (float) limitLeft + ((float) width / 2); velX = 0.0;
		} else if (((int) posX) + (width / 2) > limitRight) {
			posX = (float) limitRight - ((float) width / 2); velX = 0.0;
		}

		if (((int) posY) - (height / 2) < limitUp) {
			posY = (float) limitUp + ((float) height / 2); velY = 0;
		} else if (((int) posY) + (height / 2) > limitDown) {
			posY = (float) limitDown - ((float) height / 2); velY = 0;
		}
	}

	[[nodiscard]] int getPlayerX() const { return (int) posX; };
	[[nodiscard]] int getPlayerY() const { return (int) posY; };
	[[nodiscard]] int getPlayerW() const { return width; };
	[[nodiscard]] int getPlayerH() const { return height; };
	[[nodiscard]] int getPlayerOriginX() const { return originX; };
	[[nodiscard]] int getPlayerOriginY() const { return originY; };

private:
	InputManager& inputManager;
	int width, height;
	int originX, originY;
	int limitUp, limitRight, limitDown, limitLeft;
	float posX, posY;
	float velX, velY;
};

#endif
