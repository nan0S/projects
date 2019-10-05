#include <iostream>
#include <SFML/Graphics.hpp>
#include "../universalfunctions/useful.h"
using namespace sf;

const int W = 1200, H = 720;
const int A = 8;
const int cols = W / A, rows = H / A;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
bool is;

class Cell
{
public:

    Vector2i posInArray;
    Vector2f posOnScreen;
    
    VertexArray shape;

    enum State 
    {
        growing,
        burning
    };

    float color, newColor;
    float maxColor;
    State state, newState;

    float burnMultiplier;

    float burnResistance;
    float burnCounter;

    std :: vector <Cell*> Neighbours;

    Cell () {}

    Cell (Vector2i posInArray_)
    {
        posInArray = posInArray_;
        posOnScreen = Vector2f(posInArray.y * A, posInArray.x * A);

        shape = VertexArray(Quads, 4);

        shape[0].position = Vector2f(posOnScreen.x, posOnScreen.y);
        shape[1].position = Vector2f(posOnScreen.x, posOnScreen.y + A);
        shape[2].position = Vector2f(posOnScreen.x + A, posOnScreen.y + A);
        shape[3].position = Vector2f(posOnScreen.x + A, posOnScreen.y);

        state = growing;
        maxColor = randFraction(200.f, 255.f);
        color = std::min(randFraction(0.f, 255.f), maxColor);

        burnMultiplier = randFraction(0.1f, 1.f);

        burnCounter = 0.f;
        burnResistance = constrain(color, 0.f, 255.f, 50.f, 35.f);

        SetColor(Color(0.f, color, 0.f));
    }

    void AddNeighbour (Cell *neighbour)
    {
        Neighbours.push_back(neighbour);
    }

    void CalculateChanges ()
    {
        float burnStrength = 0.f, growStrength = 0.f;

        for (auto s : Neighbours)
        {
            if (s -> state == growing)
                growStrength += constrain(s -> color, 0.f, 255.f, 0.0001f, 0.0005f);
            else
                burnStrength += constrain(s -> color, 0.f, 255.f, 0.f, 3.f);
        }

        burnStrength *= burnMultiplier;

        if (state == growing)
        {
            if (burnCounter + burnStrength >= burnResistance)
            {
                newState = burning;
                newColor =  randFraction(240.f, 255.f);
            }
            else
            {
                newState = growing;
                newColor = std::min(color + growStrength + constrain(color, 0.f, 255.f, 0.01f, 0.5f), maxColor);

                burnCounter += burnStrength;
                burnResistance = constrain(color, 0.f, 255.f, 50.f, 35.f);
            }
        }
        else
        {
            if (color <= 1.f)
            {
                newState = growing;
                newColor = randFraction(0.f, 10.f);

                burnResistance = 100.f;
                burnCounter = 0.f;
            }
            else
            {
                newState = burning;
                newColor = color - constrain(color, 0.f, 255.f, 0.3f, 2.f);
            }
        }
    }

    void Update ()
    {
        color = newColor;
        state = newState;

        if (state == growing)
            SetColor(Color(0.f, color, 0.f));
        else
            SetColor(Color(color, 0.f, 0.f));
    }

    void Ignite ()
    {
        state = burning;
        color = 255.f;
    }

    void SetColor (Color color)
    {
        for (int i = 0; i < 4; ++i)
            shape[i].color = color;
    }

    void Render (RenderWindow &window)
    {
        window.draw(shape);
    }
};

class Game
{
public:

    RenderWindow window;
    Cell* Cells[rows + 1][cols + 1];

    Game ()
    {
        SetupWindow();
        SetupCells();
    }

    void SetupWindow ()
    {
        std::cout << "Click on tree to set it on fire";
        ContextSettings settings;
        settings.antialiasingLevel = 16;
        window.create(VideoMode(W, H), "Forest Fire Simulation", Style :: Default, settings);
        window.setFramerateLimit(60);
    }

    void SetupCells ()
    {
        for (int x = 0; x < rows; ++x) 
            for (int y = 0; y < cols; ++y)
                Cells[x][y] = new Cell(Vector2i(x, y));

        for (int x = 0; x < rows; ++x)
        {
            for (int y = 0; y < cols; ++y)
            {
                Cell *c = Cells[x][y];

                for (int i = 0; i < 4; ++i)
                {
                    int nx = x + dx[i], ny = y + dy[i];
                    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols)
                        continue;
                    
                    c -> AddNeighbour(Cells[nx][ny]);
                }
            }
        }
    }

    void Run ()
    {
        while (window.isOpen())
        {
            ProcessEvents();
            Update();
            Render();
        }
    }

    void ProcessEvents ()
    {
        Event event;
        
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event :: Closed:
                    window.close();
                    break;
                
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        Vector2i pos = Mouse::getPosition(window);
                        pos /= A;
                        Cells[pos.y][pos.x] -> Ignite();
                    }
            }
        }
    }

    void Update ()
    {
        for (int x = 0; x < rows; ++x)
            for (int y = 0; y < cols; ++y)
                Cells[x][y] -> CalculateChanges();

        for (int x = 0; x < rows; ++x)
            for (int y = 0; y < cols; ++y)
                Cells[x][y] -> Update();
    }

    void Render ()
    {
        window.clear(Color::Black);

        for (int x = 0; x < rows; ++x)
            for (int y = 0; y < cols; ++y)
                Cells[x][y] -> Render(window);

        window.display();
    }
};

int main()
{
    srand(time(NULL));

    Game game;
    game.Run();

    return 0;
}