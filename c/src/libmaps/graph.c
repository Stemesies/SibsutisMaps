#include <libmaps/graph.h>
// #include <uchar.h>

GRAPH *graph_create(int n)
{
    GRAPH *graph = malloc(sizeof(GRAPH) * n);
    graph->graph_matrix = malloc(sizeof(EDGE *) * n);
    for (int i = 0; i < n; i++)
        graph->graph_matrix[i] = malloc(n * sizeof(EDGE));
    graph->n_verticles = n;
    graph->verticles = calloc(n, sizeof(int));
    graph->visited = calloc(n, sizeof(bool));
    return graph;
}

void graph_destroy(GRAPH *graph)
{
    free(graph->visited);
    free(graph->verticles);
    for (int i = 0; i < graph->n_verticles; i++)
    {
        free(graph->graph_matrix[i]);
    }
    free(graph->graph_matrix);
    free(graph);
}

void add_edge(GRAPH *graph, unsigned int i, unsigned int j, int len, int speed)
{
    graph->graph_matrix[i][j].len = len;
    graph->graph_matrix[i][j].speed = speed;
    graph->graph_matrix[j][i].len = len;
    graph->graph_matrix[j][i].speed = speed;
}

// /*хэш-функция КР*/
// unsigned int KRHash(char *key)
// {
//     unsigned int h = 0, hash_mul = 31;
//     while (*key)
//         h = h * hash_mul + (unsigned int)*key++;
//     return h % HASHTAB_SIZE;
// }

/*хэш-функция ELF*/
unsigned int ELFHash(char *s)
{
    unsigned int h = 0, g;
    while (*s)
    {
        h = (h << 4) + (unsigned int)*s++;
        g = h & 0xF0000000L;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % HASHTAB_SIZE;
}

HASH *hashtab_create()
{
    HASH *a = (HASH *)malloc(sizeof(HASH) * HASHTAB_SIZE);
    for (int i = 0; i < HASHTAB_SIZE; i++)
    {
        a[i].key = NULL;
        // a[i].next = NULL;
    }
    return a;
}

void hashtab_destroy(HASH *table)
{
    for (int i = 0; i < HASHTAB_SIZE; i++)
    {
        if (!table[i].key)
            continue;
        free(table[i].key);
    }
    free(table);
}

unsigned int hashtab_add(HASH *hashtab, char *key)
{
    // HASH *node = malloc(sizeof(*node));
    // unsigned int index = KRHash(key);
    unsigned int index = ELFHash(key);

    if (hashtab[index].key != NULL)
    {
        for (int i = index, r = 0; r < HASHTAB_SIZE && i >= r && i < HASHTAB_SIZE; r++)
        {
            (r % 2 == 0) ? (i += r) : (i -= r);

            if (hashtab[i].key == NULL)
            {
                strcpy(hashtab[i].key, key);
                index = i;
            }
        }
    }
    else
    {
        hashtab[index].key = calloc(MAXSTR, sizeof(char));
        strcpy(hashtab[index].key, key);
    }

    return index;
}

int hashtab_lookup(HASH *hashtab, char *key)
{
    // HASH node;
    // int index = KRHash(key);
    if (!key)
        return -1;
    int index = ELFHash(key);
    if (hashtab[index].key == NULL)
        return -1;

    for (int i = index, r = 0; r < HASHTAB_SIZE && i >= r && i < HASHTAB_SIZE; r++)
    {
        (r % 2 == 0) ? (i += r) : (i -= r);

        if (strcmp(hashtab[i].key, key) == 0)
            return i;
    }

    return -1;
}

bool is_in_table(HASH *table, char *key)
{
    return (hashtab_lookup(table, key) >= 0);
}

void graph_init(GRAPH *graph, HASH *table, FILE *fp)
{
    int path, speed, count = 0;
    char *str = calloc(MAXSTR, sizeof(char));
    char ch = 0;
    unsigned int v_1, v_2;
    while ((ch = fgetc(fp)) != EOF)
    {
        ungetc(ch, fp);
        while ((ch = fgetc(fp)) != ' ' && ch != EOF)
        {
            str[count] = ch;
            count++;
        }
        str[count] = '\0';
        v_1 = (!is_in_table(table, str)) ? hashtab_add(table, str) : ELFHash(str);
        count = 0;
        // fseek(fp, 1, SEEK_CUR);
        while ((ch = fgetc(fp)) != ' ' && ch != EOF)
        {
            str[count] = ch;
            count++;
        }
        str[count] = '\0';
        v_2 = (!is_in_table(table, str)) ? hashtab_add(table, str) : ELFHash(str);
        count = 0;
        // fseek(fp, 1, SEEK_CUR);
        fscanf(fp, " %d", &path);
        fscanf(fp, " %d", &speed);
        add_edge(graph, v_1, v_2, path, speed);
        fseek(fp, 1, SEEK_CUR);
    }
}

/*Демонстрация графа как матрицы смежности*/
void show_graph(int v, EDGE **mass)
{
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            printf("%d\t", mass[i][j].len);
        }
        putchar('\n');
    }
}
