#ifndef __H_LEVEL_PLAYER__
#define __H_LEVEL_PLAYER__

#include <tuple>
#include <vector>

#include "InputManager.hpp"
#include "Tick.hpp"
#include "Level.hpp"
#include "lib/UtilCollisionDetection.h"

static constexpr float LEVEL_ACCELERATION = 0.07f;
static constexpr float LEVEL_DECELERATION = 0.1f;
static constexpr float LEVEL_VELOCITY_LIMIT = 1.8f;
static constexpr float LEVEL_GRAVITY_ACCELERATION = 0.2f;



class LevelPlayer : public Tick
{
public:
	explicit LevelPlayer(InputManager& inputManager) :
			Tick(),
			inputManager(inputManager),
			width(0),
			height(0),
			originX(0),
			originY(0),
			limitLeft(0),
			limitUp(0),
			limitRight(0),
			limitDown(0),
			posX(0),
			posY(0),
			velX(0.0f),
			velY(0.0f)
	{
	}

	~LevelPlayer() override = default;

	void setLevel(Level* newLevel) {
		level = newLevel;
		blocks.clear();
		auto blocksSize = level->getSize();
		for (int x = std::get<0>(blocksSize) - 1; x >= 0; --x) {
			for (int y = std::get<1>(blocksSize) - 1; y >= 0; --y) {
				if (level->getBlockAt(x, y) != Block::NONE)
					blocks.push_back(std::make_tuple<int, int>(x * TILE_SIZE, y * TILE_SIZE));
			}
		}
	}

	void reset(int newPosX, int newPosY, float newVelX, float newVelY, int newLimitLeft, int newLimitUp, int newLimitRight, int newLimitDown, int newWidth, int newHeight, int newOriginX, int newOriginY)
	{
		posX = (float) newPosX;
		posY = (float) newPosY;
		velX = newVelX;
		velY = newVelY;
		limitLeft = newLimitLeft;
		limitUp = newLimitUp;
		limitRight = newLimitRight;
		limitDown = newLimitDown;
		width = newWidth;
		height = newHeight;
		originX = newOriginX;
		originY = newOriginY;
	}

	void tick() override
	{
		bool pressedUp = inputManager.getActionPressed(Action::MOVE_UP) == Press::DOWN;
		bool pressedDown = inputManager.getActionPressed(Action::MOVE_DOWN) == Press::DOWN;
		bool pressedLeft = inputManager.getActionPressed(Action::MOVE_LEFT) == Press::DOWN;
		bool pressedRight = inputManager.getActionPressed(Action::MOVE_RIGHT) == Press::DOWN;

		bool onGround = false;
		for (const auto &item : blocks) {
			int _x = std::get<0>(item);
			int _y = std::get<1>(item);
			onGround = CollisionDetection::checkRectangles(_x, _x + TILE_SIZE, _y, _y + TILE_SIZE, (int) posX, (int) posX + TILE_SIZE, ((int) posY) + 0, ((int) posY) + 32);
			if (onGround)
				break;
		}

		if (onGround) {
			velY = velY > 0 ? 0 : velY;
		} else {
			velY += LEVEL_GRAVITY_ACCELERATION;
		}

//		if (pressedUp && !pressedDown)
//			velY -= (isless(velY, 0) ? LEVEL_ACCELERATION : LEVEL_DECELERATION);
		 if (!pressedUp && pressedDown)
			velY += (isgreaterequal(velY, 0) ? LEVEL_ACCELERATION : LEVEL_DECELERATION);
		else if (!pressedUp && !pressedDown)
			velY = bring_towards_zero(velY, LEVEL_DECELERATION);

		velY = velY < LEVEL_VELOCITY_LIMIT ? (velY > -LEVEL_VELOCITY_LIMIT ? velY : -LEVEL_VELOCITY_LIMIT) : LEVEL_VELOCITY_LIMIT;

		if (pressedLeft && !pressedRight)
			velX -= (isless(velX, 0) ? LEVEL_ACCELERATION : LEVEL_DECELERATION);
		else if (!pressedLeft && pressedRight)
			velX += (isgreaterequal(velX, 0) ? LEVEL_ACCELERATION : LEVEL_DECELERATION);
		else if (!pressedLeft && !pressedRight)
			velX = bring_towards_zero(velX, LEVEL_DECELERATION);

		velX = velX < LEVEL_VELOCITY_LIMIT ? (velX > -LEVEL_VELOCITY_LIMIT ? velX : -LEVEL_VELOCITY_LIMIT) : LEVEL_VELOCITY_LIMIT;

		if (onGround && pressedUp && !pressedDown) {
			velY -= 5.0f; posX -= 1.0f;
		}

		posX += velX;
		posY += velY;

		if (((int) posX) < limitLeft) {
			posX = (float) limitLeft; velX = 0.0;
		} else if (((int) posX) + (width) > limitRight) {
			posX = (float) limitRight - ((float) width); velX = 0.0;
		}

		if (((int) posY) < limitUp) {
			posY = (float) limitUp; velY = 0.0;
		} else if (((int) posY) + (height) > limitDown) {
			posY = (float) limitDown - ((float) height); velY = 0.0;
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
	Level* level;
	int width, height;
	int originX, originY;
	int limitUp, limitRight, limitDown, limitLeft;
	float posX, posY;
	float velX, velY;
	std::vector<std::tuple<int, int>> blocks;

	static float bring_towards_zero(float x, float step);
};

#endif
