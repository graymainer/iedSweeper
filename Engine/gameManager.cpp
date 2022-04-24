#include "gameManager.h"
#include <assert.h>
#include <random>

gameMan::gameMan(int nBombs)
{
	assert(nBombs > 0 && nBombs < gridWidth * gridHeight);

	nTilesToReveal -= nBombs;

	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> xRange(0, gridWidth - 1);
	std::uniform_int_distribution<int> yRange(0, gridHeight - 1);

	for (int nSpawned = 0; nSpawned < nBombs; nSpawned++)
	{
		Vei2 spawnPos;

		do
		{
			spawnPos = { xRange(rng), yRange(rng) };
		} while (lookAt(spawnPos).hasBomb());

		lookAt(spawnPos).spawnBomb();
	}

	for (Vei2 gridPos{ 0,0 }; gridPos.y < gridHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < gridWidth; gridPos.x++)
		{
			lookAt(gridPos).setNearbyBombCounter(countNearbyBombs(gridPos));
		}
	}

#if !_DEBUG //just so that when i debug while listening to music, i dont shoot myself
	startSoundScape();
#endif

	beginSnd.Play(1.0f, 0.15f);
}

gameMan::tile & gameMan::lookAt(const Vei2 & pos)
{
	return field[pos.y * gridWidth + pos.x];
}

const gameMan::tile & gameMan::lookAt(const Vei2 & pos) const
{
	return field[pos.y * gridWidth + pos.x];
}

Vei2 gameMan::screenToGrid(const Vei2 & screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

int gameMan::countNearbyBombs(const Vei2 pos)
{
	const int xStart = std::max(0, pos.x - 1);
	const int yStart = std::max(0, pos.y - 1);

	const int xEnd = std::min(gridWidth - 1, pos.x + 1);
	const int yEnd = std::min(gridHeight - 1, pos.y + 1);

	int bombCount = 0;


	for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (lookAt(gridPos).hasBomb()) //is pulling a non existant tile
				bombCount++;
		}
	}

	return bombCount;
}

void gameMan::stopSoundScape()
{
	ambMus.StopAll();
	ambGun.StopAll();
	ambWind.StopAll();
}

void gameMan::startSoundScape()
{
	ambMus.Play(1.0f, 0.35f);
	ambGun.Play(1.0f, 0.07f);
	ambWind.Play(1.0f, 0.12f);
}

void gameMan::draw(Graphics & gfx) const
{
	gfx.DrawRect(makeBG(), Color(192, 192, 192));

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < gridHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < gridWidth; gridPos.x++)
		{
			lookAt(gridPos).drawTile(gfx, gridPos * SpriteCodex::tileSize, bGameOver, bGameWon);
		}
	}
}

RectI gameMan::makeBG() const
{
	return RectI( 0, gridWidth * SpriteCodex::tileSize, 0, gridHeight * SpriteCodex::tileSize);
}

void gameMan::onRightClick(const Vei2 & clickCoords)
{
	if (bGameOver || bGameWon)
		return;

	const Vei2 gridPos = screenToGrid(clickCoords);
	assert((gridPos.x >= 0 && gridPos.x < gridWidth) && (gridPos.y >= 0 && gridPos.y < gridHeight));

	tile& clickedTile = lookAt(gridPos);

	if (!clickedTile.isRevealed())
		clickedTile.flag();

}

void gameMan::onLeftClick(const Vei2 & clickCoords)
{
	if (bGameOver || bGameWon)
		return;

	const Vei2 gridPos = screenToGrid(clickCoords);
	assert((gridPos.x >= 0 && gridPos.x < gridWidth) && (gridPos.y >= 0 && gridPos.y < gridHeight));

	tile& clickedTile = lookAt(gridPos);

	if (!clickedTile.isRevealed() && !clickedTile.isFlagged())
	{
		clickedTile.reveal();
		if (clickedTile.hasBomb())
			
		{
			boomSnd.Play(1.0f, 0.15f);
			bGameOver = true;
			stopSoundScape();
		}
		else
		{
			nTilesToReveal--;

			if (nTilesToReveal <= 0)
			{
				assert(nTilesToReveal >= 0); //shouldn't be a negative num
				bGameWon = true;
				winSnd.Play(1.0f, 0.15f);
				stopSoundScape();
			}
		}
			
	}
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

void gameMan::tile::drawTile(Graphics & gfx, const Vei2& pixelCoords, const bool bFucked, const bool bWon) const
{
	if (bFucked)
	{
		switch (status)
		{
		case state::hidden:
			if (hasBomb())
				SpriteCodex::DrawTileBomb(pixelCoords, gfx);
			else
				SpriteCodex::DrawTile0(pixelCoords, gfx);

			break;

		case state::revealed:
			if (hasBomb())
				SpriteCodex::DrawTileBombRed(pixelCoords, gfx);
			else
				SpriteCodex::DrawTileNumber(pixelCoords, nNearbyBombs, gfx);

			break;

		case state::flagged:
			if (hasBomb())
			{
				SpriteCodex::DrawTileBomb(pixelCoords, gfx);
				SpriteCodex::DrawTileFlag(pixelCoords, gfx);
			}	
			else
			{
				SpriteCodex::DrawTileButton(pixelCoords, gfx);
				SpriteCodex::DrawTileCross(pixelCoords, gfx);
			}

			break;

		default:
			assert(0 > 1);
			break;
		}

		return;
	}
	else if (bWon)
	{
		switch (status)
		{
		case state::hidden:
			if (hasBomb())
				SpriteCodex::DrawTileBomb(pixelCoords, gfx);
			else
				SpriteCodex::DrawTile0(pixelCoords, gfx);
			break;

		case state::revealed:
			SpriteCodex::DrawTileNumber(pixelCoords, nNearbyBombs, gfx);
			break;

		case state::flagged:
			if (hasBomb())
			{
				SpriteCodex::DrawTileBomb(pixelCoords, gfx);
				SpriteCodex::DrawTileFlag(pixelCoords, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(pixelCoords, gfx);
				SpriteCodex::DrawTileCross(pixelCoords, gfx);
			}

			break;

		default:
			assert(0 > 1);
			break;
		}
		return;
	}

	switch (status)
	{
	case state::hidden:
		SpriteCodex::DrawTileButton(pixelCoords, gfx);
		break;

	case state::revealed:
		if (!hasBomb())
			SpriteCodex::DrawTileNumber(pixelCoords, nNearbyBombs, gfx);
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
	status = state::revealed;
}

bool gameMan::tile::isRevealed() const
{
	return status == state::revealed;
}

bool gameMan::tile::isFlagged() const
{
	return status == state::flagged;
}

void gameMan::tile::flag()
{
	assert(!isRevealed());

	if (isFlagged())
		status = state::hidden;
	else
		status = state::flagged;
}

void gameMan::tile::setNearbyBombCounter(int foundBombs)
{

	assert(nNearbyBombs == -1);


	nNearbyBombs = foundBombs;
}
