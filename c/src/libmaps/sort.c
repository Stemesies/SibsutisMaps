#include <libmaps/sort.h>

static void swap(Path** a, Path** b)
{
    Path* temp = *a;
    *a = *b;
    *b = temp;
}

/*Возврат индекса опорного элемента*/
static int partition(Path** paths_arr, int low, int high, Priority priotity)
{
    Pivot pivot;
    if ((priotity == SHORTEST) || (priotity == LONGEST)) {
        pivot.i = paths_arr[high]->path;
    } else {
        pivot.d = paths_arr[high]->time;
    }

    int i = low;
    for (int j = low; j < high; j++) {
        if ((((priotity == SHORTEST) || (priotity == LONGEST))
             && paths_arr[j]->path <= pivot.i)
            || ((priotity == QUICKEST) && paths_arr[j]->time <= pivot.d)) {
            swap(paths_arr + i, paths_arr + j);
            i++;
        }
    }
    swap(paths_arr + i, paths_arr + high);
    return i;
}

static void
sort_paths_priority(Path** paths_arr, int low, int high, Priority priotity)
{
    if (low < high) {
        int part = partition(paths_arr, low, high, priotity);
        sort_paths_priority(paths_arr, low, part - 1, priotity);
        sort_paths_priority(paths_arr, part + 1, high, priotity);
    }
}

PathsContain* sort_paths(PathsContain* path, Priority priotity)
{
    Path** paths_arr = calloc(path->count, sizeof(Path*));
    if (!paths_arr) {
        return NULL;
    }

    int n = 0;
    for (Path* cur = path->first; cur; cur = cur->next) {
        paths_arr[n] = cur;
        n++;
    }

    sort_paths_priority(paths_arr, 0, n - 1, priotity);

    PathsContain* sorted_paths = def_path_contain_construct();
    if (priotity == LONGEST) {
        for (int i = n - 1; i >= 0; i--) {
            insert_in_path_contain(sorted_paths, paths_arr[i]);
        }
    } else {
        for (int i = 0; i < n; i++) {
            insert_in_path_contain(sorted_paths, paths_arr[i]);
        }
    }

    free(paths_arr);
    return sorted_paths;
}
