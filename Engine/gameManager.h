#pragma once
#include "util.h" 

#include "Graphics.h"
#include "Sound.h"

class gameMan {
public:
	gameMan( int nBombs);

	void draw(Graphics& gfx) const;

	RectI makeBG() const;
	void onClick( const Vei2& pixelCoords);

private:



	class tile {
	public:
		enum class state {
			hidden,
			revealed,
			flagged
		};
		void spawnBomb();
		bool hasBomb() const;
		void drawTile(Graphics& gfx, const Vei2& pixelCoords) const;
		void reveal();
		bool isRevealed() const;
	private:
		state status = state::hidden;
		bool bBomb = false;
	};





private:
	tile& lookAt(const Vei2 & pos);
	const tile& lookAt(const Vei2 & pos) const;
	Vei2 screenToGrid(const Vei2& screenPos);

private:

	static constexpr int fieldWidth = 20;
	static constexpr int fieldHeight = 16;
	tile field[fieldWidth * fieldHeight];
};
