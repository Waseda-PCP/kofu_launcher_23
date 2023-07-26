# pragma once
# include <Siv3D.hpp>
# include "manager.hpp"

namespace Scenes {
	constexpr double transitionTime = 0.5;

	class Selects : public App::Scene {
	public:
		Selects(const InitData&);
		void draw() const override;
		void update() override;
	private:
		struct GameData
		{
			GameData() = default;

			struct ColoredString
			{
				ColoredString() = default;
				String value;
				Color color = Palette::White;
			};

			ColoredString title;
			ColoredString creator;
			String description;
			String launch;
			Optional<String> iconPath;
			Optional<String> target;
		};

		void drawIcon(const Texture&, double, int = 0) const;
		void drawGlyphs(Vec2&, double, const String&, const Font&, Color = Palette::White) const;

		Texture background;
		Texture icon;
		Texture play;
		Texture text;
		Texture title;
		Texture chusan;
		HashTable<String, GameData> games;
		HashTable<String,Texture> icons;
		int32 selecting = 0;
		Stopwatch moveAnimation, pressing, moveTime;
		Font font{ 20 }, subject{ 20,Typeface::Bold };
		int go = 0;

		double distance;
	};
}
