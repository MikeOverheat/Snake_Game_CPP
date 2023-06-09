#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
using namespace std;

int const WIDTH = 20; // Ширина
int const HEIGHT = 20; // Высота
bool game_over;
int x, y; // Координаты змеи
int fruit_x, fruit_y, score; // Координаты фрукта
int tail_x[100], tail_y[100]; // Координаты хвоста
int n_tail; // Размер хвоста
enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Направления движения
direction snake;
direction lock_move;

void Setup()
{
    game_over = false;
    snake = STOP;
    x = WIDTH / 2 - 1;
    y = HEIGHT / 2 - 1;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
    score = 0;
}

void Draw()
{
    system("cls");

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < HEIGHT; i++) {
        cout << "#";
        for (int j = 0; j < WIDTH; j++) {
            if (i == y && j == x)
                cout << "S";
            else if (i == fruit_y && j == fruit_x)
                cout << "F";
            else {
                bool print = false;
                for (int k = 0; k < n_tail; k++) {
                    if (tail_x[k] == j && tail_y[k] == i) {
                        print = true;
                        cout << "s";
                    }
                }
                if (!print)
                    cout << " ";
            }
        }
        cout << "#" << endl;
    }

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    cout << "SCORE: " << score << endl;
}

void Input()
{
    if (_kbhit()) {
        switch (_getch())
        {
        case 'a':
            if (lock_move != LEFT) {
                snake = LEFT;
                lock_move = RIGHT;
            }
            break;
        case 'd':
            if (lock_move != RIGHT) {
                snake = RIGHT;
                lock_move = LEFT;
            }
            break;
        case 'w':
            if (lock_move != UP) {
                snake = UP;
                lock_move = DOWN;
            }
            break;
        case 's':
            if (lock_move != DOWN) {
                snake = DOWN;
                lock_move = UP;
            }
            break;
        case 'x':
            game_over = true;
            break;
        }
    }
}

void Logic()
{
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev_2x, prev_2y;
    tail_x[0] = x;
    tail_y[0] = y;
    for (int i = 1; i < n_tail; i++) {
        prev_2x = tail_x[i];
        prev_2y = tail_y[i];
        tail_x[i] = prev_x;
        tail_y[i] = prev_y;
        prev_x = prev_2x;
        prev_y = prev_2y;
    }

    switch (snake)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    if (x >= WIDTH)
        x = 0;
    else if (x < 0)
        x = WIDTH - 1;
    if (y >= HEIGHT)
        y = 0;
    else if (y < 0)
        y = HEIGHT - 1;

    for (int i = 0; i < n_tail; i++) {
        if (tail_x[i] == x && tail_y[i] == y)
            game_over = true;
    }

    if (x == fruit_x && y == fruit_y) {
        score += 10;
        n_tail++;
        fruit_x = rand() % WIDTH;
        fruit_y = rand() % HEIGHT;
    }
}

int main() {
    HWND hWindow = GetConsoleWindow();
    SetWindowPos(hWindow, HWND_TOP, 600, 100, 300, 300, NULL);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {25, 25}; // 22, 24 - минимальный размер консоли для корректного отображения
    SMALL_RECT src = {0, 0, coord.X-1, coord.Y-1};
    SetConsoleWindowInfo(hOut, true, &src);
    SetConsoleScreenBufferSize(hOut, coord);
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

    srand(static_cast<int>(time(0)));
    Setup();
    while (!game_over){
        Draw();
        Input();
        Logic();
        Sleep(500 / (n_tail + 1));
    }
    cout << "GAME OVER" << endl;
    getch();
    return 0;
}