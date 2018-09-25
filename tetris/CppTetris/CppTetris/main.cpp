/*
* Author   : Taelim Hwang
* Date     : 2018-09-11
* Title    : Cpp Tetris
* contents : This code is for making simple tetris game.
*            Code is written in C++.
*            Original source code is made by FamTrinli.
*/
#include <SFML/Graphics.hpp>
#include <time.h>
#include <cstdio>

const int M = 20;
const int N = 10;

// Set field
int field[M][N] = { 0 };

// Define point
// Point를 설정 함과 동시에 할당함
struct Point {
	int x, y;
} a[4], b[4];

// Set tetrimino
int figures[7][4] = {
	1, 3, 5, 7, // I
	2, 4, 5, 7, // Z
	3, 5, 4, 6, // S
	3, 5, 4, 7, // T
	2, 3, 5, 7, // L
	3, 5, 7, 6, // J
	2, 3, 4, 5, // O
};

// 테트로미노가 화면 밖을 빠져나가거나, 이미 블록이 있는 곳인지 확인
bool check() {
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	return 1;
}

int main(){
	// Set random
	srand(time(0));

	// Init game window
	sf::RenderWindow window(sf::VideoMode(320, 480), "The Tetris");


	// Set texture
	sf::Texture t;
	t.loadFromFile("image/tiles.png");


	// Set sprite
	sf::Sprite s(t);
	s.setTextureRect(sf::IntRect(0, 0, 18, 18));

	// Set feature of tetromino
	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;
	float timer = 0;
	float delay = 0.3;

	sf::Clock clock;

	// Fix bug : First block appears as a single one
	a[0].x = 0, a[0].y = 1;
	a[1].x = 1, a[1].y = 1;
	a[2].x = 1, a[2].y = 2;
	a[3].x = 1, a[3].y = 3;

	// Window event codes
	while (window.isOpen()) {
		// Set timer
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Init event
		sf::Event e;


		// Game codes within event
		while (window.pollEvent(e)) {
			// Quit game
			if (e.type == sf::Event::Closed)
				window.close();

			// Key pressed
			if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Up) rotate = true;
				else if (e.key.code == sf::Keyboard::Left) dx = -1;
				else if (e.key.code == sf::Keyboard::Right) dx = 1;
				
				if (e.key.code == sf::Keyboard::Down) delay = 0.05;
			}

		}

		// Move tetromino
		for (int i = 0; i < 4; i++) {
			b[i] = a[i];
			a[i].x += dx;
		}
		// Check tetromino
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
			

		// Rotate tetromino
		// 알고리즘 문제에서 회전하는 부분을 구할 때 요긴할듯
		if (rotate) {
			Point p = a[1];
			for (int i = 0; i < 4; i++) {
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}

			// Check tetromino
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}

		// Tick
		if (timer > delay) {
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}
			// Check tetromino
			if (!check()) {
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
				
			timer = 0;

			
		}

		// After block is moved...
		dx = 0;
		rotate = 0;
		delay = 0.3;

		// Draw tetromino

		// Set window background
		window.clear(sf::Color::White);

		for(int i = 0;i < M;i++)
			for (int j = 0; j < N; j++) {
				if (field[i][j] == 0) continue;
				s.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				window.draw(s);
			}

		for (int i = 0; i < 4; i++) {
			s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			window.draw(s);
		}
		window.draw(s);
		window.display();
	}
	return 0;
}