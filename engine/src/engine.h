#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>
#include <SFML\Window\Event.hpp>
#include <map>

#define GAMEX 1920
#define GAMEY 1080
#define DIVIDER 4.0f
#define REBIND false;

class Scene {
public:
  Scene() = default;
  virtual ~Scene();
  virtual void Load();
  virtual void LoadAsync();
  virtual void UnLoad();
  virtual void Update(const double& dt);
  virtual void Render();
  sf::FloatRect CalculateViewport(const sf::Vector2u& screensize,
	  const sf::Vector2u& gamesize);
  bool isLoaded() const;
  std::shared_ptr<Entity> makeEntity();
  // For the pause menu
  std::shared_ptr<Entity> makeEntity2();

  EntityManager ents;
  EntityManager ents2;

protected:
  void setLoaded(bool);
private:
  mutable bool _loaded;
  mutable std::future<void> _loaded_future;
  mutable std::mutex _loaded_mtx;
};

class Engine {
public:
	Engine() = delete;
	static void Start(unsigned int width, unsigned int height,
		const std::string& gameName, Scene* scn);
	static void ChangeScene(Scene*);
	static sf::RenderWindow& GetWindow();
	static sf::Vector2u getWindowSize();
	static void setVsync(bool b);
	static bool _gamePause;


	//MOUSE BINDING VARIABLES AND METHODS

	static enum InputType
	{
		KeyboardInput,
		MouseInput,
		JoystickInput
	};

	static struct MyKeys
	{
		InputType myInputType;
		sf::Event::EventType myEventType;
		sf::Keyboard::Key myKeyCode;
		sf::Mouse::Button myMouseButton;
		sf::Joystick::Axis myJoysticAxis;
		int JoysticButtonNum;
	};

	static std::map<std::string, MyKeys> getKeysss();
	static void setKeysss(std::map<std::string, MyKeys> test);
	static MyKeys _key;
	static std::map<std::string, MyKeys> _Keysss;
	static void setXAxisValue(std::string s, sf::Joystick::Axis myJoysticAxis);
	static void setXButtonValue(std::string s, int JoysticButtonNum);
	static void setKeyboardbutton(std::string s, sf::Keyboard::Key myKeyCode);
	static void setMouseButton(std::string s, sf::Mouse::Button myMouseButton);
	static bool getPause() { return _gamePause; }
	static void setPause(bool gamePause) { _gamePause = gamePause; }

private:
  static Scene* _activeScene;
  static std::string _gameName;
  static void Update();
  static void Render(sf::RenderWindow& window);

  

};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing