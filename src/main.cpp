#include "Application.hpp"

int main(int argc, char const *argv[])
{

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_TITLE, sf::Style::Close);
    sf::View view(window.getDefaultView());
    window.setView(view);

    Bird birds[BIRD_NUMBER];

    setup(birds);

    while (true)
    {

        if (update(birds, &window, &view)) break;

        window.setView(view);

        draw(birds, &window);

    }
    



    return 0;
}
