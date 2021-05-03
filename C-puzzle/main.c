#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct
{
	char* word; //word and corresponding hint
	char* clue;
	int x; //Starting x and y positions
	int y;
	char direction; //H for horizontal, V for vertical
	int f; //solved or not
} Word_t;

Word_t* loadTextFile(FILE* myFile, int nrWords);								//done
void displayBoard(int rows, int cols, char** myBoard);							//done
int isBoardFilled(int rows, int cols, char** myBoard);							//done
char** createArray(int rows, int cols);											//done
int findIndex(int arr[], int size, int val);									//done
char** updateBoard(char** myBoard, Word_t* words, int solve, int nrWords);		//done
void playGame(char** myBoard, int words, Word_t* word, int x, int y);			//done
int charToInt(char c);
Word_t* deleteArray(Word_t* words, int size, int element);
int valueInArray(int arr[], int size, int val);


int main()
{
	char** board;
	char line[256];
	char myfile[30];
	printf("Please enter the file name: ");
	scanf("%s", myfile);
	FILE* myFile = fopen(myfile, "r");
	while(myFile == NULL)
	{
		printf("Please enter a valid file name:  ");
		scanf("%s", myfile);

		myFile = fopen(myfile, "r");
	}
	int rowNumber;
	int colNumber;
	int nrWords;
	fgets(line, sizeof(line), myFile);
	rowNumber = charToInt(line[0]);
	colNumber = charToInt(line[2]);
	nrWords = charToInt(line[4]);
	if(strlen(line) == 7)
		nrWords = (nrWords * 10) + charToInt(line[5]);
	Word_t* array = (Word_t*)malloc(nrWords * sizeof(Word_t));
	array = loadTextFile(myFile, nrWords);
	board = createArray(rowNumber, colNumber);
	printf("Game is %d rows x %d cols with %d words\nWords and clues are read!\n", rowNumber, colNumber, nrWords);
	playGame(board, nrWords, array, rowNumber, colNumber);
	fclose(myFile);
	return 0;
}



Word_t* loadTextFile(FILE* myFile, int nrWords)
{
	Word_t* temp = (Word_t*)malloc(nrWords * sizeof(Word_t));
	for (int i = 0; i < nrWords; i++) {
		(*(temp + i)).word = (char*)malloc(256 * sizeof(char));
		(*(temp + i)).clue = (char*)malloc(256 * sizeof(char));
		fscanf(myFile, "%c %d %d %s %[^\n]\n", &((temp + i))->direction, &((temp + i))->x, &((temp + i))->y, ((temp + i))->word, ((temp + i))->clue);
		(*(temp + i)).f = 0;
	}
	return temp;
}



char** createArray(int rows, int cols)
{
	char** tempBoard = (char**)malloc(rows * sizeof(char*));
	for (int i = 0; i < rows; i++)
	{
		*(tempBoard + i) = (char*)malloc(cols * sizeof(char));
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			tempBoard[i][j] = *"#";
		}
	}
	return tempBoard;
}



void displayBoard(int rows, int cols, char** myBoard)
{
	char* colNumer;
	printf("\t");
	for (int i = 1; i <= cols; i++)
	{
		printf(" %d ", i);
	}
	printf("\n\t ");
	for (int i = 1; i <= cols; i++)
	{
		printf("-- ");
	}
	printf("\n");
	for (int i = 0; i < rows; i++)
	{
		printf("%d\t|", i + 1);
		for (int j = 0; j < cols; j++)
		{
			printf(" %c ", myBoard[i][j]);
		}
		printf("\n");
	}
	printf(" \t");
	for (int i = 0; i < cols; i++)
		printf("***");
}



int isBoardFilled(int rows, int cols, char** myBoard)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (myBoard[i][j] == *"_") {
				return 0;
			}
		}
	}
	return 1;
}


char** updateBoard(char** myBoard, Word_t* words, int solve, int nrWords)
{
	if (solve == -2)
	{
		for (int i = 0; i < nrWords; i++)
		{
			int startRow = words[i].x - 1;
			int startCol = words[i].y - 1;
			char direction = words[i].direction;
			for (int j = 0; j < strlen(words[i].word); j++)
			{
				if (direction == 'H')
				{
					myBoard[startRow][startCol] = *"_";
					startCol += 1;
				}
				if (direction == 'V')
				{
					myBoard[startRow][startCol] = *"_";
					startRow += 1;
				}
			}
		}
	}
	if (words[solve].f == 1)
	{
		int startRow = words[solve].x - 1;
		int startCol = words[solve].y - 1;
		char direction = words[solve].direction;
		for (int j = 0; j < strlen(words[solve].word); j++)
		{
			if (direction == 'H')
			{
				myBoard[startRow][startCol] = words[solve].word[j];
				startCol += 1;
			}
			if (direction == 'V')
			{
				myBoard[startRow][startCol] = words[solve].word[j];
				startRow += 1;
			}
		}
	}
	return myBoard;

}


void playGame(char** myBoard, int words, Word_t* word, int row, int col)
{
	myBoard = updateBoard(myBoard, word, -2, words);
	int solve, comparison;
	char answer[20];
	int nrWords = words;
	printf("Current Puzzle:\n\n");
	displayBoard(row, col, myBoard);
	int gameOver = 0;
	int* notSolved = (int*)malloc(words * sizeof(int));
	for (int i = 0; i < words; i++)
	{
		*(notSolved + i) = i + 1;
	}
	while ((gameOver < nrWords))
	{
		if (isBoardFilled(row, col, myBoard) == 1) {
			break;
		}
		printf("\nAsk for hint:\n");
		printf("#  Direction\trow  col\n");
		for (int i = 0; i < words; i++)
		{
			if (word[i].direction == *"V") {
				printf("%d: Vertical\t%d     %d\n", notSolved[i], word[i].x, word[i].y);
			}
			else if (word[i].direction == *"H") {
				printf("%d: Horizontal\t%d     %d\n", notSolved[i], word[i].x, word[i].y);
			}
		}
		printf("Enter -1 to exit\nWhich word to solve next?: ");
		scanf("%d", &solve);
		if (solve == -1)
		{
			printf("Thanks for playing!!");
			break;
		}
		if (findIndex(notSolved, words, solve))
		{
			printf("Current hint:  %s\nEnter your solution:  ", word[solve - 1].clue);
			scanf("%s", &answer);
			int z = 0;
			for (int i = 0; i < strlen(answer); i++) {
				*(answer + i) = toupper(*(answer + i));
			}
			comparison = strcmp(answer, word[solve - 1].word);
			if (comparison == 0)
			{
				word[solve - 1].f = 1;
				printf("\n\nCorrect!\n\n");
				myBoard = updateBoard(myBoard, word, solve - 1, words);
				word = deleteArray(word, words, solve);
				words--;
				gameOver++;
			}
			else {
				printf("WRONG ANSWER...\n");
			}
		}
		else {
			printf("Please enter a valid value...\n\n");
		}
		if (word)
		{
			printf("Current Puzzle:\n\n");
			displayBoard(row, col, myBoard);
		}
	}
	printf("\nCongratulations! You beat the puzzle");
}

int findIndex(int arr[], int size, int val)
{
	if (val > size || val <= 0) {
		return 0;
	}
	if (valueInArray(arr, size, val))
		return 1;
}

int valueInArray(int arr[], int size, int val)
{
	for (int i = 0; i < size; i++)
	{
		if (val == arr[i])
			return 1;
	}
	return 0;
}


int charToInt(char c) {
	return c - '0';
}


Word_t* deleteArray(Word_t* words, int size, int element)
{
	for (int i = element - 1; i < size - 1; i++)
	{
		words[i] = words[i + 1];
	}
	return words;
}
