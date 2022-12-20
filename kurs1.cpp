#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <time.h>

int** vvod_graph(int* size);
int** generation_graph(int* size);
int** file_graph(int* size);
void print_graph(int** graph, int size);
int check_cycle(int** graph, int size);
void eulerian_cycle(int** graph, int size, int cycle_start);

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	printf("Курсовая работа по предмету Логика и основы алгоритмизации в инженерных задачах\nТема: Реализация алгоритма нахождения эйлеровых циклов\nВыполнил: Давкин Максим, 21ВВ1.3\n\n");
	int size = 0, choise1, ** graph = NULL;
	while (true)
	{
		printf("Выберите нужное действие:\n1. Ввести матрицу смежности вручную\n2. Сгенерировать матрицу смежности (случайные значения)\n3. Cчитать матрицу смежности из файла\nEsc - Выход\n\n");
		choise1 = _getch();
		if (choise1 == '1')
			graph = vvod_graph(&size);
		if (choise1 == '2')
			graph = generation_graph(&size);
		if (choise1 == '3')
			graph = file_graph(&size);
		if (choise1 == 27)
			exit(0);
		if ((choise1 != '1') && (choise1 != '2') && (choise1 != '3') && (choise1 != 27)) {
			printf("Неверный ввод!\n");
			continue;
		}
		print_graph(graph, size);
		check_cycle(graph, size);
		for (int i = 0; i < size; i++)
		{
			free(graph[i]);
		}
		free(graph);
	}
}

int** vvod_graph(int* size)
{
	printf("Введите количество вершин графа: ");
	scanf("%d", size);
	printf("Введите матрицу смежности, оставляя пробелы между соседними значениями:\n");
	int** graph = (int**)malloc(sizeof(int*) * (*size));
	for (int i = 0; i < (*size); i++)
	{
		graph[i] = (int*)malloc(sizeof(int) * (*size));
	}
	for (int i = 0; i < (*size); i++)
	{
		for (int j = 0; j < (*size); j++)
		{
			scanf("%d", &graph[i][j]);
		}
	}
	for (int i = 0; i < (*size); i++)
		for (int j = 0; j < (*size); j++)
			if ((graph[i][j] != graph[j][i]) || (graph[i][i] != 0)) {
				printf("Неверный ввод матрицы!\n");
				exit(0);
			}
	printf("Введенная матрица:\n");
	return graph;
}

int** generation_graph(int* size)
{
	printf("Введите количество вершин графа: ");
	scanf("%d", size);
	printf("\n");
	int** graph = (int**)malloc(sizeof(int*) * (*size));
	for (int i = 0; i < (*size); i++)
	{
		graph[i] = (int*)malloc(sizeof(int) * (*size));
	}
	for (int i = 0; i < (*size); i++)
	{
		for (int j = i; j < (*size); j++)
		{
			if (i == j)
			{
				graph[i][j] = 0;
			}
			else {
				graph[i][j] = rand() % 2;
				graph[j][i] = graph[i][j];
			}
		}
	}
	return graph;
}

int** file_graph(int* size)
{
	char input_graph[20];
	printf("Файл должен содержать в себе количество вершин графа, ниже - матрицу смежности. Пример ввода: privet.txt\nВведите название файла: ");
	scanf("%s", &input_graph);
	FILE* f1 = fopen(input_graph, "r");
	if (f1 == NULL)
	{
		printf("Не удалось открыть файл!\n");
		return NULL;
	}
	fscanf(f1, "%d", size);
	int** graph = (int**)malloc(sizeof(int*) * (*size));
	for (int i = 0; i < (*size); i++)
	{
		graph[i] = (int*)malloc(sizeof(int) * (*size));
		for (int j = 0; j < (*size); j++)
		{
			fscanf(f1, "%d", &graph[i][j]);
		}
	}
	fclose(f1);
	return graph;
}

void print_graph(int** graph, int size)
{
	printf("   ");
	for (int i = 0; i < size; i++)
	{
		printf("[%d]", i);
	}
	printf("\n");
	for (int i = 0; i < size; i++)
	{
		printf("[%d]", i);
		for (int j = 0; j < size; j++)
		{
			printf(" %d ", graph[i][j]);
		}
		printf("\n");
	}
}

int check_cycle(int** graph, int size)
{
	int flag = 0;
	for (int i = 0; i < size; i++)
	{
		int count = 0;
		for (int j = 0; j < size; j++)
		{
			if (graph[i][j] == 1)
				count += graph[i][j];
		}
		if ((count % 2 != 0) || (count == 0))
		{
			flag = 1;
		}
	}
	if (flag == 1) {
		int m = -1, nul = -1, chet = 0;
		printf("В данном графе эйлерова цикла не существует! Хотите достроить (удалить) ребра в графе для получения эйлерова цикла?\n+ да, - нет\n\n");
		int choise2 = _getch();
		if (choise2 == 45)
			return 0;
		if (choise2 == 43) {
			int* degree = (int*)malloc(sizeof(int*) * (size));
			for (int i = 0; i < size; i++)
			{
				degree[i] = 0;
			}
			int o = -1; // сохраняет вершину со степенью size-1
			int p = -1; // сохраняет вершину с 0 степенью
			int u = -1; // сохраняет вершину с нечетной степенью
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (graph[i][j] == 1) {
						degree[i]++;
					}
				}
				if ((degree[i] == size - 1) && ((size - 1) % 2 != 0)) {
					if (o == -1) {
						o = i;
					}
					else {
						graph[o][i] = 0;
						graph[i][o] = 0;
						printf("Удалено ребро [%d]-[%d]\n", o, i);
						degree[o]--;
						degree[i]--;
						o = -1;
					}
				}
				if (degree[i] == 0) {
					if (p == -1) {
						p = i;
					}
					else {
						graph[p][i] = 1;
						graph[i][p] = 1;
						printf("Добавлено ребро [%d]-[%d]\n", p, i);
						degree[i]++;
						degree[p]++;
						p = -1;
					}
				}
			}
			for (int i = 0; i < size; i++)
			{
				if (o != -1) {
					if (graph[o][i] == 1)
					{
						graph[o][i] = 0;
						graph[i][o] = 0;
						printf("Удалено ребро [%d]-[%d]\n", o, i);
						degree[i]--;
						degree[o]--;
						o = -1;
					}
				}
				if (p != -1) {
					if ((graph[i][p] == 0) && (degree[i] % 2 != 0)) {
						graph[p][i] = 1;
						graph[i][p] = 1;
						printf("Добавлено ребро [%d]-[%d]\n", p, i);
						degree[i]++;
						degree[p]++;
						p = -1;
					}
				}
				if ((degree[i] % 2 != 0) && (o == -1)) {
					if (u == -1) {
						u = i;
					}
					else {
						if (graph[u][i] == 0)
						{
							graph[u][i] = 1;
							graph[i][u] = 1;
							printf("Добавлено ребро [%d]-[%d]\n", u, i);
							degree[i]++;
							degree[u]++;
							u = -1;
						}
					}
				}
			}
			for (int i = 0; i < size; i++)
			{
				if (degree[i] % 2 != 0) {
					if (u == -1) {
						u = i;
					}
					else {
						if (graph[u][i] == 0)
						{
							graph[u][i] = 1;
							graph[i][u] = 1;
							printf("Добавлено ребро [%d]-[%d]\n", u, i);
							degree[i]++;
							degree[u]++;
							u = -1;
						}
					}
				}
				if (degree[i] == 0)
				{
					for (int h = 0; h < size; h++) {
						if (degree[h] != 0) {
							for (int j = 0; j < size; j++) {
								if (graph[h][j] == 1) {
									graph[h][j] = 0;
									graph[j][h] = 0;
									printf("Удалено ребро [%d]-[%d]\n", h, j);
									graph[h][i] = 1;
									graph[i][h] = 1;
									graph[j][i] = 1;
									graph[i][j] = 1;
									printf("Добавлено ребро [%d]-[%d]\n", h, i);
									printf("Добавлено ребро [%d]-[%d]\n", i, j);
									degree[i]+=2;
									break;
								}
							}
							break;
						}
					}
				}
			}
			printf("\n");
			print_graph(graph, size);
		}
		if ((choise2 != 45) && (choise2 != 43)) {
			printf("Неверный ввод!\n\n");
			return 0;
		}
	}
	int cycle_start = 0;
	printf("В данном графе эйлеров цикл существует. Введите вершину, с которой начнется цикл: ");
	scanf("%d", &cycle_start);
	if (cycle_start >= size || cycle_start < 0)
	{
		printf("Введенной вершины в графе не существует! Вершиной начала цикла будет являться максимальная по значению вершина графа.\n");
		cycle_start = size - 1;
	}
	printf("Эйлеров цикл: ");
	eulerian_cycle(graph, size, cycle_start);
	printf("\n\n");
}

void eulerian_cycle(int** graph, int size, int cycle_start)
{
	for (int cycle_next = 0; cycle_next < size; cycle_next++)
	{
		if (graph[cycle_start][cycle_next] == 1)
		{
			graph[cycle_start][cycle_next] = 0;
			graph[cycle_next][cycle_start] = 0;
			printf("%d-%d ", cycle_start, cycle_next);
			eulerian_cycle(graph, size, cycle_next);
		}
	}
}