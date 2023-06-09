# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "Protocol.hpp"
# include "DataController.hpp"

void Main()
{
	TextEditState id;
	Font font{ FontMethod::MSDF,20 };
	while (System::Update()) {
		SimpleGUI::TextBoxAt(id, Scene::Center());
		if (SimpleGUI::ButtonAt(U"Launch", Scene::Center() + Vec2{0, SimpleGUI::TextBoxRegion(Scene::Center()).h})) break;
	}
	TimerClient client{ IPv4Address::Localhost(),9080 };
	client.connect(atoll(id.text.narrow().c_str()));
	while (System::Update()) {
		client.update();
		font(U"残り時間: {}"_fmt(client.getTimeLeft())).drawAt(Scene::Center());
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
