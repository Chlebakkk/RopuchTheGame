#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

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

    //reagowanie na klawisze
    void update() {
        //tu bedzie wsad
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

public:
    Lane(float y, sf::Color color) {
        yPosition = y;

        background.setSize({ 800.0f, 50.0f });
        background.setPosition({ 0.0f, yPosition });
        background.setFillColor(color);
    }

    void update() {
        //tu bedą zagrożenia
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(background);
    }
};



class World {
private:
    std::vector<Lane> lanes;
    Ropuch player;

public:
    World() : player(380.0f, 555.0f) {
        //murawa dla testu
        for (int i = 0; i < 12; i++) {
            sf::Color color = (i % 2 == 0) ? sf::Color(50, 50, 50) : sf::Color(100, 100, 100);
            lanes.emplace_back(i * 50.0f, color);
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