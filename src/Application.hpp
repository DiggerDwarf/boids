#ifndef APPLICATION_INCLUDED
#define APPLICATION_INCLUDED

#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <math.h>


#define BIRD_NUMBER             250
#define FLY_SPEED               0.8F
#define LOCAL_GROUP_RADIUS      150
#define AVERAGE_POS_STRENGTH    0.03F
#define AVERAGE_DIR_STRENGTH    0.01F
#define AVOID_RADIUS            50
#define AVOID_STRENGTH          0.06F

#define WIN_WIDTH  1000
#define WIN_HEIGHT 1000
#define WIN_TITLE "Boids."


struct Bird
{
    sf::Vector2f  position;
    sf::Vector2f direction;
};
// bool operator==(const Bird& bird1, const Bird& bird2)
// {
//     return bird1.position == bird2.position && bird1.angle == bird2.angle;
// }

template <typename T1, typename T2>
constexpr bool isSame(const T1& obj1, const T2& obj2)
{
    return &obj1 == &obj2;
}

template <typename T>
constexpr float dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
    return (v1.x*v2.x) + (v1.y*v2.y);
}

constexpr float angle(const sf::Vector2f& v)
{
    return atan2f(v.y,v.x);
}
constexpr float angle_to(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return atan2f((v2.y*v1.x) - (v2.x*v1.y), (v2.x, v1.x) + (v2.y,v1.y));
}

template <typename T>
constexpr float length(const sf::Vector2<T>& v)
{
    return sqrt((v.x*v.x)+(v.y*v.y));
}

template <typename T>
constexpr sf::Vector2<T> operator/(const T& val, const sf::Vector2<T>& vec)
{
    return sf::Vector2<T>(val/vec.x, val/vec.y);
}

void setup( Bird ( & birds )[ BIRD_NUMBER ] );

bool update( Bird ( & birds )[ BIRD_NUMBER ], sf::RenderWindow* window, sf::View* view);

void draw( Bird ( & birds )[ BIRD_NUMBER ], sf::RenderWindow* window );


#endif