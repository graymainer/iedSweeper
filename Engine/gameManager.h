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
	gameMan(Graphics& gfx);

	void draw(Graphics& gfx) const;

private:
	tile& lookAt(const Vei2 & pos);
	const tile& lookAt(const Vei2 & pos) const;
	Vei2 screenToGrid(const Vei2& screenPos);

	RectI makeBG(const int halfW, const int halfH) const;
	RectI makeBorder(const int halfW, const int halfH, const int size) const;

	void drawBackground(Graphics & gfx, const int halfW, const int halfH, 
		const int in_size, const Color bgClr, const Color bdrClr) const;

private:

	static constexpr int fieldWidth = 20;
	static constexpr int fieldHeight = 16;
	const Vei2 gridOrigin; //based on center of grid.
	tile field[fieldWidth * fieldHeight];

	static constexpr int borderSize = 20;
	static constexpr Color backgroundColor = Color(192, 192, 192);
	static constexpr Color borderColor = Color(110, 103, 78);
};
