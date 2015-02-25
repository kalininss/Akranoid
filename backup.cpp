#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

const int H = 15;
const int W = 24;
int g_Score = 0;
int g_HighScore = 0;
char CharRealScore[5];
String StrGameState = "Menu";

String TileMap[H];
String TileMapLevelMap[H] = {
	"BBBBBBBBBBBBBBBBBBBBBBBB",
	"B                      B",
	"B    0   0     0  0    B",
	"B    0   0  0  0  0    B",
	"B    00000     0  0    B",
	"B    0   0  0          B",
	"B    0   0  0  0  0    B",
	"B                      B",
	"B                      B",
	"B                      B",
	"B                      B",
	"B                      B",
	"B                      B",
	"B                      B",
	"BEEEEEEEEEEEEEEEEEEEEEEB",
};

//-----------------------------------------------------------------------------------------------------------------------------
//Класс МЯЧ
//-----------------------------------------------------------------------------------------------------------------------------

class CBall {
private:
	Texture Texture;
	float dx, dy;

public:	

	Sprite Sprite;

public:
	CBall()
	{
		Texture.loadFromFile("ball.png");
		Sprite.setTexture(Texture);
		Sprite.setPosition(32 + 7, 32 * 4 + 7);
		dx = dy = 0.15;
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Функция определения столкновений, реакция и передвижение Sprite'а
	//-----------------------------------------------------------------------------------------------------------------------------

	void Check(float IntBranch_X, float IntBranch_Y, float time)
	{
		//переменные положения
		float X = Sprite.getPosition().x + dx * time;
		float Y = Sprite.getPosition().y + dy * time;
		
		//столкновение с Веткой
		if ((X + 16 > IntBranch_X) && (X < IntBranch_X + 96)
			&& ((Y + 16) >= 400) && ((Y + 16) <= 402))
		{
			dy = -dy;
			return;
		}	

		//Цикл прохода по всем смежным ячейкам
		for (int iH = (Y) / 32; iH < (Y + 16) / 32; iH++)
			for (int jW = ((X) / 32); jW < (X + 16) / 32; jW++)
			{
				//если задет бонус
				if (TileMap[iH][jW] == '0')
				{
					TileMap[iH][jW] = ' ';
					g_Score++;
					Collision(iH);
					if (g_Score == g_HighScore)
					{
						StrGameState = "Menu";
					}
					return;
				}
				//если задета стенка
				if (TileMap[iH][jW] == 'B')
				{
					Collision(iH);
					return;
				}
				//если задета нижняя граница
				if (TileMap[iH][jW] == 'E')
				{
					StrGameState = "Menu";
				}				
			}
		Sprite.move(dx * time, dy * time);
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Функция определения направления движения после отталкивания
	//-----------------------------------------------------------------------------------------------------------------------------

	void Collision(int i)
	{
		if ((dx > 0) && (dy > 0))
		{
			if ((Sprite.getPosition().y + 16) > i * 32 + 1) dx = -dx;
			else dy = -dy;
			return;
		}
		if ((dx < 0) && (dy > 0))
		{
			if ((Sprite.getPosition().y + 16) > i * 32 + 1) dx = -dx;
			else dy = -dy;
			return;
		}
		if ((dx > 0) && (dy < 0))
		{
			if (Sprite.getPosition().y > (i * 32 + 31)) dy = -dy;
			else dx = -dx;
			return;
		}
		if ((dx < 0) && (dy < 0))
		{
			if (Sprite.getPosition().y < (i * 32 + 32)) dx = -dx; 
			else dy = -dy;
			return;
		}
	}
};

//-----------------------------------------------------------------------------------------------------------------------------
//Класс ВЕТКА	
//-----------------------------------------------------------------------------------------------------------------------------

class CBranch
{
private:	
	Texture Texture;

public:
	float dx;
	Sprite Sprite;

	CBranch()
	{
		Texture.loadFromFile("branch.png");
		Sprite.setTexture(Texture);
		Sprite.setPosition(300, 400);
		dx = 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Функция обновления позиции Sprite'а
	//-----------------------------------------------------------------------------------------------------------------------------

	void Update(float time)
	{
		int IntLeft_X = (Sprite.getPosition().x + dx* time) / 32;
		int IntTop_Y = Sprite.getPosition().y / 32;

		Collision(IntLeft_X, IntTop_Y);
		Sprite.move(dx * time, 0);
		dx = 0;		
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Функция определения столкновений Ветки с границами карты (B)
	//-----------------------------------------------------------------------------------------------------------------------------

	void Collision(int IntLeft_X, int IntTop_Y)
	{
		if (TileMap[IntTop_Y][IntLeft_X] == 'B') dx = 0;
		if (TileMap[IntTop_Y][IntLeft_X+3] == 'B') dx = 0;
	}
};

//-----------------------------------------------------------------------------------------------------------------------------
//Главная функция
//-----------------------------------------------------------------------------------------------------------------------------

int main()
{
	RenderWindow window(VideoMode(32*W, 32*H), "Arkanoid::Kalinin Production");
	Clock clock;

	Font font;
	font.loadFromFile("sansation.ttf");
	Text mytext("Hello!", font, 45);
	mytext.setColor(Color::Yellow);	

	//-----------------------------------------------------------------------------------------------------------------------------
	//Инициализация текстур Меню
	//-----------------------------------------------------------------------------------------------------------------------------

	Texture menu_texture1, menu_texture2, menu_texture3, about_texture;
	menu_texture1.loadFromFile("111.png");
	menu_texture2.loadFromFile("222.png");
	menu_texture3.loadFromFile("333.png");
	about_texture.loadFromFile("about.png");
	Sprite menu1(menu_texture1), menu2(menu_texture2), menu3(menu_texture3), about(about_texture);
	
	int MenuNum = 0;
	menu1.setPosition(270, 60);
	menu2.setPosition(270, 120);
	menu3.setPosition(270, 180);

	while (window.isOpen())
	{
	//-----------------------------------------------------------------------------------------------------------------------------
	//Вход в цикл меню
	//-----------------------------------------------------------------------------------------------------------------------------
	while (StrGameState == "Menu")
	{
		window.clear(Color::Color(50, 50, 50));

		MenuNum = 0;
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);		
		if (IntRect(270, 60, 170, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); MenuNum = 1; }
		if (IntRect(270, 120, 200, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); MenuNum = 2; }
		if (IntRect(270, 180, 120, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Yellow); MenuNum = 3; }
		
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
			if (MenuNum == 1) StrGameState = "Game";
			if (MenuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (MenuNum == 3) { window.close(); StrGameState = ""; }
		}
		
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.display();
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Инициализация объектов перед переходом в режим игры
	//-----------------------------------------------------------------------------------------------------------------------------
	
	CBall* BallObject = new CBall;
	CBranch* BranchObject = new CBranch;
	RectangleShape rectangle(Vector2f(32, 32));
	mytext.setPosition(0, 0);
	g_Score = 0;
	g_HighScore = 0;

	for (int i = 0; i < H; i++)
	{	//Копирование карты
		TileMap[i] = TileMapLevelMap[i];
	}
	for (int i = 0; i<H; i++)
		for (int j = 0; j<W; j++)
		{	//Получение суммы HighScore
			if (TileMap[i][j] == '0') g_HighScore++;
		}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Вход в режим отрисовки игрового процесса
	//-----------------------------------------------------------------------------------------------------------------------------

	while (StrGameState == "Game")
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

		if (Keyboard::isKeyPressed(Keyboard::Left)) BranchObject->dx = -0.3;		
		if (Keyboard::isKeyPressed(Keyboard::Right)) BranchObject->dx = 0.3;
		
		BallObject->Check(BranchObject->Sprite.getPosition().x, 
							BranchObject->Sprite.getPosition().y, time);
		BranchObject->Update(time);

		window.clear(Color::Color(50,50,50));

		for (int i = 0; i<H; i++)
			for (int j = 0; j<W; j++)
			{
				if (TileMap[i][j] == 'B') rectangle.setFillColor(Color::Black);
				if (TileMap[i][j] == 'E') rectangle.setFillColor(Color::Black);
				if (TileMap[i][j] == '0') rectangle.setFillColor(Color::Yellow);
				if (TileMap[i][j] == ' ') continue;

				rectangle.setPosition(j * 32, i * 32);
				window.draw(rectangle);
			}

		window.draw(BallObject->Sprite);
		window.draw(BranchObject->Sprite);

		//IntToStr и вывод очков
		std::ostringstream ss;
		ss << g_Score;
		mytext.setString(ss.str());
		window.draw(mytext);

		window.display();
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	//Переход в режим отрисовки счета
	//-----------------------------------------------------------------------------------------------------------------------------

	delete BallObject;
	delete BranchObject;
	StrGameState = "Record";

	Text tRecord("Your score:", font, 45);
	Text tMenu("Menu", font, 45);
	tRecord.setColor(Color::White);
	tMenu.setColor(Color::White);
	tRecord.setPosition(270, 60);
	mytext.setPosition(350, 120);
	tMenu.setPosition(320, 220);

	//-----------------------------------------------------------------------------------------------------------------------------
	//Режим вывода счета
	//-----------------------------------------------------------------------------------------------------------------------------

	while (StrGameState == "Record")
	{
		MenuNum = 0;
		tMenu.setColor(Color::White);

		if (IntRect(320, 220, 120, 50).contains(Mouse::getPosition(window))) { tMenu.setColor(Color::Yellow); MenuNum = 1; }

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
			if (MenuNum == 1) StrGameState = "Menu";
		}

		window.clear(Color::Color(50, 50, 50));
		window.draw(tRecord);
		window.draw(tMenu);
		window.draw(mytext);
		window.display();
	}
	}

	return 0;
}

