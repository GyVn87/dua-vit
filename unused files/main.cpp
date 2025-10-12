#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

class Duck {
    sf::Text name;
    sf::Sprite sprite;

public:
    // Khởi tạo các thành viên `name` và `sprite` trong danh sách khởi tạo
    Duck(sf::Font& font, sf::Texture& texture, const std::string& str, float y)
        : name(font), // Khởi tạo `name`
          sprite(texture)      // Khởi tạo `sprite`
    {
        // Bây giờ, `name` và `sprite` đã được khởi tạo và có thể thiết lập các thuộc tính khác
        
        // setup text
        name.setFillColor(sf::Color::Black);
        name.setPosition({0.f, y - 50.f});

        // setup sprite
        sprite.setScale({0.1f, 0.1f});
        sprite.setPosition({0.f, y});
    }

    void move(float dx, float dy) {
        sprite.move({dx, dy});
        name.move({dx, dy});
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
        window.draw(name);
    }

    float getX() const { return sprite.getPosition().x; }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Duck Race");
    window.setFramerateLimit(60);

    sf::Clock clock;

    // load resources
    sf::Font font;
    if (!font.openFromFile("times.ttf"))
        return -1;

    sf::Texture duckTexture;
    if (!duckTexture.loadFromFile("duck.png"))
        return -1;

    // duck names
    std::vector<std::string> ducksNameList = {"Tuyenn", "Lowij", "tie", "uit"};
    std::vector<Duck> ducks;

    // create ducks
    for (int i = 0; i < ducksNameList.size(); i++) {
        ducks.emplace_back(font, duckTexture, ducksNameList[i], 150.f * i + 150.f);
    }

    float speed = 300.f;

    // main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // update ducks
        for (auto& duck : ducks) {
            duck.move(speed * dt, 0.f);
        }

        // render
        window.clear(sf::Color::White);
        for (const auto& duck : ducks) {
            duck.draw(window);
        }
        window.display();
    }

    return 0;
}
