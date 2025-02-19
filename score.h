#ifndef SCORE_H
#define SCORE_H

int const MAX_SCORES = 5;
int const NAME_LEN = 20;

struct ScoreEntry
{
    char name[NAME_LEN];
    int score;
};


ScoreEntry* getScores();

void loadScoresFromFile();

void saveScoresToFile();

void insertScore(const char *name, const int score);


#endif // !SCORE_H
