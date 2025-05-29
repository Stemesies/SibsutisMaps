/*Файл для работы со структурой графа и входными данными*/
#include <libmaps/graph.h>
#include <wchar.h>

Map* map_create()
{
    Map* map = (Map*)malloc(sizeof(Map));
    Graph* graph = graph_create(HASHTABSIZE);
    HashTable* table = hashtab_create();

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

    graph->n_verticles = n;
    graph->graph_matrix = calloc(n, sizeof(Edge*));
    graph_foreach_verticles(i, graph) graph->graph_matrix[i]
            = calloc(n, sizeof(Edge));

    graph->visited = calloc(n, sizeof(bool));
    return graph;
}

void graph_destroy(Graph* graph)
{
    if (graph == NULL)
        return;

    free(graph->visited);
    graph_foreach_verticles(i, graph) free(graph->graph_matrix[i]);

    free(graph->graph_matrix);
    free(graph);
}

void graph_add_edge(
        Graph* graph, unsigned int i, unsigned int j, int len, int speed)
{
    graph_get(graph, i, j).len = len;
    graph_get(graph, i, j).speed = speed;
    graph_get(graph, j, i).len = len;
    graph_get(graph, j, i).speed = speed;
}

void graph_init(Graph* graph, HashTable* table, FILE* fp)
{
    char* str1 = calloc(MAXSTR + 1, sizeof(char));
    char* str2 = calloc(MAXSTR + 1, sizeof(char));

    int path, speed;
    unsigned int id1, id2;

    char ch = 0;
    while ((ch = fgetc(fp)) != EOF) {
        ungetc(ch, fp);
        fscanf(fp, "%s %s %d %d", str1, str2, &path, &speed);

        id1 = (!hashtab_contains_key(table, str1))
                ? hashtab_add(table, str1)
                : hashtab_lookup(table, str1);
        id2 = (!hashtab_contains_key(table, str2))
                ? hashtab_add(table, str2)
                : hashtab_lookup(table, str2);

        graph_add_edge(graph, id1, id2, path, speed);
        fseek(fp, 1, SEEK_CUR);
    }

    free(str1);
    free(str2);
}

/*Демонстрация графа как матрицы смежности*/
void show_graph(int v, Edge** matrix)
{
    if (matrix == NULL) {
        puts("Нечего показывать.\n");
        return;
    }

    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++)
            printf("{%d %d}\t", matrix[i][j].len, matrix[i][j].speed);
        putchar('\n');
    }
}

/*хэш-функция ELF*/
unsigned int elf_hash(char* s)
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
    HashTable* table = (HashTable*)calloc(HASHTABSIZE, sizeof(HashTable));
    for (int i = 0; i < HASHTABSIZE; i++)
        table[i].key = NULL;

    table->count = 0;
    return table;
}

void hashtab_destroy(HashTable* table)
{
    if (table == NULL)
        return;

    for (int i = 0; i < HASHTABSIZE; i++)
        free(table[i].key);

    free(table);
}

unsigned int hashtab_add(HashTable* hashtab, char* strkey)
{
    unsigned int index = elf_hash(strkey);

    for (int i = index; i < HASHTABSIZE; i++) {
        if (hashtab[i].key != NULL)
            continue;

        hashtab[i].key = calloc(strlen(strkey) + 1, sizeof(char));
        strcpy(hashtab[i].key, strkey);
        hashtab->count++;

        return i;
    }
    printf("Не удалось добавить пункт \"%s\" в хэш-таблицу.\n", strkey);
    return -1;
}

int hashtab_lookup(HashTable* hashtab, char* key)
{
    if (key == NULL || hashtab == NULL)
        return -1;

    int index = elf_hash(key);

    for (int i = index; i < HASHTABSIZE; i++) {
        if (hashtab[i].key == NULL)
            return -1;

        if (strcmp(hashtab[i].key, key) == 0)
            return i;
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

bool hashtab_contains_key(HashTable* table, char* key)
{
    return (hashtab_lookup(table, key) >= 0);
}

void hashtab_print(HashTable* table)
{
    for (int i = 0; i < HASHTABSIZE; i++)
        printf("%d - %s\n", i, hashtab_getkey(table, i));
}