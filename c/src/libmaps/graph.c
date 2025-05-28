/*Файл для работы со структурой графа и входными данными*/
#include <libmaps/graph.h>

Graph* graph_create(int n)
{
    Graph* graph = calloc(n, sizeof(Graph));
    graph->graph_matrix = calloc(n, sizeof(Edge*));
    for (int i = 0; i < n; i++)
        graph->graph_matrix[i] = calloc(n, sizeof(Edge));
    graph->n_verticles = n;
    graph->verticles = calloc(n, sizeof(int));
    graph->visited = calloc(n, sizeof(bool));
    return graph;
}

void graph_destroy(Graph* graph)
{
    free(graph->visited);
    free(graph->verticles);
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

bool is_in_table(HashTable* table, char* key)
{
    return (hashtab_lookup(table, key) >= 0);
}

void graph_init(Graph* graph, HashTable* table, FILE* fp)
{
    int path, speed, count = 0;
    char* str = calloc(MAXSTR + 1, sizeof(char));
    char ch = 0;
    unsigned int v_1, v_2;
    while ((ch = fgetc(fp)) != EOF) {
        ungetc(ch, fp);
        while ((ch = fgetc(fp)) != ' ' && ch != EOF) {
            str[count] = ch;
            count++;
        }
        str[count] = '\0';
        v_1 = (!is_in_table(table, str)) ? hashtab_add(table, str)
                                         : hashtab_lookup(table, str);
        count = 0;
        while ((ch = fgetc(fp)) != ' ' && ch != EOF) {
            str[count] = ch;
            count++;
        }
        str[count] = '\0';
        v_2 = (!is_in_table(table, str)) ? hashtab_add(table, str)
                                         : hashtab_lookup(table, str);

        count = 0;
        fscanf(fp, " %d", &path);
        fscanf(fp, " %d", &speed);
        add_edge(graph, v_1, v_2, path, speed);
        fseek(fp, 1, SEEK_CUR);
    }
    free(str);
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
