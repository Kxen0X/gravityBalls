#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <random>
#pragma once


struct Arc {
	float minX;
	float maxX;
	float minY;
	float maxY;
	sf::Vector2f centr;
	sf::VertexArray arc;
	float radius;
	float radius2;
	float baseAngle;
	float currentRotationAngle;
	int idx;
};

enum State {
	MENU,
	GAME,
	END
};

struct Ball {
	sf::CircleShape ball;
	sf::Vector2f position;
	sf::Vector2f lastPosition;
	sf::Vector2f velocity;
	int sw;
	sf::Sprite* sword;
	sf::RectangleShape swordBorders;
};

class Project {

public:
	Project();
	~Project();

	void run();
	void updateInGame();
	void updateInMenu();
	void updateInEnd();
	void renderInGame();
	void renderInMenu();
	void renderInEnd();
private:
	int i = 1;
	State state = State::GAME;

	// Menu part
	sf::Font font;
	sf::Text* text;
	sf::Text* ButtonText;
	sf::Text* ExitText;
	sf::RectangleShape button;
	sf::RectangleShape exit;
	sf::Vector2f MousePos;

	// Game part
	sf::Color getRGBColor(int);
	void setRGBColor();

	sf::RenderWindow* window;
	std::vector<Ball> circles;
	std::vector<sf::CircleShape> shades;
	sf::Color rgbColor;

	std::vector<Arc> arcs;

	void eventUpdate();


	Arc createArc(int, sf::Vector2f, int, int, float, float);
	void createCircle(int);
	
	int countOfCircles;
	int maxCountOfCircles;


	int h;

	float gravity;

	sf::Clock time;
	float deltatime;
	int RGBTimer;
	sf::Vector2f centr;


	sf::Vector2f normalize(sf::Vector2f);
	float Scalar(sf::Vector2f, sf::Vector2f);
	float length(sf::Vector2f);


	void arcCollision();
	void ballsCollision();
	void mouseCollision();
	void swordCollision();
	void arcRotation();
	// End part

	sf::Text* youLose;
	sf::Text* timerText;
	sf::Sprite* death;
	sf::Texture* txt;
	int timer;
	sf::Clock out;


	sf::Texture sword;
	sf::Sprite* swordSprite1;
	sf::RectangleShape swordBorders;
};