#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <random> 
#include <algorithm>
#include <iostream>

class Duck {
    private:
        sf::Text name;
        sf::Sprite sprite;
        float speed;
    
    public:
        Duck(sf::Font& font, std::string& stringName, sf::Texture& texture, float initialY) : name(font), sprite(texture) {
            name.setString(stringName);
            name.setFillColor(sf::Color::Black);
            name.setPosition({0, initialY - 30.f});

            const float targetHeight = 82.f;
            sf::FloatRect bounds = sprite.getLocalBounds();
            float scaleRatio = targetHeight / bounds.size.y;
            sprite.setScale({scaleRatio, scaleRatio});
            sprite.setPosition({0, initialY});
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

        std::string getName() {
            return name.getString();
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

    bool isRaceEnd = false;
    std::string winnerName;
    float finishLineX = 5000.f;

    sf::View view({960.f, 540.f}, {1920.f, 1080.f});

    sf::Font font("fonts/times.ttf");

    sf::Texture duckTexture("images/duck.png");

    sf::Texture backgroundTexture("images/background.jpg");
    backgroundTexture.setRepeated(true);
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({5.9f, 5.9f});
    backgroundSprite.setTextureRect(sf::IntRect({0, 0}, {6000, 1920}));

    sf::Texture finishLineTexture("images/finishLine.png");
    sf::Sprite finishLineSprite(finishLineTexture);
    finishLineSprite.setPosition({finishLineX, 0.f});

    sf::Text winnerNameText(font);
    winnerNameText.setCharacterSize(100);
    winnerNameText.setFillColor(sf::Color::Red);
    winnerNameText.setPosition({finishLineX - 100.f, 440.f});

    sf::Text leaderPosition(font);
    leaderPosition.setCharacterSize(30);
    leaderPosition.setFillColor(sf::Color::Black);
    leaderPosition.setPosition({900.f, 0.f});

    std::vector<std::string> ducksNameList = {"tuyeen", "loiwj", "tieen", "taan", "khan"};
    std::vector<sf::Texture> avatarTexturesList;
    avatarTexturesList.reserve(ducksNameList.size());
    std::vector<Duck> ducksList;

    for (int index = 0; index < ducksNameList.size(); index++) {
        float initialY = 150.f*index + 100.f;

        avatarTexturesList.emplace_back();
        sf::Texture& avatarTexture = avatarTexturesList.back();
        std::string avatarPath = "avatars/" + ducksNameList.at(index) + ".png";
        if (!avatarTexture.loadFromFile(avatarPath)) {
            std::cerr << "Khong the tai " << avatarPath << ", nen se dung hinh mac dinh" << std::endl;
            // Khong can gia tri tra ve nen ep kieu ve void de tranh canh bao
            (void)avatarTexture.loadFromFile("images/duck.png");
        }
        ducksList.emplace_back(font, ducksNameList.at(index), avatarTexturesList.back(), initialY);
    }

    for (Duck& duck : ducksList)
        duck.setSpeed(distrb(gen));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(backgroundSprite);
        window.draw(finishLineSprite);

        if (isRaceEnd == false) {
            float dt = frameClock.restart().asSeconds();

            if (randomClock.getElapsedTime().asSeconds() > 2.f) {
                randomClock.restart();
                for (Duck& duck : ducksList)
                    duck.setSpeed(distrb(gen));
            }

            Duck* leader = &ducksList.at(0);

            for (Duck& duck : ducksList) {
                duck.move(dt);

                if (duck.getX() > leader->getX())
                    leader = &duck;

                if (duck.getX() > finishLineX) {
                    isRaceEnd = true;
                    winnerName = duck.getName();
                }
            }
            leaderPosition.setPosition({leader->getX(), 0.f});
            leaderPosition.setString(std::to_string(leader->getX()));

            if (leader->getX() > view.getCenter().x)
                view.setCenter({leader->getX(), 540.f});
            window.setView(view);
        }
        else {
            winnerNameText.setString(winnerName);
            window.draw(winnerNameText);
        }

        for (Duck& duck : ducksList) 
            duck.draw(window);

        window.draw(leaderPosition);
        window.display();
    }
}