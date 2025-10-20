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

    sf::Font TimesNewRomanFont("fonts/times.ttf");
    sf::Texture duckTexture("images/duck.png");

    sf::Texture backgroundTexture("images/background.jpg");
    backgroundTexture.setRepeated(true);
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({5.9f, 5.9f});
    backgroundSprite.setTextureRect(sf::IntRect({0, 0}, {6000, 1920}));

    sf::Texture finishLineTexture("images/finishLine.png");
    sf::Sprite finishLineSprite(finishLineTexture);
    finishLineSprite.setPosition({finishLineX, 0.f});

    sf::Text winnerNameText(TimesNewRomanFont);
    winnerNameText.setCharacterSize(100);
    winnerNameText.setFillColor(sf::Color::Red);
    winnerNameText.setPosition({finishLineX - 100.f, 440.f});

    // Khi chỉnh kích thước của thanh tiến độ thì chỉ cần chỉnh của progressBarBG vì các đối tượng khác đã được thiết kế để phụ thuộc vào kích cỡ của nó
    sf::RectangleShape progressBarBG({800.f, 30.f});
    progressBarBG.setFillColor(sf::Color(160, 160, 160));
    progressBarBG.setPosition({560.f, 20.f});
    sf::RectangleShape progressBarFill({0.f, 30.f});
    progressBarFill.setFillColor(sf::Color::Green);
    progressBarFill.setPosition({560.f, 20.f});

    sf::Text progressPercentText(TimesNewRomanFont);
    progressPercentText.setCharacterSize(30);
    progressPercentText.setFillColor(sf::Color::Black);
    progressPercentText.setPosition({1380.f, 20.f});
    float progressPercent = 0.f;
    std::string progressPercentString;

    std::vector<std::string> ducksNameList = {"tuyeen", "loiwj", "tieen", "taan", "khan"};
    std::vector<sf::Texture> avatarTexturesList;
    // dòng này rất cần thiết vì nếu dữ liệu trong vector vượt quá sức chứa, nó sẽ thay đổi vị trí bộ nhớ khiến cho sprite không trỏ đến địa chỉ texture mới, khiến hình ảnh không được hiện thị
    avatarTexturesList.reserve(ducksNameList.size());
    std::vector<Duck> ducksList;

    for (int index = 0; index < ducksNameList.size(); index++) {
        float initialY = 150.f*index + 100.f;
        avatarTexturesList.emplace_back();
        sf::Texture& avatarTexture = avatarTexturesList.back();
        std::string avatarPath = "avatars/" + ducksNameList.at(index) + ".png";
        if (!avatarTexture.loadFromFile(avatarPath)) {
            std::cerr << "Khong the tai " << avatarPath << ", nen se dung hinh mac dinh" << std::endl;
            // Không cần giá trị trả về nên ép kiểu về void để tránh cảnh báo
            (void)avatarTexture.loadFromFile("images/duck.png");
        }
        ducksList.emplace_back(TimesNewRomanFont, ducksNameList.at(index), avatarTexturesList.back(), initialY);
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

            progressPercent = leader->getX() / finishLineX;
            progressBarFill.setSize({progressBarBG.getGlobalBounds().size.x * progressPercent, 30.f});
            progressPercentString = std::to_string(static_cast<int>(progressPercent * 100.f)) + "%";
            progressPercentText.setString(progressPercentString);

            // Dòng này để ngăn việc mới bắt đầu đua thì camera đã tập trung vào góc trái, thêm dòng này thì khi con vịt đầu tiên đi quá nửa màn hình thì góc nhìn mới di chuyển theo
            if (leader->getX() > view.getCenter().x)
                view.setCenter({leader->getX(), 540.f});
            window.setView(view);
            
            progressBarBG.setPosition({view.getCenter().x - 400.f, 20.f});
            progressBarFill.setPosition({view.getCenter().x - 400.f, 20.f});
            progressPercentText.setPosition({view.getCenter().x + progressBarBG.getGlobalBounds().size.x / 2 + 20.f, 20.f});
        }
        else {
            winnerNameText.setString(winnerName);
            window.draw(winnerNameText);
        }

        window.draw(progressBarBG);
        window.draw(progressBarFill);
        window.draw(progressPercentText);

        for (Duck& duck : ducksList) 
            duck.draw(window);

        window.display();
    }
}