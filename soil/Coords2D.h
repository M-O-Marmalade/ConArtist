// Data type which stores a 2D x/y coordinate.

#pragma once

namespace Soil {
	class Coords2D {
	public:
		int x = 0;
		int y = 0;

		bool operator == (const Coords2D& other) {
			if (this->x != other.x || this->y != other.y) {
				return false;
			}
			return true;
		}

		void operator += (const Coords2D& other) {
			this->x += other.x;
			this->y += other.y;
		}

		Coords2D operator + (const int num) {
			this->x += num;
			this->y += num;
			return *this;
		}

		Coords2D(int x = 0, int y = 0) : x{ x }, y{ y } {};
	};
}