#pragma once
#include "util.h" 

#include "Graphics.h"
#include "Sound.h"

class gameMan {

private:

	//TILE CLASS
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
		bool isFlagged() const;
		void flag();
		void setNearbyBombCounter(int foundBombs);

	private:
		state status = state::hidden;
		bool bBomb = false;
		int nNearbyBombs = -1;
	};
	//TILE CLASS END

public:
	gameMan(int nBombs);

	void draw(Graphics& gfx) const;

	RectI makeBG() const;
	void onLeftClick(const Vei2& clickCoords);
	void onRightClick(const Vei2& clickCoords);


private:
	tile& lookAt(const Vei2 & pos);
	const tile& lookAt(const Vei2 & pos) const;
	Vei2 screenToGrid(const Vei2& screenPos);
	int countNearbyBombs(const Vei2 pos);

private:

	static constexpr int fieldWidth = 20;
	static constexpr int fieldHeight = 16;
	tile field[fieldWidth * fieldHeight];
};
