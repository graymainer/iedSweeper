#include "gameManager.h"
#include <assert.h>
#include <random>

gameMan::gameMan(int nBombs)
{
	assert(nBombs > 0 && nBombs < fieldWidth * fieldHeight);

	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> xRange(0, fieldWidth - 1);
	std::uniform_int_distribution<int> yRange(0, fieldHeight - 1);

	for (int nSpawned = 0; nSpawned < nBombs; nSpawned++)
	{
		Vei2 spawnPos;

		do
		{
			spawnPos = { xRange(rng), yRange(rng) };
		} while (lookAt(spawnPos).hasBomb());

		lookAt(spawnPos).spawnBomb();
	}

	//for (int i = 0; i < 120l; i++)
	//{
	//	const Vei2 pos = { xRange(rng), yRange(rng) };
	//	
	//	if (lookAt(pos).isRevealed())
	//		return;

	//	lookAt(pos).reveal();
	//}
}

gameMan::tile & gameMan::lookAt(const Vei2 & pos)
{
	return field[pos.y * fieldWidth + pos.x];
}

const gameMan::tile & gameMan::lookAt(const Vei2 & pos) const
{
	return field[pos.y * fieldWidth + pos.x];
}

void gameMan::draw(Graphics & gfx) const
{
	gfx.DrawRect(makeBG(), Color(192, 192, 192));

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < fieldHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < fieldWidth; gridPos.x++)
		{
			lookAt(gridPos).drawTile(gfx, gridPos * SpriteCodex::tileSize);
		}
	}
}

RectI gameMan::makeBG() const
{
	return RectI( 0, fieldWidth * SpriteCodex::tileSize, 0, fieldHeight * SpriteCodex::tileSize);
}

void gameMan::tile::spawnBomb()
{
	assert(!hasBomb());
	bBomb = true;
}

bool gameMan::tile::hasBomb() const
{
	return bBomb;
}

void gameMan::tile::drawTile(Graphics & gfx, const Vei2& pixelCoords) const
{
	switch (status)
	{
	case state::hidden:
		SpriteCodex::DrawTileButton(pixelCoords, gfx);
		break;

	case state::revealed:
		if (!hasBomb())
			SpriteCodex::DrawTile0(pixelCoords, gfx);
		else
			SpriteCodex::DrawTileBomb(pixelCoords, gfx);

		break;

	case state::flagged:
		SpriteCodex::DrawTileButton(pixelCoords, gfx);
		SpriteCodex::DrawTileFlag(pixelCoords, gfx);
		break;

	default:
		assert(0 > 1);
		break;
	}
}

void gameMan::tile::reveal()
{
	assert(status == state::hidden);

	if (status == state::flagged)
		return;

	status = state::revealed;
}

bool gameMan::tile::isRevealed() const
{
	return status == state::revealed;
}
