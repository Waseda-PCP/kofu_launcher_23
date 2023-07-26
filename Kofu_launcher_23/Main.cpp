# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "Protocol.hpp"
# include "DataController.hpp"
# include "scenes/scenes.hpp"
# include <thread>

using namespace std;

void Main()
{
	TextEditState id;
	Font font{ FontMethod::MSDF,20 };

	bool pass = false;

	while (System::Update())
	{
		SimpleGUI::TextBoxAt(id, Scene::Center());
		if (SimpleGUI::ButtonAt(U"Launch", Scene::Center() + Vec2{0, SimpleGUI::TextBoxRegion(Scene::Center()).h})) break;
		if (SimpleGUI::ButtonAt(U"Pass", Scene::Center() + Vec2{0, SimpleGUI::TextBoxRegion(Scene::Center()).h + SimpleGUI::ButtonRegionAt(U"Launch", { 0,0 }).h}))
		{
			pass = true;
			break;
		}
	}

	TimerClient client;
	if (!pass)
	{
		client = TimerClient{ IPv4Address::Localhost(),9080 };
		client.connect(atoll(id.text.narrow().c_str()));
	}

	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	App manager;

	manager.add<Scenes::Selects>(U"title");

	while (System::Update())
	{
		manager.update();
		if (KeyAlt.pressed() && KeyF4.pressed())
		{
			System::Exit();
		}
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
