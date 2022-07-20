#include <SFML/Graphics.hpp>
#include <sstream>   
#include <iostream>
#include <fstream>

#include "Vec2.h"
class Shape
{
public:
    Shape() = default;
    Shape(const std::string& shape_value)
    {
        FromString(shape_value);
    }
    std::string name = {};
    std::vector<int> color = {};
    vec2 position = {};
    vec2 velocity = {};
    vec2 dimension = {}; //Use the y component as radius for Circle else use (x,y) as w, h for rect
    sf::Text text{};

    bool rect = true;
private:
    void FromString(const std::string& shape_value)
    {
        std::istringstream iss(shape_value);
        int r{}, g{}, b{};
        std::string shape;
        iss >> shape;
        iss >> name >> position.x >> position.y >> velocity.x >> velocity.y >> r >> g >> b;
        color = { r, g, b };
        if (shape == "Circle")
        {
            iss >> dimension.x;
            rect = false;
        }
        else {
            iss >> dimension.x >> dimension.y;
        }
        text.setPosition({ position.x / 2.f, position.y / 2.f });
        text.setString(name);
    }
};
struct _Font
{
    std::string location{};
    int size{};
    int color[3]{};
    sf::Font actual_font;
};
std::vector<Shape> parse_config(std::string& file_name, _Font& font, vec2& dim)
{

    std::fstream input_file(file_name);
    //parse the window size;
    std::string value, temp;
    std::getline(input_file, value);
    std::istringstream iss(value);
    iss >> temp >> dim.x >> dim.y;
    //parse Font;
    std::getline(input_file, value);
    std::istringstream font_iss(value);
    font_iss >> temp >> font.location >> font.size >> font.color[0]
        >> font.color[1] >> font.color[2];
    sf::Font my_font;
    if (!my_font.loadFromFile(font.location))
    {
        std::cerr << "Failed to load font in " << font.location;
    }
    font.actual_font = my_font;
    std::vector<Shape> shapes;
    if (input_file.good())
    {
        while (std::getline(input_file, value))
        {
            shapes.emplace_back(Shape(value));
        }
    }
    return shapes;
}
void if_shape_out_of_bound(const vec2& window_dim, Shape& shape)
{
    bool touched_wall = false;
    if (shape.rect)
    {
        //check if the rectangle's left&right side touched the wall
        if (shape.position.x < 0 || shape.position.x > window_dim.x)
        {
            shape.velocity.x *= -1.0f;
        }
        //for top & bottom
        if (shape.position.y< 0 || shape.position.y > window_dim.y)
        {
            shape.velocity.y *= -1.0f;
        }
    }
    else
    {
        //check if the bounding box of the cirlce is touching any side
        //left & right side of window
        vec2 bounding_box = shape.position + shape.dimension;
        if (bounding_box.x < 0 || bounding_box.x>window_dim.x)
        {
            shape.velocity.x *= -1.0f;
        }
        if (bounding_box.y < 0 || bounding_box.y > window_dim.y)
        {
            shape.velocity.y *= -1.0f;
        }
    }
}
void draw(std::vector<Shape>& shapes, sf::RenderWindow& window
    , _Font font)
{
    for (auto& v : shapes)
    {
        v.text.setCharacterSize(font.size);
        v.text.setFillColor(sf::Color(font.color[0], font.color[1], font.color[2]));
        v.text.setFont(font.actual_font);
        if (v.rect)
        {

            sf::RectangleShape shape(sf::Vector2f(v.dimension.x, v.dimension.y));
            shape.setPosition(v.position.x, v.position.y);
            shape.setFillColor(sf::Color(v.color[0], v.color[1], v.color[2]));
            window.draw(shape);
        }
        else
        {
            sf::CircleShape shape(v.dimension.x);
            shape.setPosition(v.position.x, v.position.y);
            shape.setFillColor(sf::Color(v.color[0], v.color[1], v.color[2]));
            window.draw(shape);
        }
        window.draw(v.text);
    }
}
void update_shape_position(const vec2& dim, std::vector<Shape>& shapes)
{
    //update the postion of a shape based on its velocity
    for (auto& shape : shapes)
    {
        if_shape_out_of_bound(dim, shape);
        shape.position += shape.velocity;
        shape.text.setPosition(shape.position.x, shape.position.y);
    }
}
#if 0
int main()
{

    // create the window

    std::string file = "config.txt";
    vec2 window_dim;
    _Font font;
    auto shapes = parse_config(file, font, window_dim);

    sf::RenderWindow window(sf::VideoMode(window_dim.x, window_dim.y), "bs");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // clear the window with black color
        window.clear(sf::Color::Black);
        update_shape_position(window_dim, shapes);
        draw(shapes, window, font);
        window.display();
    }
    return 0;
}
#endif