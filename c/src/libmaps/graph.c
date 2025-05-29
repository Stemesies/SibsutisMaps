/*Файл для работы со структурой графа и входными данными*/
#include <libmaps/graph.h>
#include <wchar.h>

Map* map_create()
{
    Map* map = (Map*)malloc(sizeof(Map));
    Graph* graph = graph_create(HASHTABSIZE);
    HashTable* table = hashtab_create();

    if (map == NULL || graph == NULL || table == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под карту.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        free(map);
        free(graph);
        free(table);
        return NULL;
    }

    map->graph = graph;
    map->hashtable = table;

    return map;
}

void map_destroy(Map* map)
{
    if (map != NULL) {
        graph_destroy(map->graph);
        hashtab_destroy(map->hashtable);
        map->graph = NULL;
        map->hashtable = NULL;
    }
    free(map);
}

Graph* graph_create(int n)
{
    Graph* graph = calloc(n, sizeof(Graph));
    graph->graph_matrix = calloc(n, sizeof(Edge*));
    for (int i = 0; i < n; i++)
        graph->graph_matrix[i] = calloc(n, sizeof(Edge));
    graph->n_verticles = n;
    graph->visited = calloc(n, sizeof(bool));
    return graph;
}

void graph_destroy(Graph* graph)
{
    free(graph->visited);
    for (int i = 0; i < graph->n_verticles; i++) {
        free(graph->graph_matrix[i]);
    }
    free(graph->graph_matrix);
    free(graph);
}

void add_edge(Graph* graph, unsigned int i, unsigned int j, int len, int speed)
{
    graph->graph_matrix[i][j].len = len;
    graph->graph_matrix[i][j].speed = speed;
    graph->graph_matrix[j][i].len = len;
    graph->graph_matrix[j][i].speed = speed;
}

/*хэш-функция ELF*/
unsigned int ELFHash(char* s)
{
    unsigned int h = 0, g;
    while (*s) {
        h = (h << 4) + (unsigned int)*s++;
        g = h & 0xF0000000L;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % HASHTABSIZE;
}

HashTable* hashtab_create()
{
    HashTable* a = (HashTable*)calloc(HASHTABSIZE, sizeof(HashTable));
    for (int i = 0; i < HASHTABSIZE; i++) {
        a[i].key = NULL;
    }
    a->count = 0;
    return a;
}

void hashtab_destroy(HashTable* table)
{
    for (int i = 0; i < HASHTABSIZE; i++) {
        if (!table[i].key)
            continue;
        free(table[i].key);
    }
    free(table);
}

unsigned int hashtab_add(HashTable* hashtab, char* key)
{
    unsigned int index = ELFHash(key), temp = index;

    if (hashtab[index].key != NULL) {
        for (int i = index; i < HASHTABSIZE; i++) {
            if (hashtab[i].key == NULL) {
                hashtab[i].key = calloc(strlen(key) + 1, sizeof(char));
                strcpy(hashtab[i].key, key);
                index = i;
                hashtab->count++;
                break;
            }
            if ((index == temp) && (i == HASHTABSIZE - 1))
                i = -1;
        }
    } else {
        hashtab[index].key = calloc(strlen(key) + 1, sizeof(char));
        strcpy(hashtab[index].key, key);
        hashtab->count++;
    }

    return index;
}

int hashtab_lookup(HashTable* hashtab, char* key)
{
    if (!key)
        return -1;
    int index = ELFHash(key);
    if (hashtab[index].key == NULL)
        return -1;

    for (int i = index; i < HASHTABSIZE; i++) {
        if (hashtab[i].key == NULL)
            return -1;
        if (strcmp(hashtab[i].key, key) == 0)
            return i;
        if (i == HASHTABSIZE - 1) {
            i = -1;
        }
    }

    return -1;
}

char* hashtab_getkey(HashTable* table, int id)
{
    for (int i = 0; i < HASHTABSIZE; i++) {
        if (i == id)
            return table[i].key;
    }
    return NULL;
}

bool is_in_table(HashTable* table, char* key)
{
    return (hashtab_lookup(table, key) >= 0);
}

void graph_init(Graph* graph, HashTable* table, FILE* fp)
{
    int path, speed;
    char* str1 = calloc(MAXSTR + 1, sizeof(char));
    char* str2 = calloc(MAXSTR + 1, sizeof(char));
    char ch = 0;
    unsigned int v_1, v_2;
    while ((ch = fgetc(fp)) != EOF) {
        ungetc(ch, fp);
        fscanf(fp, "%s %s %d %d", str1, str2, &path, &speed);

        v_1 = (!is_in_table(table, str1)) ? hashtab_add(table, str1)
                                         : hashtab_lookup(table, str1);
        v_2 = (!is_in_table(table, str2)) ? hashtab_add(table, str2)
                                         : hashtab_lookup(table, str2);

        add_edge(graph, v_1, v_2, path, speed);
        fseek(fp, 1, SEEK_CUR);
    }
    free(str1);
    free(str2);
}

/*Демонстрация графа как матрицы смежности*/
void show_graph(int v, Edge** mass)
{
    if (!mass)
        puts("Нечего показывать.");
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            printf("{%d %d}\t", mass[i][j].len, mass[i][j].speed);
        }
        putchar('\n');
    }
}
