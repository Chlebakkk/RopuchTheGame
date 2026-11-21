#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <random>

class Ropuch {
private:
    sf::Vector2f position;
    sf::RectangleShape shape; //kwadratowy ropuch xd
    float speed;

public:
    Ropuch(float startX, float startY) {
        position = { startX, startY };
        speed = 50.0f; //skok

        shape.setSize({ 40.0f, 40.0f }); //grubość ropucha
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(position);
    }

    void update() {
        shape.setPosition(position);
    }

    //chodzenie
    void handleInput(sf::Keyboard::Key key) {
        if ((key == sf::Keyboard::Key::W || key == sf::Keyboard::Key::Up) && position.y - speed >= 0.0f) {
            position.y -= speed;
        }
        else if ((key == sf::Keyboard::Key::S || key == sf::Keyboard::Key::Down) && position.y + speed < 600.0f) {
            position.y += speed;
        }
        else if ((key == sf::Keyboard::Key::A || key == sf::Keyboard::Key::Left) && position.x - speed >= 0.0f) {
            position.x -= speed;
        }
        else if ((key == sf::Keyboard::Key::D || key == sf::Keyboard::Key::Right) && position.x + speed < 800.0f) {
            position.x += speed;
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
};



class Obstacle {
private:
    sf::Vector2f position;
    sf::RectangleShape shape;
    float speed;

public:
    Obstacle(float startX, float startY, float spd) {
        position = { startX, startY };
        speed = spd;
        shape.setSize({ 70.0f, 40.f });
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
    }

    void update() {
        position.x -= speed;

        if (position.x < -70.0f) {
            position.x = 800.0f;
        }
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
};



class Lane {
private:
    float yPosition;
    sf::RectangleShape background;
    std::vector<Obstacle> obstacles;

public:
    Lane(float y, sf::Color color) {
        yPosition = y;

        background.setSize({ 800.0f, 50.0f });
        background.setPosition({ 0.0f, yPosition });
        background.setFillColor(color);
    }

    void addObstacle(float startX, float speed) {
        obstacles.emplace_back(startX, yPosition + 5.0f, speed);
    }

    void update() {
        for (auto& obs : obstacles) {
            obs.update();
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(background);
        for (const auto& obs : obstacles) {
            obs.draw(window);
        }
    }
};



class World {
private:
    std::vector<Lane> lanes;
    Ropuch player;

public:
    World() : player(380.0f, 555.0f) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::discrete_distribution<> carCountDist({2,1,1,1}); //ilość aut {2x0, 1x1, 1x2, 1x3}
        std::uniform_real_distribution<> speedDist(1.5f, 4.5f); //Prędkość
        std::uniform_real_distribution<> startOffsetDist(0.0f, 250.0f); //Pozycja start

        for (int i = 0; i < 12; i++) {
            sf::Color color = (i % 2 == 0) ? sf::Color(50, 50, 50) : sf::Color(100, 100, 100);
            lanes.emplace_back(i * 50.0f, color);

            if (i>0 && i<11) {
                int numCars = carCountDist(gen);
                if (numCars > 0) {
                    float laneSpeed = speedDist(gen);
                    float startOffset = startOffsetDist(gen);

                    float spacing = 870.0f / numCars;
                    for (int j = 0; j < numCars; j++) {
                        float carX = startOffset + (j * spacing);
                        lanes.back().addObstacle(carX, laneSpeed);
                    }
                }
            }
        } 
    }

    void update() {
        player.update();
        for (auto& lane : lanes) {
            lane.update();
        }
    }

    void draw(sf::RenderWindow& window) const {
        for (const auto& lane : lanes) {
            lane.draw(window);
        }
        player.draw(window); //Stawiam ropucha "na" pasach- taki podział na warstwy
    }

    void handleInput(sf::Keyboard::Key key) {
        player.handleInput(key);
    }
};


int main() {
    //wymiary gry (800x600)
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Ropuch - Projekt C++");
    window.setFramerateLimit(60); //FPScap

    World gameWorld;

    while (window.isOpen()) {
        //obsługa zdarzeń
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                gameWorld.handleInput(keyPressed->code);
            }
        }

        //zmiana stanu gyr w czasie rzeczywistym
        gameWorld.update();

        //renderowanie
        window.clear();
        gameWorld.draw(window);
        window.display();
    }

    return 0;
}