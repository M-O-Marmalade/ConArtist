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

		Coords2D(int x = 0, int y = 0) : x{ x }, y{ y } {};
	};
}