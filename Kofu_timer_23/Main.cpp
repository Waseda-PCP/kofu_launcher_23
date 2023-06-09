# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "Protocol.hpp"

using namespace std;

void Main()
{
	TimerServer timer;
	timer.connect();
	Window::SetStyle(WindowStyle::Sizable);
	Window::Maximize();
	Font font{ FontMethod::MSDF,20 }, font25{ FontMethod::MSDF,25 };
	map<size_t, TextEditState> state;
	size_t guiHeight = SimpleGUI::ButtonRegion(U"IE", { 0,0 }).h;
	while (System::Update()) {
		size_t i = 0;
		set<size_t> use;
		for (auto& id : timer.getTimeLefts()) {
			use.insert(id.first);
			Rect{ i / 2 * 300,(i % 2 == 0 ? 0 : Scene::Size().y / 2),300,Scene::Size().y / 2 }.draw(Palette::White);
			font((timer.getPCId(id.first) != -1 ? U"PC ID: {}"_fmt(timer.getPCId(id.first)) : U"ConnectID: {}"_fmt(id.first))).drawAt(i / 2 * 300 + 150, (i % 2 == 0 ? 0 : Scene::Size().y / 2) + font.height(), Palette::Black);
			long long time(id.second >= 0 ? (id.second) : 1 - id.second);
			font25(U"{}{}:{:0>2}:{:0>2}:{:0>2}"_fmt((id.second >= 0 ? U"" : U"-"),time / (3600 * 24), time / 3600, time / 60, time % 60)).drawAt(i / 2 * 300 + 150, (i % 2 == 0 ? 0 : Scene::Size().y / 2) + font.height() * 2, Palette::Black);
			if (!state.count(id.first)) state[id.first] = TextEditState();
			SimpleGUI::TextBoxAt(state[id.first], { i / 2 * 300 + 150, (i % 2 == 0 ? 0 : Scene::Size().y / 2) + font.height() * 2 + font25.height() }, 200.0, 5);
			if (SimpleGUI::ButtonAt(U"SET", { i / 2 * 300 + 150, (i % 2 == 0 ? 0 : Scene::Size().y / 2) + font.height() * 2 + font25.height() + guiHeight })) {
				timer.setTime(id.first, atoll(state[id.first].text.narrow().c_str()));
			}
			if (SimpleGUI::ButtonAt(U"RESET", { i / 2 * 300 + 150, (i % 2 == 0 ? 0 : Scene::Size().y / 2) + font.height() * 2 + font25.height() + guiHeight * 2 })) {
				timer.setTime(id.first, timer.defaultTime);
			}
			i++;
		}
		timer.update();
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
