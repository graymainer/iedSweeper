#pragma once
#include "util.h" 

#include "Graphics.h"

class gameMan {

private:

	//TILE CLASS
	class tile {
	public:
		void drawTile(Graphics& gfx, const Vei2& pixelCoords) const;

	};
	//TILE CLASS END

public:
	gameMan();

	void draw(Graphics& gfx) const;

	RectI makeBG() const;


private:
	tile& lookAt(const Vei2 & pos);
	const tile& lookAt(const Vei2 & pos) const;
	Vei2 screenToGrid(const Vei2& screenPos);

private:

	static constexpr int fieldWidth = 20;
	static constexpr int fieldHeight = 16;
	tile field[fieldWidth * fieldHeight];
};
