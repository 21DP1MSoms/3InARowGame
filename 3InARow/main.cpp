#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

int ts = 65; //tile size
sf::Vector2i offset(55, 31);

struct piece
{ int x, y, col, row, kind, match, alpha;
    piece(){match = 0; alpha = 255;}
} grid[10][10];

void swap(piece p1, piece p2)
{
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}


int main()
{
    srand(time(0));

    sf::RenderWindow game(sf::VideoMode(1024, 768), "Match-3 Game!", sf::Style::Close);
    game.setFramerateLimit(75);

    sf::Texture t1, t2, t3;
    t1.loadFromFile("images/background.jpg");
    t2.loadFromFile("images/gems!.png");
    t3.loadFromFile("images/board.png");

    sf::Sprite background(t1), gems(t2), board(t3);
    t1.setSmooth(true); t2.setSmooth(true);

    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++)
        {
            grid[i][j].kind = rand() %3;
            grid[i][j].col  = j;
            grid[i][j].row  = i;
            grid[i][j].x    = j * ts;
            grid[i][j].y    = i * ts;
        }

    int x0, y0, x, y; int click = 0; sf::Vector2i pos;
    bool isSwap = false, isMoving = false;

    while (game.isOpen())
    {
        sf::Event e;
        while (game.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) game.close();

            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    if (!isSwap && !isMoving) click++;
                    pos = sf::Mouse::getPosition(game) - offset;
                }

            }
        }

        /// mouse click
        if (click == 1)
        {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2)
        {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1)
            {swap(grid[y0][x0], grid[y][x]); isSwap = 1; click = 0;}
            else click = 1;
        }

        /// Match finding
        int points = 0;
        for(int i = 1; i <= 9; i++)
            for(int j = 1; j <= 9; j++)
            {
                if (grid[i][j].kind == grid[i + 1][j].kind)
                    if (grid[i][j].kind == grid[i - 1][j].kind)
                        for(int n =- 1 ; n <= 1; n++)
                            grid[i + n][j].match++;

                if (grid[i][j].kind == grid[i][j + 1].kind)
                    if (grid[i][j].kind == grid[i][j - 1].kind)
                        for(int n =- 1; n <= 1; n++)
                            grid[i][j + n].match++;
            }


        /// Moving animation
        isMoving = false;
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
            {
                piece &p = grid[i][j];
                int dx, dy;
                for(int n = 0; n < 4; n++)   // 4 - speed
                {dx = p.x - p.col * ts;
                    dy = p.y - p.row * ts;
                    if (dx) p.x -= dx / abs(dx);
                    if (dy) p.y -= dy / abs(dy);}
                if (dx||dy) isMoving = 1;
            }

        /// Deleting animation
        if (!isMoving)
            for (int i = 1; i <= 9; i++)
                for (int j = 1; j <= 9; j++)
                    if (grid[i][j].match) if (grid[i][j].alpha>10) {grid[i][j].alpha-=10; isMoving=true;}

        /// Get score
        int score = 0;
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
                score+=grid[i][j].match;



        /// Second swap if no match
        if (isSwap && !isMoving)
        {if (!score) swap(grid[y0][x0],grid[y][x]); isSwap=0;}

        /// Update grid
        if (!isMoving)
        {
            for(int i = 9; i > 0; i--)
                for(int j = 1; j <= 9; j++)
                    if (grid[i][j].match)
                        for(int n = i; n > 0; n--)
                            if (!grid[n][j].match) {swap(grid[n][j], grid[i][j]); break;};

            for(int j = 1; j <= 9; j++)
                for(int i=9, n = 0; i > 0; i--)
                    if (grid[i][j].match)
                    {
                        grid[i][j].kind  = rand() %6;
                        grid[i][j].y     = -ts * n++;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                    }
        }


        /// draw
        game.draw(background);
        game.draw(board);
        board.setColor(sf::Color(255, 255, 255, 128));
        board.setPosition(48, 24);
        board.setScale(3.4f, 3.4f);

        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
            {
                piece p = grid[i][j];
                gems.setTextureRect( sf::IntRect(p.kind*64, 0, 64, 64));
                gems.setColor(sf::Color(255, 255, 255, p.alpha));
                gems.setPosition(p.x, p.y);
                gems.move(offset.x - ts, offset.y - ts);
                game.draw(gems);
                gems.setScale(0.8f, 0.8f);
            }
        game.display();
    }
    return 0;
}
