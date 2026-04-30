//Author: Daniel Patterson
//Date: 30 April 2026
//Purpose: Twordle with extra credit
//Extra credit included:
//1. Handles duplicate letters
//2. Top 5 scoreboard saved in a file
//3. User can view scoreboard

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN 5
#define MAX_GUESSES 6
#define MAX_NAME 20
#define TOP_SCORES 5

int strLen(char text[]);
bool isLetter(char ch);
bool hasNonLetter(char text[]);
void toLower(char text[]);
void copyString(char dest[], char src[]);
bool wordsMatch(char first[], char second[]);
bool getWord(char mystery[]);
void getGuess(char guess[]);
void buildDisplay(char guess[], char mystery[], char display[], char arrows[]);
void displayBoard(char guesses[][WORD_LEN + 1], char arrows[][WORD_LEN + 1], int count);
void displayWinMessage(int guessesUsed);

void getPlayerName(char name[]);
void askToViewScoreboard(void);
void initializeScores(int scores[]);
void initializeNames(char names[][MAX_NAME + 1]);
void loadScoreboard(char names[][MAX_NAME + 1], int scores[]);
void saveScoreboard(char names[][MAX_NAME + 1], int scores[]);
void displayScoreboard(char names[][MAX_NAME + 1], int scores[]);
void updateScoreboard(char playerName[], int guessesUsed);

int main(void)
{
    char mystery[WORD_LEN + 1];
    char guess[WORD_LEN + 1];
    char shownGuess[WORD_LEN + 1];
    char arrowLine[WORD_LEN + 1];
    char allGuesses[MAX_GUESSES][WORD_LEN + 1];
    char allArrows[MAX_GUESSES][WORD_LEN + 1];
    char playerName[MAX_NAME + 1];

    int guessCount;
    bool won;

    askToViewScoreboard();

    if (getWord(mystery) == false)
    {
        printf("Sorry, could not get today's word. Try again tomorrow.\n");
        return 0;
    }

    guessCount = 0;
    won = false;

    while (guessCount < MAX_GUESSES && won == false)
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

        if (wordsMatch(guess, mystery) == true)
        {
            won = true;
        }

        guessCount++;

        if (won == false)
        {
            printf("================================\n");
            displayBoard(allGuesses, allArrows, guessCount);
        }
    }

    printf("================================\n");

    if (won == true)
    {
        printf("\t\t%s\n", shownGuess);
        displayWinMessage(guessCount);

        printf("Enter your name for the scoreboard: ");
        getPlayerName(playerName);
        updateScoreboard(playerName, guessCount);
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

bool isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return true;
    }

    return false;
}

bool hasNonLetter(char text[])
{
    int i;

    i = 0;
    while (text[i] != '\0')
    {
        if (isLetter(text[i]) == false)
        {
            return true;
        }
        i++;
    }

    return false;
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

bool wordsMatch(char first[], char second[])
{
    int i;

    for (i = 0; i < WORD_LEN; i++)
    {
        if (first[i] != second[i])
        {
            return false;
        }
    }

    return true;
}

bool getWord(char mystery[])
{
    FILE *inputFile;
    int result;

    inputFile = fopen("mystery.txt", "r");

    if (inputFile == NULL)
    {
        return false;
    }

    result = fscanf(inputFile, "%5s", mystery);
    fclose(inputFile);

    if (result != 1)
    {
        return false;
    }

    toLower(mystery);
    return true;
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
        else if (hasNonLetter(guess) == true)
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
    bool usedMystery[WORD_LEN];
    bool matched;

    for (i = 0; i < WORD_LEN; i++)
    {
        display[i] = guess[i];
        arrows[i] = ' ';
        usedMystery[i] = false;
    }

    display[WORD_LEN] = '\0';
    arrows[WORD_LEN] = '\0';

    for (i = 0; i < WORD_LEN; i++)
    {
        if (guess[i] == mystery[i])
        {
            display[i] = guess[i] - ('a' - 'A');
            usedMystery[i] = true;
        }
    }

    for (i = 0; i < WORD_LEN; i++)
    {
        if (display[i] >= 'A' && display[i] <= 'Z')
        {
            continue;
        }

        matched = false;

        for (j = 0; j < WORD_LEN && matched == false; j++)
        {
            if (usedMystery[j] == false && guess[i] == mystery[j])
            {
                arrows[i] = '^';
                usedMystery[j] = true;
                matched = true;
            }
        }
    }
}

void displayBoard(char guesses[][WORD_LEN + 1], char arrows[][WORD_LEN + 1], int count)
{
    int i;
    bool hasArrow;

    for (i = 0; i < count; i++)
    {
        printf("\t\t%s\n", guesses[i]);

        hasArrow = false;

        if (arrows[i][0] == '^' || arrows[i][1] == '^' || arrows[i][2] == '^' ||
            arrows[i][3] == '^' || arrows[i][4] == '^')
        {
            hasArrow = true;
        }

        if (hasArrow == true)
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

void getPlayerName(char name[])
{
    scanf("%20s", name);
    name[MAX_NAME] = '\0';
}

void askToViewScoreboard(void)
{
    char answer;
    char names[TOP_SCORES][MAX_NAME + 1];
    int scores[TOP_SCORES];

    printf("Would you like to view the scoreboard? (y/n): ");
    scanf(" %c", &answer);

    if (answer == 'y' || answer == 'Y')
    {
        loadScoreboard(names, scores);
        displayScoreboard(names, scores);
    }
}

void initializeScores(int scores[])
{
    int i;

    for (i = 0; i < TOP_SCORES; i++)
    {
        scores[i] = 999;
    }
}

void initializeNames(char names[][MAX_NAME + 1])
{
    int i;

    for (i = 0; i < TOP_SCORES; i++)
    {
        names[i][0] = '\0';
    }
}

void loadScoreboard(char names[][MAX_NAME + 1], int scores[])
{
    FILE *scoreFile;
    int i;
    int result;

    initializeNames(names);
    initializeScores(scores);

    scoreFile = fopen("scores.txt", "r");

    if (scoreFile == NULL)
    {
        return;
    }

    for (i = 0; i < TOP_SCORES; i++)
    {
        result = fscanf(scoreFile, "%20s %d", names[i], &scores[i]);
        if (result != 2)
        {
            names[i][0] = '\0';
            scores[i] = 999;
        }
    }

    fclose(scoreFile);
}

void saveScoreboard(char names[][MAX_NAME + 1], int scores[])
{
    FILE *scoreFile;
    int i;

    scoreFile = fopen("scores.txt", "w");

    if (scoreFile == NULL)
    {
        return;
    }

    for (i = 0; i < TOP_SCORES; i++)
    {
        if (names[i][0] != '\0' && scores[i] != 999)
        {
            fprintf(scoreFile, "%s %d\n", names[i], scores[i]);
        }
    }

    fclose(scoreFile);
}

void displayScoreboard(char names[][MAX_NAME + 1], int scores[])
{
    int i;

    printf("========== SCOREBOARD ==========\n");

    for (i = 0; i < TOP_SCORES; i++)
    {
        if (names[i][0] != '\0' && scores[i] != 999)
        {
            printf("%d. %s - %d\n", i + 1, names[i], scores[i]);
        }
    }

    printf("================================\n");
}

void updateScoreboard(char playerName[], int guessesUsed)
{
    char names[TOP_SCORES][MAX_NAME + 1];
    int scores[TOP_SCORES];
    int i;
    int j;

    loadScoreboard(names, scores);

    for (i = 0; i < TOP_SCORES; i++)
    {
        if (guessesUsed < scores[i] || scores[i] == 999)
        {
            for (j = TOP_SCORES - 1; j > i; j--)
            {
                scores[j] = scores[j - 1];
                copyString(names[j], names[j - 1]);
            }

            scores[i] = guessesUsed;
            copyString(names[i], playerName);
            break;
        }
    }

    saveScoreboard(names, scores);

    printf("Updated scoreboard:\n");
    displayScoreboard(names, scores);
}
