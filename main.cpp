#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <random> 
#include <algorithm>

class Duck {
    private:
        sf::Text name;
        sf::Sprite sprite;
        float speed;
    
    public:
        Duck(sf::Font& font, std::string& stringName, sf::Texture& texture, float initialY) : name(font), sprite(texture) {
            name.setString(stringName);
            name.setFillColor(sf::Color::Black);
            sprite.setPosition({0, initialY});
            name.setPosition({0, initialY - 20.f});
        }

        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
            window.draw(name);
        }

        void setSpeed(float newSpeed) {
            speed = newSpeed;
        }

        void move(float dt) {
            sprite.move({speed * dt, 0.f});
            name.move({speed * dt, 0.f});
        }

        float getX() {
            return sprite.getPosition().x;
        }
};

int main() {
    // random engine
    std::random_device randomSeed;
    std::mt19937 gen(randomSeed());
    std::uniform_real_distribution<float> distrb(100.f, 300.f);

    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "DUCK");
    window.setFramerateLimit(60);
    sf::Clock frameClock;
    sf::Clock randomClock;

    sf::View view({960.f, 540.f}, {1920.f, 1080.f});

    sf::Font font("fonts/times.ttf");

    sf::Texture duckTexture("images/duck.png");

    sf::Texture backgroundTexture("images/background.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({3.6, 3.6});

    sf::Texture finishLineTexture("images/finishLine.png");
    sf::Sprite finishLineSprite(finishLineTexture);
    finishLineSprite.setPosition({5000.f, 0.f});

    std::vector<std::string> ducksNameList = {"tuy", "owij", "tie", "kha", "ta"};
    std::vector<Duck> ducksList;

    for (int index = 0; index < ducksNameList.size(); index++) {
        float initialY = 150.f*index + 300.f;
        ducksList.emplace_back(font, ducksNameList.at(index), duckTexture, initialY);
    }

    for (Duck& duck : ducksList)
        duck.setSpeed(distrb(gen));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = frameClock.restart().asSeconds();

        if (randomClock.getElapsedTime().asSeconds() > 2.f) {
            randomClock.restart();
            for (Duck& duck : ducksList)
                duck.setSpeed(distrb(gen));
        }

        Duck* leader = &ducksList.at(0);
    
        window.clear(sf::Color::White);
        window.draw(finishLineSprite);

        for (Duck& duck : ducksList) {
            duck.move(dt);
            duck.draw(window);

            if (duck.getX() > leader->getX())
                leader = &duck;
        }

        if (leader->getX() > view.getCenter().x)
            view.setCenter({leader->getX(), 540.f});
        window.setView(view);
        window.display();
    }
}