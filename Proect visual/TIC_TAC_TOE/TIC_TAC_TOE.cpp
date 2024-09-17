// TIC_TAC_TOE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "framework.h"
using namespace std;





void instructions()
{
	cout << "Привет это игра крестики нолики.\n";
	cout << "Человечиский мозг против компьютера!!!\n\n";
	cout << "Введите число от 0 до 8\n";
	cout << "Пример заполнения:\n\n";
	cout << " 0 | 1 | 2\n";
	cout << " ---------\n";
	cout << " 3 | 4 | 5\n";
	cout << " ---------\n";
	cout << " 6 | 7 | 8\n\n";
	cout << "Ну что поиграем????\n";
}

char askYesNo(string question)
{
	char response;
	do
	{
		cout << question << " (Д/Н): ";
		cin >> response;
	} while (response != 'Д' && response != 'Н');
	return response;
}

int askNumber(string question, int high, int low)
{
	int number;
	do
	{
		cout << question << " (" << low << " - "<< high << "): ";
		cin >> number;
	} while (number > high || number < low);
	return number;
}

char humanPiece(string nm)
{
	char go_first = askYesNo("Хотите играть первым?");
	if (go_first == 'Д')
	{
		cout << "\nХорошо " << nm << " ты первый, твой символ (Х)\n";
		return 'X';
	}
	else
	{
		cout << '\n' << nm << " ты храбрец, попробуй обыграй меня. Твой символ (О)\n";
		return 'O';
	}
}

char opponent(char piece)
{
	if (piece == 'X')
	{
		return 'O';
	}
	else
	{
		return 'X';
	}
}

void displayBoard(const vector<char>& board,HANDLE hStdOut, COORD Pos)
{
	COORD coordScreen = Pos;
	DWORD cCharsWritten;
	FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', 1000, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hStdOut, Pos);
	cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
	cout << "\n\n";
	coordScreen = { 0,28 };
	FillConsoleOutputCharacter(hStdOut, (TCHAR)' ',1000, coordScreen, &cCharsWritten);
}


char winner(const vector<char>& board)
{
	const int WINNING_ROWS[8][3] = { {0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	{0, 4, 8},
	{2, 4, 6} };
	const int TOTAL_ROWS = 8;
	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != ЕМРТУ) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}
	if (count(board.begin(), board.end(), ЕМРТУ) == 0)return TIE;
	return NO_ONE;
}

 inline bool isLegal(int move, const vector<char>& board)
{
	return (board[move] == ЕМРТУ);
}

 int humanMove(const vector<char>& board, char human)
 {
	 int move = askNumber("Какую цифру поставищь?", (board.size() - 1));
	 while (!isLegal(move,board))
	 {
		 cout << "\nЭтот сектор занят.Попробуй еще.\n";
		 move = askNumber("Какую цифру поставищь?", (board.size() - 1));
	 }
	 
	 cout << "Хорошо... \n";
	 return move;
 }

 int computerMove(vector<char> board, char computer)
 {
	 unsigned int move = 0;
	 bool found = false;
	 
	 while (!found && move < board.size())
	 {
			 if (isLegal(move, board))
			 {
				 board[move] = computer;
				 found = winner(board) == computer;
				 board[move] = ЕМРТУ;
			 }
			 if (!found)
			 {
				 ++move;
			 }
	 }

		 if (!found)
		 {
			 move = 0;
			 char human = opponent(computer);
			 while (!found && move < board.size())
			 {
				 if (isLegal(move, board))
				 {
					 board[move] = human;
					 found = winner(board) == human;
					 board[move] = ЕМРТУ;
				 }

				 if (!found)
				 {
					 ++move;
				 }
			 }
		 }
	
	

	 if (!found)
	 {
		 move = 0;
		 unsigned int i = 0;
		 if (!med)
		 {
			 const int BEST_MOVES[] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };
			 while (!found && i < board.size())
			 {
				 move = BEST_MOVES[i];
				 if (isLegal(move, board))
				 {
					 found = true;
				 }
				 ++i;
			 }
		 }
		 else
		 {
			 const int BEST_MOVES[] = { 4, 3, 2, 6, 8, 1, 0, 5, 7 };
			 while (!found && i < board.size())
			 {
				 move = BEST_MOVES[i];
				 if (isLegal(move, board))
				 {
					 found = true;
				 }
				 ++i;
			 }
		 }
	 }

	 cout << "I shall take square number " << move << endl;
	 return move;
 }

 void announceWinner(char winner, char computer, char human, string nmm)
 {
	 if (winner == computer)
	 {
		 cout << winner << " *выиграл!\n";
		 cout << "Бе бе бе, я тебя обыграл лапушок :р\n";
		 cout << "Я компьютер и я лучше тебя.\n";
		 
	 }

	 else if (winner == human)
	 {
		 cout << winner << " *выиграл!\n";
		 cout << "Нет, нет! Этого не может быть! Каким-то образом ты обманул меня.\n";
		 cout << "Но больше ты меня не обманещь. Я компьютер!!!\n";
		 cout << "\a" << "\a" << "\a" << "\a" << "\a" << "\a" << "\a" << "\a" << "\a";
	 }
	 else
	 {
		 cout << "Ничья.\n";
		 cout << "Ну и везунчик " << nmm << " ,серавно я тебя обыграю.\n";
	 }
	 
 }