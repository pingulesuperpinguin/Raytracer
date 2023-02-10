#include <SFML\Graphics.hpp>

#include <core/Misc.h>

using RenderType = double;

Viewport<RenderType> renderScene()
{
    Camera<RenderType> camera{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 0.8};
    Viewport<RenderType> viewport{1024, 768};

    Scene<RenderType> scene;
    scene.addObject(new SphereGeometry<RenderType>{ {2.0, 0, 0}, 1.0 }, new FlatColorizer<RenderType>{ {1.0, 1.0, 1.0} }, OpticalProperties<RenderType>{});

    scene.render(camera, viewport);
};

int main()
{
    sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Hello World SFML Window");

    while (sfmlWin.isOpen())
    {

        sf::Event e;
        while (sfmlWin.pollEvent(e))
        {

            switch (e.type)
            {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        sfmlWin.clear();
        sfmlWin.display();
    }
    return 0;
}
