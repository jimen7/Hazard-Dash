#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

std::map<std::string, Engine::MyKeys>Engine::_Keysss;
Engine::MyKeys Engine::_key;

bool gamePause = false;


void Engine::setKeysss(std::map<std::string, MyKeys> test) {
	Engine::_Keysss = test;
}

std::map<std::string, Engine::MyKeys> Engine::getKeysss() {
	return Engine::_Keysss;
}

void Engine::setXAxisValue(string s, sf::Joystick::Axis myJoysticAxis) {
	_Keysss[s].myJoysticAxis = myJoysticAxis;
}

void Engine::setXButtonValue(std::string s, int JoysticButtonNum) {
	_Keysss[s].JoysticButtonNum = JoysticButtonNum;
}

void Engine::setKeyboardbutton(std::string s, sf::Keyboard::Key myKeyCode) {
	_Keysss[s].myKeyCode = myKeyCode;
}

void Engine::setMouseButton(std::string s, sf::Mouse::Button myMouseButton) {
	_Keysss[s].myMouseButton = myMouseButton;
}
static bool TestEvent(Engine::MyKeys k, sf::Event e);





void Loading_update(float dt, const Scene* const scn) {
  //  cout << "Eng: Loading Screen\n";
  if (scn->isLoaded()) {
    cout << "Eng: Exiting Loading Screen\n";
    loading = false;
  } else {
    loadingspinner += 220.0f * dt;
    loadingTime += dt;
  }
}
void Loading_render() {
  // cout << "Eng: Loading Screen Render\n";
  static CircleShape octagon(80, 8);
  octagon.setOrigin(80, 80);
  octagon.setRotation(loadingspinner);
  octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
  octagon.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
  t.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f,0.3f));
  Renderer::queue(&t);
  Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
  static sf::Clock clock;
  float dt = clock.restart().asSeconds();
  {
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }

  if (loading) {
    Loading_update(dt, _activeScene);
  } else if (_activeScene != nullptr) {


    Physics::update(dt);
     _activeScene->Update(dt);
  }
}

void Engine::Render(RenderWindow& window) {
  if (loading) {
    Loading_render();
  } else if (_activeScene != nullptr) {
    _activeScene->Render();
  }

  Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
  RenderWindow window(VideoMode(width, height), gameName);
  _gameName = gameName;
  _window = &window;
  Renderer::initialise(window);
  Physics::initialise();
  ChangeScene(scn);


  // Variables for demo
  
	// Let's bind the left mouse button to the "Click" action
  _key.myInputType = MouseInput;
  _key.myEventType = sf::Event::MouseButtonPressed;
  _key.myMouseButton = sf::Mouse::Left;
  _Keysss["Click"] = _key;

  // Let's bind the Return key to the "Jump" action
  _key.myInputType = KeyboardInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myKeyCode = sf::Keyboard::Up;
  _key.JoysticButtonNum = 0;
  _Keysss["Jump"] = _key;

  // Let's bind the Return key to the "Movement" action when using the controller
  _key.myInputType = JoystickInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myJoysticAxis = sf::Joystick::Axis::X;
  _Keysss["Movement"] = _key;


  // Let's bind the Left Control key to the "Left" action
  _key.myInputType = KeyboardInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myKeyCode = sf::Keyboard::Left;
  _Keysss["Left"] = _key;

  //Let's bind the Left Control key to the "Right" action
  _key.myInputType = KeyboardInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myKeyCode = sf::Keyboard::Right;
  _Keysss["Right"] = _key;

  //Let's bind the Left Control key to the "Space" action
  _key.myInputType = KeyboardInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myKeyCode = sf::Keyboard::Space;
  _Keysss["Space"] = _key;

  //Let's bind the Esc Control key to the "Escape" action
  _key.myInputType = KeyboardInput;
  _key.myEventType = sf::Event::KeyPressed;
  _key.myKeyCode = sf::Keyboard::Escape;
  _Keysss["Escape"] = _key;

  //Let's bind the Esc Control key to the "Escape" action
  _key.myInputType = JoystickInput;
  _key.myEventType = sf::Event::JoystickButtonPressed;
  _key.myJoysticAxis = sf::Joystick::PovY;
  _key.JoysticButtonNum = 7;
  _Keysss["Pause"] = _key;


  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
		//setKeysss(_Keysss);
      if (event.type == Event::Closed) {
        window.close();
      }

	  /*if (event.type == Event::KeyPressed) {		//For debugging
		  cout << "PRESSED" << endl;
	  }*/
 	 if (TestEvent(_Keysss["Escape"], event))
	  {
		  // You can use a function
		  window.close();
	  }



    }


	//if (sf::Joystick::getAxisPosition(0, _Keysss["Pause"].myJoysticAxis) > 0) {
	//	gamePause = !gamePause;
	//}


	//if (sf::Joystick::isButtonPressed(0,_Keysss["Escape"].JoysticButtonNum) > 0) {
	//	gamePause = !gamePause;
	//}




    //if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    //  window.close();
    //}

	if (Keyboard::isKeyPressed(Keyboard::P)) {
		gamePause = !gamePause;
	}

    window.clear();

	if (!gamePause) {
		Update();
	}


    Render(window);
    window.display();
  }
  if (_activeScene != nullptr) {
    _activeScene->UnLoad();
    _activeScene = nullptr;
  }
  window.close();
  Physics::shutdown();
  // Render::shutdown();
}




bool TestEvent(Engine::MyKeys k, sf::Event e)
{
	// Mouse event
	if (k.myInputType == Engine::MouseInput &&
		k.myEventType == e.type &&
		k.myMouseButton == e.mouseButton.button)
	{
		return (true);
	}
	// Keyboard event
	if (k.myInputType == Engine::KeyboardInput &&
		k.myEventType == e.type &&
		k.myKeyCode == e.key.code)
	{
		return (true);
	}
	return (false);
}

std::shared_ptr<Entity> Scene::makeEntity() {
  auto e = make_shared<Entity>(this);
  ents.list.push_back(e);
  return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s) {
  cout << "Eng: changing scene: " << s << endl;
  auto old = _activeScene;
  _activeScene = s;

  if (old != nullptr) {
    old->UnLoad(); // todo: Unload Async
  }

  // Notes:
  // * There is a `thread::sleep_for(3)` insode `Scene1.load().
  // * If the game crashes change `_activeScene->LoadAsync()` to `_activeScene->Load()`.

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->Load();
    loading = true;
  }
}

// Create FloatRect to fits Game into Screen while preserving aspect
sf::FloatRect Scene::CalculateViewport(const sf::Vector2u& screensize,
	const sf::Vector2u& gamesize) {

	const Vector2f screensf(screensize.x, screensize.y);
	const Vector2f gamesf(gamesize.x, gamesize.y);
	const float gameAspect = gamesf.x / gamesf.y;
	const float screenAspect = screensf.x / screensf.y;
	float scaledWidth;  // final size.x of game viewport in piels
	float scaledHeight; //final size.y of game viewport in piels
	bool scaleSide = false; // false = scale to screen.x, true = screen.y

							//Work out which way to scale
	if (gamesize.x > gamesize.y) { // game is wider than tall
								   // Can we use full width?
		if (screensf.y < (screensf.x / gameAspect)) {
			//no, not high enough to fit game height
			scaleSide = true;
		}
		else {
			//Yes, use all width available
			scaleSide = false;
		}
	}
	else { // Game is Square or Taller than Wide
		   // Can we use full height?
		if (screensf.x < (screensf.y * gameAspect)) {
			// No, screensize not wide enough to fit game width
			scaleSide = false;
		}
		else {
			// Yes, use all height available
			scaleSide = true;
		}
	}

	if (scaleSide) { // use max screen height
		scaledHeight = screensf.y;
		scaledWidth = floor(scaledHeight * gameAspect);
	}
	else { //use max screen width
		scaledWidth = screensf.x;
		scaledHeight = floor(scaledWidth / gameAspect);
	}

	//calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}

void Scene::Update(const double& dt) { if (!gamePause) { ents.update(dt); } }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    // Are we already loading asynchronously?
    if (_loaded_future.valid() // yes
        &&                     // Has it finished?
        _loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
      // Yes
      _loaded_future.get();
      _loaded = true;
    }
    return _loaded;
  }
}
void Scene::setLoaded(bool b) {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    _loaded = b;
  }
}

void Scene::UnLoad() {
  ents.list.clear();
  setLoaded(false);
}

void Scene::Load() {
	const sf::Vector2u gamesize(GAMEX, GAMEY);
	//set View as normal
	//Engine::GetWindow().setSize(screensize);
	sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
	auto v = sf::View(visibleArea);
	// figure out how to scale and maintain aspect;
	auto viewport = CalculateViewport(Engine::GetWindow().getSize(), gamesize);
	//Optionally Center it
	bool centered = true;
	if (centered) {
		viewport.left = (1.0 - viewport.width) * 0.5;
		viewport.top = (1.0 - viewport.height) * 0.5;
	}
	//set!
	v.setViewport(viewport);
	Engine::GetWindow().setView(v);
}

void Scene::LoadAsync() {
	_loaded_future = std::async(&Scene::Load, this); 
}

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}
} // namespace timing

Scene::~Scene() { UnLoad(); }