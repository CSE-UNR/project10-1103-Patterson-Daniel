//Author: Daniel Patterson
//Date: 30 April 2026
//Purpose: Project 10 CS135 Twordle.c

#include <stdio.h>

#define WORD_LEN 5
#define MAX_GUESSES 6
#define TRUE 1
#define FALSE 0

int strLen(char text[]);
int isLetter(char ch);
int hasNonLetter(char text[]);
void toLower(char text[]);
void copyString(char dest[], char src[]);
int wordsMatch(char first[], char second[]);
int getWord(char mystery[]);
void getGuess(char guess[]);
void buildDisplay(char guess[], char mystery[], char display[], char arrows[]);
void displayBoard(char guesses[][WORD_LEN + 1], char arrows[][WORD_LEN + 1], int count);
void displayWinMessage(int guessesUsed);

int main(void)
{
    char mystery[WORD_LEN + 1];
    char guess[WORD_LEN + 1];
    char shownGuess[WORD_LEN + 1];
    char arrowLine[WORD_LEN + 1];
    char allGuesses[MAX_GUESSES][WORD_LEN + 1];
    char allArrows[MAX_GUESSES][WORD_LEN + 1];

    int guessCount;
    int won;

    if (getWord(mystery) == FALSE)
    {
        printf("Sorry, could not get today's word. Try again tomorrow.\n");
        return 0;
    }

    guessCount = 0;
    won = FALSE;

    while (guessCount < MAX_GUESSES && won == FALSE)
    {
        if (guessCount < MAX_GUESSES - 1)
        {
            printf("GUESS %d! Enter your guess: ", guessCount + 1);
        }
        else
        {
            printf("FINAL GUESS : ");
        }

        getGuess(guess);
        toLower(guess);

        buildDisplay(guess, mystery, shownGuess, arrowLine);

        copyString(allGuesses[guessCount], shownGuess);
        copyString(allArrows[guessCount], arrowLine);

        if (wordsMatch(guess, mystery) == TRUE)
        {
            won = TRUE;
        }

        guessCount++;

        if (won == FALSE)
        {
            printf("================================\n");
            displayBoard(allGuesses, allArrows, guessCount);
        }
    }

    printf("================================\n");

    if (won == TRUE)
    {
        printf("\t\t%s\n", shownGuess);
        displayWinMessage(guessCount);
    }
    else
    {
        displayBoard(allGuesses, allArrows, guessCount);
        printf("\tYou lost, better luck next time!\n");
    }

    return 0;
}

int strLen(char text[])
{
    int i;

    i = 0;
    while (text[i] != '\0')
    {
        i++;
    }

    return i;
}

int isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return TRUE;
    }

    return FALSE;
}

int hasNonLetter(char text[])
{
    int i;

    i = 0;
    while (text[i] != '\0')
    {
        if (isLetter(text[i]) == FALSE)
        {
            return TRUE;
        }
        i++;
    }

    return FALSE;
}

void toLower(char text[])
{
    int i;

    i = 0;
    while (text[i] != '\0')
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] = text[i] + ('a' - 'A');
        }
        i++;
    }
}

void copyString(char dest[], char src[])
{
    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}

int wordsMatch(char first[], char second[])
{
    int i;

    for (i = 0; i < WORD_LEN; i++)
    {
        if (first[i] != second[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

int getWord(char mystery[])
{
    FILE *inputFile;
    int result;

    inputFile = fopen("mystery.txt", "r");

    if (inputFile == NULL)
    {
        return FALSE;
    }

    result = fscanf(inputFile, "%5s", mystery);
    fclose(inputFile);

    if (result != 1)
    {
        return FALSE;
    }

    toLower(mystery);
    return TRUE;
}

void getGuess(char guess[])
{
    while (1)
    {
        scanf("%20s", guess);

        if (strLen(guess) != WORD_LEN)
        {
            printf("\tThe guess must be 5 letters long. Try again: ");
        }
        else if (hasNonLetter(guess) == TRUE)
        {
            printf("\tThe guess must contain only letters. Try again: ");
        }
        else
        {
            return;
        }
    }
}

void buildDisplay(char guess[], char mystery[], char display[], char arrows[])
{
    int i;
    int j;
    int usedMystery[WORD_LEN];
    int matched;

    for (i = 0; i < WORD_LEN; i++)
    {
        display[i] = guess[i];
        arrows[i] = ' ';
        usedMystery[i] = FALSE;
    }

    display[WORD_LEN] = '\0';
    arrows[WORD_LEN] = '\0';

    for (i = 0; i < WORD_LEN; i++)
    {
        if (guess[i] == mystery[i])
        {
            display[i] = guess[i] - ('a' - 'A');
            usedMystery[i] = TRUE;
        }
    }

    for (i = 0; i < WORD_LEN; i++)
    {
        if (display[i] >= 'A' && display[i] <= 'Z')
        {
            continue;
        }

        matched = FALSE;

        for (j = 0; j < WORD_LEN && matched == FALSE; j++)
        {
            if (usedMystery[j] == FALSE && guess[i] == mystery[j])
            {
                arrows[i] = '^';
                usedMystery[j] = TRUE;
                matched = TRUE;
            }
        }
    }
}

void displayBoard(char guesses[][WORD_LEN + 1], char arrows[][WORD_LEN + 1], int count)
{
    int i;
    int hasArrow;

    for (i = 0; i < count; i++)
    {
        printf("\t\t%s\n", guesses[i]);

        hasArrow = FALSE;

        if (arrows[i][0] == '^' || arrows[i][1] == '^' || arrows[i][2] == '^' ||
            arrows[i][3] == '^' || arrows[i][4] == '^')
        {
            hasArrow = TRUE;
        }

        if (hasArrow == TRUE)
        {
            printf("\t\t%s\n", arrows[i]);
        }
    }
}

void displayWinMessage(int guessesUsed)
{
    if (guessesUsed == 1)
    {
        printf("\tYou won in 1 guess!\n");
        printf("\t\tGOATED!\n");
    }
    else if (guessesUsed == 2)
    {
        printf("\tYou won in 2 guesses!\n");
        printf("\t\tAmazing!\n");
    }
    else if (guessesUsed == 3)
    {
        printf("\tYou won in 3 guesses!\n");
        printf("\t\tAmazing!\n");
    }
    else if (guessesUsed == 4)
    {
        printf("\tYou won in 4 guesses!\n");
        printf("\t\tNice!\n");
    }
    else if (guessesUsed == 5)
    {
        printf("\tYou won in 5 guesses!\n");
        printf("\t\tNice!\n");
    }
    else
    {
        printf("\tYou won in 6 guesses!\n");
    }
}
