#include "../headers/project.h"


float getRandom(float min, float max) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}


Project::Project(){

	this->window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Gravity Balls", sf::Style::None);
	this->centr = { 1000, 500 };
	this->countOfCircles = 0;
	this->maxCountOfCircles = 1;
	this->gravity = 800;
	this->h = 0;
	this->RGBTimer = 0;
	this->areArcsExists = 0;
	
	
	if(this->CrackBuffer.loadFromFile("../resources/crack.mp3")) this->crack = new sf::Sound(this->CrackBuffer);
	if(this->TouchBuffer.loadFromFile("../resources/touch.mp3")) this->touch = new sf::Sound(this->TouchBuffer);
}


Project::~Project(){
	delete this->crack;
	delete this->touch;
	delete this->window;
}


void Project::run() {
	while (this->window->isOpen()) {
		this->eventUpdate();

		this->updateInGame();
		this->renderInGame();

	}
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
					circle.velocity = sf::Vector2f(getRandom(500.f, 1000.f), getRandom(500.f, 1000.f));
				}
			}
		}
		
	}
}


void Project::updateInGame() {

	if (this->isStart) {
		this->isStart = 0;
		this->beginningTimer.start();
	}

	this->deltatime = this->time.restart().asSeconds();

	if (!this->areArcsExists) this->arcsGeneration(20, 490, 495, this->centr, 300, 720);

	if (this->countOfCircles < this->maxCountOfCircles) {
		this->createCircle(15);
	}

	this->setRGBColor();

	if (this->beginningTimer.getElapsedTime().asSeconds() >= 3 || this->isEnded) {
		if (this->beginningTimer.getElapsedTime().asSeconds() >= 3) this->isEnded = 1;
		this->arcRotation();
		this->arcCollision();
		this->ballsCollision();
		this->arcDeletion();
	}
}


void Project::renderInGame()
{
	this->window->clear();

	for (auto& el : this->circles) {
		this->window->draw(el.ball);
	}

	for (auto arc : this->arcs) {
		sf::Transform tr;
		tr.rotate(sf::degrees(arc.currentRotationAngle), arc.centr);
		this->window->draw(arc.arc, tr);

	}

	this->window->display();
}


void Project::arcsGeneration(int count, int radius1, int radius2, sf::Vector2f centr, float BaseAngle, int point)
{
	while (count) {
		this->arcs.push_back(this->createArc(point, centr, radius1, radius2, BaseAngle));
		radius1 -= 20;
		radius2 -= 20;
		BaseAngle -= 1;

		count--;
	}
	this->areArcsExists = 1;
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
					float minDist = arc.radius2 + circle.ball.getRadius();
					if (dist < minDist && this->length(circle.lastPosition - arc.centr) >= arc.radius2) {
						sf::Vector2f normal = diff / dist;
						circle.position = arc.centr + normal * minDist;

						float scal = this->Scalar(circle.velocity, normal);
						if (scal < 0) {
							circle.velocity -= (1.f + 1.f) * normal * scal;
						}
						this->touch->play();

					}
					if (dist > maxDist && dist < maxPenetration && this->length(circle.lastPosition - arc.centr) < arc.radius) {
						sf::Vector2f normal = -diff / dist;
						circle.position = arc.centr - normal * maxDist;

						float scal = this->Scalar(circle.velocity, normal);
						if (scal < 0) {
							circle.velocity -= (1.f + 1.f) * normal * scal;
						}
						this->touch->play();

					}



				}
				else {
					if (arc.baseAngle < 360.f) {

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
									circle.velocity -= (1.f + 1.f) * normal * scal;
								}
							}
							};

						checkEdgeCollision(edge1);
						checkEdgeCollision(edge2);
					}
					if (dist > maxDist) {
						this->k = -this->k;

						arc.isBreaked = 1;
						this->crack->play();
					}
				}
				circle.ball.setPosition(circle.position);
			}
		}
	}
}


void Project::arcDeletion()
{
	this->arcs.erase(std::remove_if(this->arcs.begin(), this->arcs.end(), [](Arc& arc) {return arc.isBreaked; }), this->arcs.end());

}


void Project::arcRotation()
{
	for (auto& el : this->arcs) {
		el.currentRotationAngle += this->k * 4 * this->deltatime * el.idx;

		if (el.currentRotationAngle >= 360.f) el.currentRotationAngle -= 360.f;
	}

}


Arc Project::createArc(int point, sf::Vector2f centr, int radius1, int radius2, float BaseAngle)
{
	sf::VertexArray arc(sf::PrimitiveType::TriangleStrip, 2 * point);
	Arc realArc;
	realArc.radius = radius1;
	realArc.radius2 = radius2;
	realArc.baseAngle = BaseAngle;
	realArc.centr = centr;

	int idx = 0;

	float toTopAngle = 90.f - BaseAngle / 2;
	for (int i = 0; i < point; i++) {
		float angle = (BaseAngle * i / (point - 1)) * 3.1415926f / 180.f;

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

	realArc.currentRotationAngle = toTopAngle;
	realArc.arc = arc;
	realArc.idx = this->i++;

	return realArc;
}


void Project::createCircle(int radius)
{
	Ball circle;
	circle.ball = sf::CircleShape(radius);
	sf::Color col(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	circle.ball.setFillColor(col);
	circle.ball.setOrigin({ (float)radius, (float)radius });
	circle.ball.setPointCount(512);
	circle.position = { getRandom(this->centr.x - 10, this->centr.x + 10), getRandom(this->centr.y - 10, this->centr.y + 10) };
	circle.ball.setPosition(circle.position);
	this->countOfCircles++;
	this->circles.push_back(circle);

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

