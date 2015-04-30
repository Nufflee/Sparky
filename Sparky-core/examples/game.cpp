#include "../src/sparky.h"

using namespace sparky;
using namespace graphics;

class Game : public Sparky
{
private:
	Window* window;
	Layer* layer;
	Label* fps;
	Sprite* sprite;
	Shader* shader;
	Model* model;
	float angle;
public:
	Game()
	{

	}

	~Game()
	{
		delete layer;
	}

	void init() override
	{
		using namespace audio;

		window = createWindow("Test Game", 960, 540);
		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
#ifdef SPARKY_EMSCRIPTEN
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else
		shader = new Shader("src/shaders/model.vert", "src/shaders/model.frag");
#endif
		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
#ifdef SPARKY_EMSCRIPTEN
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("res/tb.png"));
#else
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("tb.png"));
#endif
		layer->add(sprite);

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->add(fps);

		layer->setLineThickness(0.05f);

		SoundManager::add(new Sound("Cherno", "res/Cherno.ogg"));

		model = new Model("teapot.obj", 0.04f);
		shader->enable();
		maths::mat4 vw_matrix = maths::mat4::translation(maths::vec3(0, -2, -5));
		//vw_matrix = vw_matrix * maths::mat4::rotation(angle, maths::vec3(0, 1, 0));
		shader->setUniformMat4("vw_matrix", vw_matrix);
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		std::cout << getUPS() << " ups, " << getFPS() << " fps" << std::endl;
	}

	void update() override
	{

		float speed = 0.5f;
		if (window->isKeyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;
		else if (window->isKeyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;

		double x, y;
		window->getMousePosition(x, y);
		//shader->setUniform2f("light_pos", maths::vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
		shader->enable();
		maths::mat4 ml_matrix = maths::mat4::rotation(angle++, maths::vec3(0, 1, 0));
		shader->setUniformMat4("ml_matrix", ml_matrix);
	}

	void render() override
	{
		//layer->drawLine(maths::vec3(-2, -4, 0), maths::vec3(-1.5f, -2.5f, 0), 0xffff3020);
		//layer->render();
		shader->enable();
		model->render();
	}
};

int main()
{
	Game game;
	game.start();
	return 0;
}