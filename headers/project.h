#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>



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
	bool isBreaked = 0;
};



struct Ball {
	sf::CircleShape ball;
	sf::Vector2f position;
	sf::Vector2f lastPosition;
	sf::Vector2f velocity;
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
	int k = 1;


	sf::Color getRGBColor(int);
	void setRGBColor();

	sf::RenderWindow* window;
	std::vector<Ball> circles;
	std::vector<sf::CircleShape> shades;
	sf::Color rgbColor;

	std::vector<Arc> arcs;

	void eventUpdate();


	Arc createArc(int, sf::Vector2f, int, int, float);
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
	void arcDeletion();
	void arcsGeneration(int, int, int, sf::Vector2f, float, int);
	bool areArcsExists;




	sf::SoundBuffer CrackBuffer;
	sf::Sound* crack;
	sf::SoundBuffer TouchBuffer;
	sf::Sound* touch;

	sf::Clock beginningTimer;
	bool isStart = 1;
	bool isEnded = 0;
	
};