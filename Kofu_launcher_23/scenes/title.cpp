# include "../stdafx.h"
# include "title.hpp"
# include <filesystem>

Scenes::Selects::Selects(const InitData& init)
	:IScene{ init },
	background{ Image(Resource(U"images/background.png")) },
	icon{ Image(Resource(U"images/icon.png")) },
	play{ Image(Resource(U"images/play.png")) },
	text{ Image(Resource(U"images/text.png")) },
	title{ Image(Resource(U"images/title.png")) },
	chusan{ Image(Resource(U"images/chusan.png")) }
{
	Window::SetStyle(WindowStyle::Sizable);
	Window::SetFullscreen(true);
	Window::SetToggleFullscreenEnabled(false);
	const auto contents = FileSystem::DirectoryContents(U"games", YesNo<Recursive_tag>::No);
	for (const auto& folder : contents)
	{
		XMLReader data(U"{}/game.xml"_fmt(folder));
		if (!data)
		{
			System::MessageBoxOK(U"Error", U"Cannnot read file {}/game.xml"_fmt(folder), MessageBoxStyle::Error);
			System::Exit();
		}

		games[folder] = GameData();

		for (XMLElement& element = data; element; element = element.nextSibling())
		{
			if (element.name() == U"gameinfo")
			{
				for (auto gameInfo = element.firstChild(); gameInfo; gameInfo = gameInfo.nextSibling())
				{
					if (gameInfo.name() == U"title")
					{
						if (const auto color = gameInfo.attribute(U"color"))
						{
							games[folder].title.color = Color(*color);
						}

						games[folder].title.value = gameInfo.text();
					}

					if (gameInfo.name() == U"creator")
					{
						if (const auto color = gameInfo.attribute(U"color"))
						{
							games[folder].creator.color = Color(*color);
						}

						games[folder].creator.value = gameInfo.text();
					}

					if (gameInfo.name() == U"description")
					{
						games[folder].description = gameInfo.text();
					}

					if (gameInfo.name() == U"launch")
					{
						games[folder].launch = gameInfo.text();
					}

					if (gameInfo.name() == U"icon")
					{
						games[folder].iconPath = U"{}/{}"_fmt(folder, gameInfo.text());
					}

					if (gameInfo.name() == U"target")
					{
						games[folder].target = gameInfo.text();
					}
				}
			}
		}

		icons[folder] = Texture(Image((games[folder].iconPath ? *games[folder].iconPath : U"{}/icon.png"_fmt(folder))));

		distance = Scene::Width() / 3.0;
	}
}

void Scenes::Selects::draw() const
{
	background.resized(Scene::Size()).draw(0, 0);
	const auto playTexture = play.resized(Scene::Width());
	const auto titleTexture = title.resized(Scene::Width());
	const auto textTexture = text.resized((double)text.size().x / (double)text.size().y * Scene::Height() / 2, (double)Scene::Height() / 2);

	// アイコン描画
	for (int i = -3; i <= 3; i++)
	{
		auto it = icons.begin();
		for (int32 j = 0; j < (selecting - go + i + icons.size() * 2) % icons.size(); j++)
		{
			it++;
		}
		const double leftPos = (Scene::Width() - (double)text.size().x / (double)text.size().y * Scene::Height() / 2) / 2 - Scene::Height() / 4 - distance * i;
		drawIcon((*it).second, leftPos, i);
	}

	playTexture.draw(0, Scene::Height() - playTexture.size.y);
	titleTexture.draw(0, Scene::Height() - titleTexture.size.y - 10);
	textTexture.draw(Scene::Width() - textTexture.size.x, Scene::Height() - titleTexture.size.y - textTexture.size.y);

	auto it = games.begin();
	for (int32 j = 0; j < (selecting - go + icons.size() * 2) % icons.size(); j++)
	{
		it++;
	}

	const auto& baseX = Scene::Width() - textTexture.size.x + 114;
	Vec2 pos{ baseX, Scene::Height() - titleTexture.size.y - textTexture.size.y + 16 };

	// 作者表示
	{
		drawGlyphs(pos, baseX, U"Creator: ", subject);
		drawGlyphs(pos, baseX, (*it).second.creator.value, font, (*it).second.creator.color);
		pos = Vec2{ baseX, pos.y + Max(subject.height(), font.height()) };
	}

	// 対象年齢表記
	{
		drawGlyphs(pos, baseX, U"Target: ", subject);
		drawGlyphs(pos, baseX, ((*it).second.target ? (*it).second.target.value() : U"全年齢対象"), font);
		pos = Vec2{ baseX, pos.y + Max(subject.height(), font.height()) };
	}

	// 説明表記
	{
		drawGlyphs(pos, baseX, (*it).second.description, font);
		pos = Vec2{ baseX, pos.y + font.height() };
	}
}
void Scenes::Selects::update()
{
	if ((go < 0 && moveAnimation.sF() > -go * transitionTime) || (go > 0 && moveAnimation.sF() > go * transitionTime))
	{
		go = 0;
		moveAnimation.reset();
	}

	auto it = icons.begin();
	for (int32 i = 0; i < selecting; i++)
	{
		it++;
	}

	if (KeyRight.pressed() && go == 0)
	{
		go--;
		moveAnimation.start();
		selecting--;
		if (selecting < 0)
		{
			selecting = icons.size() - 1;
		}
	}

	if (KeyLeft.pressed() && go == 0)
	{
		go++;
		moveAnimation.start();
		selecting++;
		selecting = selecting % icons.size();
	}
}

void Scenes::Selects::drawIcon(const Texture& gameIcon, double leftPos, int pos) const
{
	const double t = Min<double>(moveAnimation.sF(), transitionTime) * (1 / transitionTime);
	double move = EaseInOutExpo(t) * distance * go;
	double sized = 0.7;
	Color borderColor = Palette::White;
	if (pos == 0)
	{
		if (go != 0)
		{
			sized = 1.0 - EaseInOutExpo(t) * 0.3;
			borderColor = Color(255, 255, 255 * EaseInOutExpo(t));
		}
		else
		{
			sized = 1.0;
			borderColor = Palette::Yellow;
		}
	}
	if (go != 0 && pos == go)
	{
		sized = EaseInOutExpo(t) * 0.3 + 0.7;
		borderColor = Color(255, 255, 255 * (1.0 - EaseInOutExpo(t)));
	}
	leftPos += move;
	const auto& gameIconSize = Min(gameIcon.size().x, gameIcon.size().y);
	const auto& clipedGameIcon = gameIcon(double(gameIcon.size().x - gameIconSize) / 2, double(gameIcon.size().y - gameIconSize) / 2, gameIconSize, gameIconSize);
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		clipedGameIcon.resized((Scene::Height() / 2) * sized).draw(leftPos, Scene::Height() - (double)Scene::Width() * title.size().y / title.size().x - (double)Scene::Height() / 2);
	}
	RoundRect{ leftPos,Scene::Height() - (double)Scene::Width() * title.size().y / title.size().x - (double)Scene::Height() / 2, Scene::Height() / 2 * sized, Scene::Height() / 2 * sized, 5 * sized }.drawFrame(20 * sized, borderColor);
}

void Scenes::Selects::drawGlyphs(Vec2& pos, double baseX, const String& value, const Font& font, Color color) const {
	const auto& glyphs = font.getGlyphs(value);
	for (const auto& glyph : glyphs)
	{
		if (pos.x + glyph.xAdvance > Scene::Width())
		{
			pos = Vec2{ baseX, pos.y + font.height() };
		}
		glyph.texture.draw(pos + glyph.getOffset());
		pos.x += glyph.xAdvance;
	}
};
