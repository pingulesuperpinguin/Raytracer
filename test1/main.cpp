#include <SFML\Graphics.hpp>

#include <core/Scene.h>

using RenderType = double;

template<typename T>
sf::Texture viewportToTexture(const Viewport<T>& viewport)
{
    sf::Texture texture;
    texture.create(viewport.getWidth(), viewport.getHeight());
    sf::Uint8* pixels = new sf::Uint8[viewport.getWidth() * viewport.getHeight() * 4];

    T min, max;
    std::tie(min, max) = getMinMaxOnViewport<T>(viewport);

    T factor = ((max - min) == 0) ? 1.0 : 1 / (max - min);

    for (int col = 0; col < viewport.getWidth(); ++col)
    {
        for (int row = 0; row < viewport.getHeight(); ++row)
        {
            const Color<T>& c = viewport(col, row);
            pixels[4 * (col + row * viewport.getWidth()) + 0] = sf::Uint8((c.r - min) / (max - min) * 255.0);
            pixels[4 * (col + row * viewport.getWidth()) + 1] = sf::Uint8((c.g - min) / (max - min) * 255.0);
            pixels[4 * (col + row * viewport.getWidth()) + 2] = sf::Uint8((c.b - min) / (max - min) * 255.0);
            pixels[4 * (col + row * viewport.getWidth()) + 3] = sf::Uint8(255);
        }
    }

    texture.update(pixels);
    delete[] pixels;
    return texture;
}

Viewport<RenderType> renderScene()
{
    Camera<RenderType> camera{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, deg_to_rad(90.0)};
    Viewport<RenderType> viewport{800, 600};

    Scene<RenderType> scene;
    scene.addObject(new SphereGeometry<RenderType>{ {5.0, 0, 0}, std::sqrt(1) }, new FlatColorizer<RenderType>{ {1.0, 0.0, 0.0} }, new OpticalProperties<RenderType>{});
    scene.addObject(new SphereGeometry<RenderType>{ {5.0, 1.0, 0}, std::sqrt(0.25) }, new FlatColorizer<RenderType>{ {0.0, 1.0, 0.0} }, new OpticalProperties<RenderType>{});
    scene.addObject(new SphereGeometry<RenderType>{ {5.0, 0, 1.0}, std::sqrt(0.25) }, new FlatColorizer<RenderType>{ {0.0, 0.0, 1.0} }, new OpticalProperties<RenderType>{});

    scene.addLight(new PointLight<RenderType>{ {0.0, 0.5, 5.0}, {1.0, 1.0, 1.0}, 1000.0 });

    scene.render(camera, viewport);

    return viewport;
}

int main()
{
    sf::RenderWindow sfmlWin(sf::VideoMode(1200, 800), "Hello World SFML Window");

    sf::Texture texture = viewportToTexture(renderScene());
    sf::Sprite sprite;
    sprite.setTexture(texture);

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
        sfmlWin.draw(sprite);
        sfmlWin.display();
    }
    return 0;
}
