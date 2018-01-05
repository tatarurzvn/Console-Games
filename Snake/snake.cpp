#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <queue>
#include <thread>
#include <vector>
#include <cstdlib>

char getch();

using namespace std;
queue <char> key;

bool alive = true;

void inputThread() {
	while (alive) {
		key.push(getch());
	}
}

const int n = 25;
const int m = 80;

char mat[n][m];

void draw() {
	system("clear");
	for (int i = 0; i < n; ++i, std::cout << std::endl)
	{
		for (int j = 0; j < m; ++j)
		{
			cout << mat[i][j];
		}
	}
}

class Pos {
public:
	int x;
	int y;

	Pos(int x = 0, int y = 0) : x(x), y(y) {}
};

bool isInside(int x, int y) {
	return x > 0 && y > 0 && x < n - 1 && y < m - 1;
}

bool isInside(Pos p) {
	return isInside(p.x, p.y);
}

int main(int argc, char const *argv[])
{
	thread input = thread(inputThread);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (i == 0 || i == 24 || j == 0 || j == 79)
				mat[i][j] = '*';
			else
				mat[i][j] = ' ';
		}
	}

	draw();

	vector<Pos> snake;

	snake.push_back(Pos(n / 2, m / 2 + 1));
	snake.push_back(Pos(n / 2, m / 2 + 2));
	snake.push_back(Pos(n / 2, m / 2 + 3));
	snake.push_back(Pos(n / 2, m / 2 + 4));

	Pos dir(0, 1);
	int time = 0;

	mat[18][40] = '#';

	bool start = true;
	while (alive) {
		if (key.size() > 0) {
			int pressKey = key.front();
			key.pop();

			if (pressKey == 'o') {
				alive = false;
			}
			else if (pressKey == 'w') {
				dir = Pos(-1, 0);
			}
			else if (pressKey == 's') {
				dir = Pos(1, 0);
			}
			else if (pressKey == 'a') {
				dir = Pos(0, -1);
			}
			else if (pressKey == 'd') {
				dir = Pos(0, 1);	
			}
		}

		if (isInside(snake[0]) && time++ % 5 == 0) {
			for (int i = snake.size() - 1; i > 0; i--)
				snake[i] = snake[i - 1];
			
			Pos tail;
			if (isInside(snake.back())) {
				mat[snake.back().x][snake.back().y] = ' ';
				tail = snake.back();
			}
			
			snake[0].x += dir.x;
			snake[0].y += dir.y;
			if (!isInside(snake[0])) {
				snake[0].x -= dir.x;
				snake[0].y -= dir.y;
			}
			start = false;

			if (mat[snake[0].x][snake[0].y] == 'o' && snake.size() > 4) {
				alive = false;
				std::cout << "GAME OVER!" << std::endl;
			}

			if (mat[snake[0].x][snake[0].y] == '#') {
				snake.push_back(tail);
				int x = rand() % (n - 1) + 1;
				int y = rand() % (m - 1) + 1;
				
				while (mat[x][y] != ' ') {
					x = rand() % (n - 1) + 1;
					y = rand() % (m - 1) + 1;
				}

				mat[x][y] = '#';
			}
		}

		if (isInside(snake.front()))
			mat[snake.front().x][snake.front().y] = 'o';

		if (alive) {
			draw();
			usleep(8000);
		}
	}

	if (input.joinable())
		input.join();
	
	return 0;
}

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                std::cout << ("read()") << std::endl;
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                std::cout << ("read()") << std::endl;
        if (read(0, &buf, 1) < 0)
                std::cout << ("read()") << std::endl;
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                std::cout << ("read()") << std::endl;
        return (buf);
}