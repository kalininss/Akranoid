#include <SFML/Graphics.hpp>

using namespace sf;

const int H = 12;
const int W = 20;
int IntScore = 0;
bool GameState = 0;
bool Menu = 1;

String TileMap[H] = {
	"BBBBBBBBBBBBBBBBBBBB",
	"B                  B",
	"B   000000000000   B",
	"B   000000000000   B",
	"B                  B",
	"B                  B",
	"B                  B",
	"B                  B",
	"B                  B",
	"B                  B",
	"B                  B",
	"BEEEEEEEEEEEEEEEEEEB",
};

class BALL {
private:
	Texture TextureBall;
public:
	float dx, dy;
	Sprite sprite;

	BALL()
	{
		TextureBall.loadFromFile("ball.png");
		sprite.setTexture(TextureBall);
		sprite.setPosition(32 + 16, 32 * 3 + 16);

		dx = dy = 0.1;
	}

	void update(float time)
	{
		sprite.move(dx * time, dy * time);
	}

	void Collision(float IntBranch_X, float IntBranch_Y)
	{
		int varX = sprite.getPosition().x / 32;
		int varY = sprite.getPosition().y / 32;
		int vary = (sprite.getPosition().y + 16) / 32;
		int varx = (sprite.getPosition().x + 16) / 32;

		if ((sprite.getPosition().x + 32 > IntBranch_X) && (sprite.getPosition().x < IntBranch_X + 96)
			&& (sprite.getPosition().y + 32 >= 300) && (sprite.getPosition().y + 32 <= 302))
		{
			dy = -dy;
			return;
		}

		if ((dy > 0) && (TileMap[varY + 1][varx] == 'B')
			|| (TileMap[varY + 1][varx] == '0'))
			dy = -dy;
		else
		{
			if (((TileMap[varY][varx]) == 'B')
				|| ((TileMap[varY][varx]) == '0'))
				dy = -dy;
		}

		if ((dx > 0) && (TileMap[vary][varX + 1] == 'B')
			|| (TileMap[vary][varX + 1] == '0'))
			dx = -dx;
		else
		{
			if ((TileMap[vary][varX] == 'B')
				|| (TileMap[vary][varX] == '0'))
				dx = -dx;
		}

		for (int i = sprite.getPosition().y / 32; i<(sprite.getPosition().y + 32) / 32; i++)
			for (int j = sprite.getPosition().x / 32; j<(sprite.getPosition().x + 32) / 32; j++)
			{
				if (TileMap[i][j] == '0')
				{
					TileMap[i][j] = ' ';
					IntScore++;
					if (IntScore == 24)
					{
						GameState = false;
						Menu = 1;
					}
				}
				if (TileMap[i][j] == 'E')
				{
					GameState = false;
					Menu = 1;
				}
			}
	}
};

class BRANCH
{
private:
	Texture TextBranch;

public:
	float dx;
	Sprite SpriteBranch;

	BRANCH()
	{
		TextBranch.loadFromFile("branch.png");
		SpriteBranch.setTexture(TextBranch);
		SpriteBranch.setPosition(200, 300);

		dx = 0;
	}

	void update(float time)
	{
		int IntLeft_X = (SpriteBranch.getPosition().x + dx) / 32;
		int IntTop_Y = SpriteBranch.getPosition().y / 32;

		collision(IntLeft_X, IntTop_Y);
		SpriteBranch.move(dx * time, 0);
		dx = 0;

	}

	void collision(int IntLeft_X, int IntTop_Y)
	{
		if (TileMap[IntTop_Y][IntLeft_X] == 'B') dx = 0;
		if (TileMap[IntTop_Y][IntLeft_X + 3] == 'B') dx = 0;
	}
};

int main()
{
	RenderWindow window(VideoMode(640, 384), "Arkanoid::Kalinin Production");


	BRANCH BranchObject;
	Clock clock;

	RectangleShape rectangle(Vector2f(32, 32));

	Texture menu_texture1, menu_texture2, menu_texture3, about_texture;
	menu_texture1.loadFromFile("111.png");
	menu_texture2.loadFromFile("222.png");
	menu_texture3.loadFromFile("333.png");
	about_texture.loadFromFile("about.png");
	Sprite menu1(menu_texture1), menu2(menu_texture2), menu3(menu_texture3), about(about_texture);

	int MenuNum = 0;
	menu1.setPosition(200, 30);
	menu2.setPosition(200, 90);
	menu3.setPosition(200, 150);

	while (window.isOpen())
	{
		/////меню
		while (Menu)
		{
			menu1.setColor(Color::White);
			menu2.setColor(Color::White);
			menu3.setColor(Color::White);
			MenuNum = 0;
			window.clear(Color(0, 0, 0));

			if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); MenuNum = 1; }
			if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); MenuNum = 2; }
			if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Yellow); MenuNum = 3; }

			Event event;

			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
					return 0;
				}
			}

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (MenuNum == 1)
				{
					Menu = false;
					GameState = 1;
				}
				if (MenuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (MenuNum == 3)  { window.close(); Menu = false; }

			}


			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);

			window.display();
		}
		BALL* BallObject = new BALL;
		while (GameState)
		{
			float time = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			time = time / 700;

			if (time>20) time = 20;

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
					return 0;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				BranchObject.dx = -0.3;
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				BranchObject.dx = 0.3;
			}
			BallObject->update(time);
			BallObject->Collision(BranchObject.SpriteBranch.getPosition().x, BranchObject.SpriteBranch.getPosition().y);

			BranchObject.update(time);

			window.clear(Color::White);

			for (int i = 0; i<H; i++)
				for (int j = 0; j<W; j++)
				{
					if (TileMap[i][j] == 'B') rectangle.setFillColor(Color::Black);

					if (TileMap[i][j] == 'E') rectangle.setFillColor(Color::Black);

					if (TileMap[i][j] == '0') rectangle.setFillColor(Color::Green);

					if (TileMap[i][j] == ' ') continue;

					rectangle.setPosition(j * 32, i * 32);
					window.draw(rectangle);
				}
			window.draw(BranchObject.SpriteBranch);
			window.draw(BallObject->sprite);
			window.display();
		}

		delete BallObject;

	}
	return 0;
}

