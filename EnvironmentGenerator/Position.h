#ifndef POSITION_H
#define POSITION_H

struct Position
{
	enum Direction
	{
		PositiveX,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ
	};

	Position() :
		x(0), y(0), z(0) {}

	Position(const Position& other) :
		x(other.x), y(other.y), z(other.z) {}

	Position(int x, int y, int z) :
		x(x), y(y), z(z) {}

	int x, y, z;

	Position getNeighbour(Direction dir) const
	{
		switch (dir)
		{
		case Position::PositiveX:
			return Position(this->x + 1, this->y, this->z);
		case Position::NegativeX:
			return Position(this->x - 1, this->y, this->z);
		case Position::PositiveY:
			return Position(this->x, this->y + 1, this->z);
		case Position::NegativeY:
			return Position(this->x, this->y - 1, this->z);
		case Position::PositiveZ:
			return Position(this->x, this->y, this->z + 1);
		case Position::NegativeZ:
			return Position(this->x, this->y, this->z - 1);
		default:
			return Position();
		}
	}

	float distanceTo(const Position& other) const
	{
		return sqrtf(powf(static_cast<float>(other.x - this->x), 2.0f) + powf(static_cast<float>(other.y - this->y), 2.0f) + powf(static_cast<float>(other.z - this->z), 2.0f));
	}

	bool operator==(const Position& other) const
	{
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	Position& operator=(const Position& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
};

#endif // !POSITION_H
