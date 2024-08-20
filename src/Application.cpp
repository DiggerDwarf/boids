#include "Application.hpp"

sf::Vector2f rotated(const sf::Vector2f& v, const float a)
{
    float s = sinf(a), c = cosf(a);
    return {
        -v.y*s+v.x*c,
        v.y*c+v.x*s
    };
}
void rotate(sf::Vector2f& v, const float a)
{
    v = rotated(v, a);
}

void setup( Bird ( & birds )[ BIRD_NUMBER ] )
{
    srand(time(nullptr));
    for (int birdId = 0; birdId < BIRD_NUMBER; birdId++)
    {
        birds[ birdId ].position = sf::Vector2f(rand()%WIN_WIDTH, rand()%WIN_HEIGHT);
        float randangle = ((rand()%6282) * 0.001F)-M_PI;
        birds[ birdId ].direction    = {cosf(randangle), sinf(randangle)};
    }
}

bool update( Bird ( & birds )[ BIRD_NUMBER ], sf::RenderWindow* window, sf::View* view )
{
    sf::Event event;
    bool out = false;
    // sf::Vector2i mPos = sf::Mouse::getPosition();
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
            out = true;
            break;

        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
            {
                window->close();
                out = true;
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                setup(birds);
            }
            
            break;
        case sf::Event::MouseWheelScrolled:
            view->zoom(exp(event.mouseWheelScroll.delta));
            break;
        }
    }

    {
        sf::Vector2f offset;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up   )) offset.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down )) offset.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left )) offset.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) offset.x += 1;
        view->move(offset*view->getSize().x * 0.002F);
    }

    sf::Vector2f localGroupAveragePosition;
    sf::Vector2f localGroupAverageDirection;
    float localGroupSize;
    float btod;
    float positionSteer;
    float directionSteer;
    float avoidSteer;
    Bird* closest;
    float closestDist;

    for (Bird& bird : birds)
    {
        closestDist = HUGE_VALF;
        closest = &bird;
        localGroupSize = 0;
        localGroupAveragePosition = {0,0};
        localGroupAverageDirection = {0,0};
        avoidSteer = 0;
        for (Bird& otherBird : birds)
        {
            if (isSame(bird, otherBird)) continue;
            btod = length(otherBird.position - bird.position);
            if (btod > LOCAL_GROUP_RADIUS) continue;
            localGroupSize++;
            localGroupAveragePosition += otherBird.position;
            localGroupAverageDirection += otherBird.direction;
            if (btod < closestDist)
            {
                closest = &otherBird;
                closestDist = btod;
            }
            // if (btod > AVOID_RADIUS) continue;

        }

        bird.position += bird.direction * FLY_SPEED;

        if (localGroupSize == 0) continue;

        if (closest != &bird && length(closest->position - bird.position) < AVOID_RADIUS)
        {
            avoidSteer = ((__signbitf(angle_to(bird.direction, closest->position - bird.position)) *2) -1) * AVOID_STRENGTH;
        }
        else avoidSteer = 0;
        

        localGroupAveragePosition /= localGroupSize;
        localGroupAveragePosition = localGroupAveragePosition - bird.position;

        positionSteer = -((__signbitf(angle_to(bird.direction, localGroupAveragePosition)) *2) -1) * AVERAGE_POS_STRENGTH;

        // localGroupAverageDirection /= localGroupSize;
        
        directionSteer = -((__signbitf(angle_to(bird.direction, localGroupAverageDirection)) *2) -1) * AVERAGE_DIR_STRENGTH;
        
        rotate(bird.direction, positionSteer + directionSteer + avoidSteer);
    }   

    return out;
}

void draw( Bird ( & birds )[ BIRD_NUMBER ], sf::RenderWindow* window )
{
    window->clear();

    {
        sf::CircleShape big(LOCAL_GROUP_RADIUS);
        big.setFillColor(sf::Color(0xaa2222ff));
        big.setPosition(500,500);
        window->draw(big);
        big.setFillColor(sf::Color(0xaaee22ff));
        big.setRadius(AVOID_RADIUS);
        window->draw(big);
    }

    sf::CircleShape birb(10.0F);
    birb.setFillColor(sf::Color(0xffffffff));
    for (int birdId = 0; birdId < BIRD_NUMBER; birdId++)
    {
        birb.setPosition(birds[ birdId ].position);
        window->draw(birb);
    }

    window->display();
}
