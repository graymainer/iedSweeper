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
		void drawTile(Graphics& gfx, const Vei2& pixelCoords, const bool bFucked, const bool bWon) const;
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
	gameMan(Graphics& gfx, int nBombs);

	void draw(Graphics& gfx) const;

	RectI makeBG(const int halfW, const int halfH) const;
	RectI makeBorder(const int halfW, const int halfH, const int size) const;

	void drawBackground(Graphics & gfx, const int halfW, const int halfH,
		const int in_size, const Color bgClr, const Color bdrClr) const;


	void onLeftClick(const Vei2& clickCoords);
	void onRightClick(const Vei2& clickCoords);


private:
	tile& lookAt(const Vei2 & pos);
	const tile& lookAt(const Vei2 & pos) const;
	Vei2 screenToGrid(const Vei2& screenPos);
	int countNearbyBombs(const Vei2 pos);
	void stopSoundScape();
	void startSoundScape();

public:

	//grid stuff
	static constexpr int gridWidth = 20;
	static constexpr int gridHeight = 16;

private:

	//some other grid stuff

	const Vei2 gridOrigin; //based in the center of the grid.
	tile field[gridWidth * gridHeight];

	//background rendering stuff
	static constexpr int borderSize = 20;
	static constexpr Color backgroundColor = Color(192, 192, 192);
	static constexpr Color borderColor = Color(110, 103, 78);

	//game conditional stuff
	bool bGameOver = false;
	bool bGameWon = false;
	int nTilesToReveal = gridWidth * gridHeight;

private:
	//game conditional sounds
	Sound boomSnd = { L"Sounds\\boom.wav" };
	Sound winSnd = { L"Sounds\\win.wav" };
	Sound beginSnd = { L"Sounds\\begin.wav" };

	//soundscape sounds
	Sound ambMus = { L"Sounds\\amb_music.wav", Sound::LoopType::AutoFullSound };
	Sound ambGun = { L"Sounds\\amb_gunfire.wav", Sound::LoopType::AutoFullSound };
	Sound ambWind = { L"Sounds\\amb_wind.wav", Sound::LoopType::AutoFullSound };


};
