#include "gameManager.h"
#include <assert.h>
#include <random>

gameMan::gameMan(Graphics & gfx)
	:
	gridOrigin(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2)
{
}

gameMan::tile & gameMan::lookAt(const Vei2 & pos)
{
	return field[pos.y * fieldWidth + pos.x];
}

const gameMan::tile & gameMan::lookAt(const Vei2 & pos) const
{
	return field[pos.y * fieldWidth + pos.x];
}

Vei2 gameMan::screenToGrid(const Vei2 & screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}


void gameMan::draw(Graphics & gfx) const
{
	const int halfWidth = (fieldWidth) / 2;
	const int halfHeight = (fieldHeight) / 2;

	gfx.DrawRect(makeBG(halfWidth, halfHeight), Color(192, 192, 192));

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < fieldHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < fieldWidth; gridPos.x++)
		{
			lookAt(gridPos).drawTile(gfx, Vei2((gridPos.x * SpriteCodex::tileSize) + (gridOrigin.x - (halfWidth * SpriteCodex::tileSize)), (gridPos.y * SpriteCodex::tileSize) + (gridOrigin.y - (halfHeight * SpriteCodex::tileSize))));
		}
	}
}

RectI gameMan::makeBG(const int halfW, const int halfH) const
{
	return RectI(gridOrigin.x - (halfW * SpriteCodex::tileSize), gridOrigin.x + (halfW * SpriteCodex::tileSize), gridOrigin.y - (halfH * SpriteCodex::tileSize), gridOrigin.y + (halfH * SpriteCodex::tileSize));
}

void gameMan::tile::drawTile(Graphics & gfx, const Vei2& pixelCoords) const
{
	SpriteCodex::DrawTileButton(pixelCoords, gfx);
}
