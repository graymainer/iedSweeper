#include "gameManager.h"
#include <assert.h>
#include <random>

gameMan::gameMan()
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

void gameMan::tile::drawTile(Graphics & gfx, const Vei2& pixelCoords) const
{
	SpriteCodex::DrawTileButton(pixelCoords, gfx);
}
