#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Utilities.h"
#include "cinder/Json.h"

#include "bulldog.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BullDogSimpleApp : public App {
  public:
	void setup() override;
	void draw() override;
	void keyUp(KeyEvent event) override;
	BullDogRef bd;
	string text;
	Color background;
	vec2 textPos;
};

void BullDogSimpleApp::setup()
{
	bd = BullDog::create(false);
	auto root = getAssetPath(fs::path());
	vector<fs::path> paths = {
		root / "text.txt", root / "params.json"
	};
	
	bd->watch(paths, [&]{
		auto root = getAssetPath(fs::path());
		text = loadString(loadFile(root / "text.txt"));
		JsonTree data = JsonTree(loadFile(root / "params.json"));

		auto getVal = [&](std::string key){
			return data.getValueForKey<float>(key);
		};

		background = Color(getVal("r"), getVal("g"), getVal("b"));
		textPos = vec2(getVal("textPosX"), getVal("textPosY"));
	});
	gl::enableAlphaBlending();
}


void BullDogSimpleApp::draw()
{
	gl::clear( background); 
	gl::drawStringCentered(text, textPos, Color::white(), Font("Arial", 20));
}

void BullDogSimpleApp::keyUp(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_RETURN:
		bd->disconnectUpdateSignal();
		text += "\n <update signal disconnected>";
		text += "\n <use space to manually update assets>";
		break;
	case KeyEvent::KEY_SPACE:
		bd->check();
		break;
	default:
		break;
	}
}

CINDER_APP( BullDogSimpleApp, RendererGl )
