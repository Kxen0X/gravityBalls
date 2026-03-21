#include "../headers/project.h"

float getRandom(float min, float max) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}
Project::Project(){

	this->state = State::GAME;
	this->window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Gravity Balls", sf::Style::None);
	this->centr = { 1000, 500 };
	Arc temp = this->createArc(500, this->centr, 490, 500, 380, 0);
	this->arcs.push_back(temp);
	temp = this->createArc(500, this->centr, 430, 440, 320, 0);
	this->arcs.push_back(temp);
	temp = this->createArc(500, this->centr, 370, 380, 280, 0);
	this->arcs.push_back(temp);
	this->countOfCircles = 0;
	this->maxCountOfCircles = 1;
	this->gravity = 700;
	this->h = 0;
	this->RGBTimer = 0;
	this->sword.loadFromFile("../resources/sword2.png");
	
	this->swordSprite1 = new sf::Sprite(this->sword);
	this->swordSprite1->setScale({ 2, 2 });
	this->font.openFromFile("../resources/VeniteAdoremus-rgRBA.ttf");
	this->text = new sf::Text(this->font);
	this->text->setCharacterSize(76);
	this->text->setString("PC DEATH");
	this->text->setPosition({ 750, 100 });
	this->ButtonText = new sf::Text(this->font);
	this->ButtonText->setCharacterSize(76);
	this->ButtonText->setString("PLAY");
	this->ButtonText->setPosition({ 850, 350 });
	this->ExitText = new sf::Text(this->font);
	this->ExitText->setCharacterSize(76);
	this->ExitText->setString("EXIT");
	this->ExitText->setPosition({ 850, 750 });
	this->button.setSize({ 400, 200 });
	this->button.setOrigin({ 200, 100 });
	this->button.setPosition({960, 400});
	this->button.setOutlineColor(sf::Color::White);
	this->button.setFillColor(sf::Color::Black);
	this->button.setOutlineThickness(3);
	this->exit.setSize({ 400, 200 });
	this->exit.setOrigin({ 200, 100 });
	this->exit.setPosition({960, 800});
	this->exit.setOutlineColor(sf::Color::White);
	this->exit.setFillColor(sf::Color::Black);
	this->exit.setOutlineThickness(3);
	this->txt = new sf::Texture("../resources/a.png");
	
	this->death = new sf::Sprite(*this->txt);
	this->death->setScale({ (float)this->window->getSize().x / this->txt->getSize().x, (float)this->window->getSize().y / this->txt->getSize().y});
	this->timer = 10;
	this->youLose = new sf::Text(this->font);
	this->youLose->setString("YOU LOSE");
	this->youLose->setCharacterSize(76);
	this->youLose->setPosition({ 750, 350 });
	this->timerText = new sf::Text(this->font);
	this->timerText->setCharacterSize(76);
	this->timerText->setPosition({ 920, 750 });
}


Project::~Project(){
	delete this->ButtonText;
	delete this->txt;
	delete this->ExitText;
	delete this->youLose;
	delete this->timerText;
	delete this->death;
	delete this->text;
	delete this->swordSprite1;
	delete this->window;
}
void Project::eventUpdate()
{
	while (const auto& event = this->window->pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			this->window->close();
		}

		if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
			if (k->scancode == sf::Keyboard::Scancode::Escape) {
				this->window->close();
			}
			if (k->scancode == sf::Keyboard::Scancode::B) {
				for (auto& circle : this->circles) {
					circle.velocity = sf::Vector2f(getRandom(-500.f, 500.f), getRandom(-500.f, 500.f));
				}
			}
		}
		if (event->is<sf::Event::MouseButtonPressed>()) {
			if (this->state == State::MENU) {
				this->MousePos = this->window->mapPixelToCoords(sf::Mouse::getPosition());
				if (this->button.getGlobalBounds().contains(this->MousePos)) {
					this->state = State::GAME;
				}
				if (this->exit.getGlobalBounds().contains(this->MousePos)) {
					this->window->close();
				}
			}
		}
	}
}

void Project::updateInEnd(){
	if (this->out.getElapsedTime().asSeconds() >= 1) {
		this->timer--;
		this->timerText->setString(std::to_string(this->timer));
		this->out.restart();
	}
	
}
void Project::renderInEnd(){
	this->window->clear();
	if (this->timer <= -2) {
		this->window->draw(*death);
	}
	else if(this->timer > -2 && this->timer <= 0) {
		this->timerText->setString("7");
		this->window->draw(*this->youLose);
		this->window->draw(*this->timerText);
		this->window->display();
		this->timerText->setString("0");
		this->window->draw(*this->youLose);
		this->window->draw(*this->timerText);
	}
	else {
		this->window->draw(*this->youLose);
		this->window->draw(*this->timerText);
	}
	this->window->display();
}



void Project::run() {
	while (this->window->isOpen()) {
		this->eventUpdate();
		/*if (this->state == State::MENU) {
			this->renderInMenu();
		}*/
		if (this->state == State::GAME) {
			this->updateInGame();
			this->renderInGame();
		}
		/*if (this->state == State::END) {
			this->out.start();
			this->updateInEnd();
			this->renderInEnd();
		}*/
	}
}



void Project::renderInMenu() {
	this->window->clear();
	this->window->draw(this->button);
	this->window->draw(this->exit);
	this->window->draw(*this->text);
	this->window->draw(*this->ButtonText);
	this->window->draw(*this->ExitText);
	this->window->display();
}

void Project::renderInGame()
{
	this->window->clear(sf::Color(101, 101, 101));
	for (auto& el : this->circles) {
		this->window->draw(el.ball);
		//this->window->draw(el.swordBorders);
	}
	for (auto arc : this->arcs) {
		sf::Transform tr;
		tr.rotate(sf::degrees(arc.currentRotationAngle), arc.centr);
		this->window->draw(arc.arc, tr);

	}
	//this->window->draw(*this->swordSprite1);
	this->window->display();
}

void Project::createCircle(int radius)
{
	Ball circle;
	circle.ball = sf::CircleShape(radius);
	sf::Color col(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	circle.ball.setFillColor(col);
	circle.ball.setOrigin({ (float)radius, (float)radius });
	circle.ball.setPointCount(512);
	circle.position = { 900, 100};
	/*circle.sword = this->swordSprite1;
	circle.swordBorders = this->swordBorders;
	circle.swordBorders.setPosition({ circle.position.x+100, circle.position.y - 131 });
	circle.swordBorders.setSize({30, 181});
	circle.swordBorders.setRotation(sf::degrees(30));
	circle.swordBorders.setFillColor(sf::Color::Blue);*/
	//circle.swordBorders.setTexture(&this->sword);
	//circle.swordBorders.setTextureRect(sf::IntRect({10, 0}, {35, 65}));
	this->countOfCircles++;
	this->circles.push_back(circle);

	

}

Arc Project::createArc(int point, sf::Vector2f centr, int radius1, int radius2, float BaseAngle, float raz)
{
	sf::VertexArray arc(sf::PrimitiveType::TriangleStrip, 2 * point);
	Arc realArc;
	realArc.radius = radius1;
	realArc.radius2 = radius2;
	realArc.baseAngle = BaseAngle;
	realArc.centr = centr;

	int idx = 0;
	for (int i = 0; i < point; i++) {
		float angle = (0 + BaseAngle * i / (point + 1)) * 3.1415926f / 180.f;

		float x1 = centr.x + radius1 * std::cos(angle);
		float y1 = centr.y + radius1 * std::sin(angle);

		float x2 = centr.x + radius2 * std::cos(angle);
		float y2 = centr.y + radius2 * std::sin(angle);

		arc[idx].position = { x1, y1 };
		arc[idx].color = sf::Color(117, 66, 137);

		arc[++idx].position = { x2, y2 };
		arc[idx].color = sf::Color(117, 66, 137);
		idx++;
	}
	
	realArc.currentRotationAngle = 0;
	realArc.arc = arc;
	realArc.idx = this->i;
	this->i ++;
	return realArc;
}



sf::Color Project::getRGBColor(int h)
{
	float r = 0, g = 0, b = 0;

	float h_f = static_cast<float>(h);
	float x = 1.0f * (1.0f - std::abs(std::fmod(h_f / 60.0f, 2.0f) - 1.0f));

	if (h >= 0 && h < 60) {
		r = 1; g = x; b = 0;
	}
	else if (h >= 60 && h < 120) {
		r = x; g = 1; b = 0;
	}
	else if (h >= 120 && h < 180) {
		r = 0; g = 1; b = x;
	}
	else if (h >= 180 && h < 240) {
		r = 0; g = x; b = 1;
	}
	else if (h >= 240 && h < 300) {
		r = x; g = 0; b = 1;
	}
	else if (h >= 300 && h <= 360) {
		r = 1; g = 0; b = x;
	}

	return sf::Color(
		static_cast<std::uint8_t>(r * 255),
		static_cast<std::uint8_t>(g * 255),
		static_cast<std::uint8_t>(b * 255)
	);
}

void Project::updateInGame() {
	
	this->deltatime = this->time.restart().asSeconds();
	if (this->countOfCircles < this->maxCountOfCircles) this->createCircle(20);
	this->arcRotation();
	this->arcCollision();
	//this->swordCollision();
	this->ballsCollision();
	//this->mouseCollision();
	//this->setRGBColor();
	


	
}

void Project::swordCollision()
{
	for (int i = 0; i < this->circles.size(); i++) {
		for (int j = i + 1; j < this->circles.size(); j++) {
			if (this->circles[j].ball.getGlobalBounds().findIntersection(this->circles[i].swordBorders.getGlobalBounds())) {
				this->circles.erase(this->circles.begin() + j);
			}
		}
	}
}

void Project::setRGBColor()
{
	if (this->RGBTimer % 100 == 0) {
		this->rgbColor = this->getRGBColor(this->h);
		for (auto& arc : this->arcs) {


			for (int i = 0; i < arc.arc.getVertexCount(); i++) {
				arc.arc[i].color = this->rgbColor;
			}
		}
		
		for (auto& circle : this->circles) {
			circle.ball.setFillColor(this->rgbColor);
		}
		this->h = (this->h + 1) % 360;
	}
	this->RGBTimer++;
	if (this->RGBTimer > 5000) this->RGBTimer = 0;
}


float Project::length(sf::Vector2f v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}


sf::Vector2f Project::normalize(sf::Vector2f v)
{
	float length = this->length(v);
	return length != 0 ? v / length : v;
}


float Project::Scalar(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}




void Project::arcCollision()
{
	float subDeltaT = this->deltatime / 10.f;

	for (int i = 0; i < 10; i++) {
		for (auto& circle : this->circles) {
			circle.lastPosition = circle.position;
			
			circle.velocity.y += gravity * subDeltaT;
			circle.position += circle.velocity * subDeltaT;
			circle.ball.setPosition(circle.position);
			//circle.swordBorders.setPosition({ circle.position.x + 100, circle.position.y - 131 });

		}

		for (auto& arc : this->arcs) {
			for (auto& circle : this->circles) {
				sf::Vector2f diff = circle.position - arc.centr;
				float dist = this->length(diff);
				float maxDist = arc.radius - circle.ball.getRadius();

				float angleRad = std::atan2(diff.y, diff.x);
				float ballAngle = angleRad * 180.f / 3.14159265f;
				if (ballAngle < 0) ballAngle += 360.f;

				float relativeAngle = ballAngle - arc.currentRotationAngle;

				while (relativeAngle < 0) relativeAngle += 360.f;
				while (relativeAngle >= 360) relativeAngle -= 360.f;

				bool isInsideArcSector = (arc.baseAngle >= 360.f) || (relativeAngle <= arc.baseAngle);
				if (isInsideArcSector) {
					float maxPenetration = arc.radius + circle.ball.getRadius() * 2.0f;
					float minDist = arc.radius + circle.ball.getRadius();
					if (dist < minDist && this->length(circle.lastPosition - arc.centr) >= arc.radius) {
						sf::Vector2f normal = diff / dist;
						circle.position = arc.centr + normal * minDist;

						float scal = this->Scalar(circle.velocity, normal);
						if (scal < 0) {
							circle.velocity -= (1.f + 1.f) * normal * scal;
						}
					
					}
					if (dist > maxDist && dist < maxPenetration && this->length(circle.lastPosition - arc.centr) < arc.radius) {
						sf::Vector2f normal = -diff / dist;
						circle.position = arc.centr - normal * maxDist;

						float scal = this->Scalar(circle.velocity, normal);
						if (scal < 0) {
							circle.velocity -= (1.f + 1.f) * normal * scal;
						}

					}
					
					
				}
				else if (arc.baseAngle < 360.f) {

					float startRad = arc.currentRotationAngle * 3.14159265f / 180.f;
					sf::Vector2f edge1 = arc.centr + sf::Vector2f(std::cos(startRad), std::sin(startRad)) * arc.radius;

					float endRad = (arc.currentRotationAngle + arc.baseAngle) * 3.14159265f / 180.f;
					sf::Vector2f edge2 = arc.centr + sf::Vector2f(std::cos(endRad), std::sin(endRad)) * arc.radius;
					
					auto checkEdgeCollision = [&](sf::Vector2f edgePoint) {
						sf::Vector2f d = circle.position - edgePoint;
						float dLen = this->length(d);

						if (dLen < circle.ball.getRadius() && dLen != 0) {
							sf::Vector2f normal = d / dLen;
							circle.position = edgePoint + normal * circle.ball.getRadius();

							float scal = this->Scalar(circle.velocity, normal);
							if (scal < 0) {
								circle.velocity -= (1.f + 0.6f) * normal * scal;
							}
						}
						};

					checkEdgeCollision(edge1);
					checkEdgeCollision(edge2);
				}

				circle.ball.setPosition(circle.position);
			}
		}
	}
}

void Project::ballsCollision() {
	for (int j = 0; j < this->circles.size(); j++) {
		for (int k = j + 1; k < this->circles.size(); k++) {
			sf::Vector2f diff = circles[j].position - circles[k].position;
			float dist = this->length(diff);
			float minDistance = circles[j].ball.getRadius() + circles[k].ball.getRadius();

			if (dist < minDistance) {

				float overlap = minDistance - dist;
				sf::Vector2f normal = diff / dist;

				circles[j].position += normal * (overlap / 2.f);
				circles[k].position -= normal * (overlap / 2.f);

				float v1n = this->Scalar(circles[j].velocity, normal);
				float v2n = this->Scalar(circles[k].velocity, normal);

				if (v1n - v2n < 0) {
					float restitution = 1.f;

					sf::Vector2f v1n_vec = normal * v1n;
					sf::Vector2f v2n_vec = normal * v2n;

					circles[j].velocity -= (1.f + restitution) * (v1n - v2n) * 0.5f * normal;
					circles[k].velocity += (1.f + restitution) * (v1n - v2n) * 0.5f * normal;
				}
			}
		}
	}
}

/*void Project::mouseCollision()
{
	this->MousePos = this->window->mapPixelToCoords(sf::Mouse::getPosition());
	for (auto& el : this->circles) {
		if (this->length(el.position - this->MousePos) < el.ball.getRadius()) {
			this->state = State::END;
		}
	}

	if (this->length(this->MousePos - this->centr) > 500) {
		sf::Mouse::setPosition({ 600 + rand() % 300, 100 + rand() % 500});
	}

}*/


void Project::arcRotation()
{
	for (auto& el : this->arcs) {
		el.currentRotationAngle += 60 * this->deltatime * 3/el.idx;
		
		if (el.currentRotationAngle >= 360.f) el.currentRotationAngle -= 360.f;
	}

}