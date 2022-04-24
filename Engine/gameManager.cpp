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

	drawBackground(gfx, halfWidth, halfHeight, borderSize, backgroundColor, borderColor);



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

RectI gameMan::makeBorder(const int halfW, const int halfH, const int size) const
{
	return RectI((gridOrigin.x - (halfW * SpriteCodex::tileSize)) - size, (gridOrigin.x + (halfW * SpriteCodex::tileSize)) + size, (gridOrigin.y - (halfH * SpriteCodex::tileSize)) - size, (gridOrigin.y + (halfH * SpriteCodex::tileSize)) + size);
}

void gameMan::drawBackground(Graphics & gfx, const int halfW, const int halfH, const int in_size, const Color bgClr, const Color bdrClr) const
{
	gfx.DrawRect(makeBorder(halfW, halfH, in_size), bdrClr);
	gfx.DrawRect(makeBG(halfW, halfH), bgClr);
}

void gameMan::tile::drawTile(Graphics & gfx, const Vei2& pixelCoords) const
{
	SpriteCodex::DrawTileButton(pixelCoords, gfx);
}
