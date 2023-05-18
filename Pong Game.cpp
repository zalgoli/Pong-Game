#include "raylib.h"
#include <string>

struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};
struct Paddle {
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main()
{
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;

	while (!WindowShouldClose()) {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) {

			ball.y = 0;
			ball.speedY *= -1.0f;
		}

		if (ball.y > GetScreenHeight()) {

			ball.y = GetScreenHeight();
			ball.speedY *= -1.0f;
		}


		// Moving left Paddle
		if (IsKeyDown(KEY_W))
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		// Moving right Paddle
		if (IsKeyDown(KEY_UP))
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}


		// Handling collisions for left paddle
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		// Handling collisions for right paddle
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		int left_player_wins = 0;
		int right_player_wins = 0;

		if (ball.x < 0)
		{
			++right_player_wins;
			winnerText = "Right Player Wins!";
		}

		if (ball.x > GetScreenWidth())
		{
			++left_player_wins;
			winnerText = "Left Player Wins!";
		}

		std::string score = std::to_string(left_player_wins) + " - " + std::to_string(right_player_wins);

		int textWidth = MeasureText(score.c_str(), 60);
		DrawText(score.c_str(), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() - (GetScreenHeight() - 15), 35, LIGHTGRAY);

		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			winnerText = nullptr;

			leftPaddle.y = GetScreenHeight() / 2;
			rightPaddle.y = GetScreenHeight() / 2;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();

		if (winnerText) {

			int textWidth = MeasureText(winnerText, 60);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
		}

		DrawFPS(10, 10);
		EndDrawing();


	}
	CloseWindow();

	return 0;
}