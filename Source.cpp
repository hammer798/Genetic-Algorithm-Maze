#include <iostream>;
#include <time.h>;
using namespace std;

#define CROSSOVER .2
#define MUTATE .001
#define POP_SIZE 100
#define INSTRUCT_LEN 20
#define MAX_ALLOWABLE_GENS 50

#define RANDOM_NUM ((float)rand()/(RAND_MAX+1))


struct chromosome_typ {
	char* instructions;
	int fitness;
};

void randomInstruct(char*);
void Mutate(chromosome_typ&);
void Crossover(chromosome_typ&, chromosome_typ&);
int CalcFitness(char*, int (&maze)[10][10]);
bool opposite(char, char);
void sortByFitness(chromosome_typ (&Population)[POP_SIZE]);
void nextGen(chromosome_typ(&Population)[POP_SIZE]);




int main(int argc, char** argv) {
	srand((int)time(NULL));
	
	int currentGen = 0;
	chromosome_typ solution;
	int maze[10][10] = { {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
						{1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1,	0, 1, 1, 0, 0},
						{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
						{0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
						{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
						{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
						{0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

	chromosome_typ Population[POP_SIZE];
	for (int x = 0; x < POP_SIZE; x++) {
		Population[x].instructions = new char[20];
		randomInstruct(Population[x].instructions);
		Population[x].fitness = 0;
	}
	bool success = false;
	while(!success && currentGen < MAX_ALLOWABLE_GENS){
		int maxFitness = 0;
		currentGen++;
		for (int x = 0; x < POP_SIZE; x++) {
			Population[x].fitness = CalcFitness(Population[x].instructions, maze);
			if (Population[x].fitness == 20) {
				success = true;
				solution = Population[x];
				break;

			}
			if (Population[x].fitness > maxFitness)
				maxFitness = Population[x].fitness;
			for (int i = 0; i < INSTRUCT_LEN; i++) {
				cout << Population[x].instructions[i];
			}
			cout << endl << "Fitness: " << Population[x].fitness << endl;
		}
		cout << "Max Fitness this gen was " << maxFitness << endl;
		sortByFitness(Population);
		nextGen(Population);
	}
	if (success) {
		cout << endl << endl;
		cout << "Congatulations, we solved the maze in " << currentGen << " generations!";
	}
	else {
		cout << endl << endl;
		cout << "Sorry, no success this time!";
	}
	cout << endl;
}

int CalcFitness(char* instructions, int (&maze)[10][10]) {
	int x = 0, y = 0, fitness = 0;
	char last = 'C';
	for (int i = 0; i < INSTRUCT_LEN; i++) {
		switch (instructions[i]) {
		case 'R':
			x++;
			break;
		case 'L':
			x--;
			break;
		case 'U':
			y--;
			break;
		case 'D':
			y++;
			break;
		}
		if (x < 0 || y < 0 || x > 9 || y > 9)
			return fitness;
		else if (opposite(last, instructions[i]))
			return fitness;
		else if (maze[y][x] == 0)
			return fitness;
		fitness++;
		last = instructions[i];
	}
	return fitness;
}

bool opposite(char char1, char char2) {
	if (abs(char1 - char2) == 6)
		return true;
	else if (abs(char1 - char2) == 17)
return true;
return false;
}

void randomInstruct(char* instructions) {
	for (int i = 0; i < INSTRUCT_LEN; i++) {
		int num = (int)(RANDOM_NUM * 4);
		char instruct;
		switch (num) {
		case 0:
			instruct = 'R';
			break;
		case 1:
			instruct = 'D';
			break;
		case 2:
			instruct = 'L';
			break;
		default:
			instruct = 'U';
		}
		instructions[i] = instruct;
	}
}

void Crossover(chromosome_typ& chrome1, chromosome_typ& chrome2) {
	if (RANDOM_NUM < CROSSOVER)
	{
		//create a random crossover point
		int crossover = (int)(RANDOM_NUM * INSTRUCT_LEN);
		//temporary array to store part of array
		char* temp = new char[INSTRUCT_LEN - crossover];
		for (int x = crossover; x < INSTRUCT_LEN; x++) {
			temp[x - crossover] = chrome1.instructions[x];
		}
		for (int x = crossover; x < INSTRUCT_LEN; x++) {
			chrome1.instructions[x] = chrome2.instructions[x];
			chrome2.instructions[x] = temp[x - crossover];
		}
		delete(temp);
	}
}

void Mutate(chromosome_typ& chromo) {
	for (int i = 0; i < INSTRUCT_LEN; i++)
	{
		if (RANDOM_NUM < MUTATE)
		{
			int num = (int)(RANDOM_NUM * 4);
			switch (chromo.instructions[i]) {
			case 'D':
				if (num == 0)
					chromo.instructions[i] = 'U';
				else if (num == 1)
					chromo.instructions[i] = 'L';
				else
					chromo.instructions[i] = 'R';
			case 'U':
				if (num == 0)
					chromo.instructions[i] = 'U';
				else if (num == 1)
					chromo.instructions[i] = 'D';
				else
					chromo.instructions[i] = 'R';
			case 'L':
				if (num == 0)
					chromo.instructions[i] = 'U';
				else if (num == 1)
					chromo.instructions[i] = 'D';
				else
					chromo.instructions[i] = 'R';
			case 'R':
				if (num == 0)
					chromo.instructions[i] = 'U';
				else if (num == 1)
					chromo.instructions[i] = 'L';
				else
					chromo.instructions[i] = 'D';
			}
		}
	}
}


void sortByFitness(chromosome_typ(&Population)[POP_SIZE]) {
	int i, j;
	chromosome_typ temp;
	for (i = 0; i < POP_SIZE; i++)
	{
		for (j = i; j < POP_SIZE; j++)  
		{
			if (Population[i].fitness > Population[j].fitness) {
				temp = Population[i];   
				Population[i] = Population[j];
				Population[j] = temp;
			}
		}
		
	}
}

void nextGen(chromosome_typ(&Population)[POP_SIZE]){
	chromosome_typ tempPop[POP_SIZE];
	
	for (int i = 0; i < POP_SIZE; i += 2) {
		int rand1 = (int)(RANDOM_NUM * (POP_SIZE / 2)) + 50;
		int rand2 = (int)(RANDOM_NUM * (POP_SIZE / 2)) + 50;
		chromosome_typ chrome1 = Population[rand1];
		chromosome_typ chrome2 = Population[rand2];
		
		Crossover(chrome1, chrome2);
		Mutate(chrome1);
		Mutate(chrome2);

		tempPop[i] = chrome1;
		tempPop[i + 1] = chrome2;
	}
	for (int x = 0; x < POP_SIZE; x++) {
		Population[x] = tempPop[x];
	}
} 