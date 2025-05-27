#include <libmaps/sort.h>

static void swap(LIST** a, LIST** b)
{
    LIST* temp = *a;
    *a = *b;
    *b = temp;
}

/*Возврат индекса опорного элемента*/
static int partition(LIST** paths_arr, int low, int high, Priority priotity)
{
    Pivot pivot;
    if ((priotity == Shortest) || (priotity == Longest)) {
        pivot.i = paths_arr[high]->path;
    } else {
        pivot.d = paths_arr[high]->time;
    }

    int i = low;
    for (int j = low; j < high; j++) {
        if ((((priotity == Shortest) || (priotity == Longest))
             && paths_arr[j]->path <= pivot.i)
            || ((priotity == Quickest) && paths_arr[j]->time <= pivot.d)) {
            swap(paths_arr + i, paths_arr + j);
            i++;
        }
    }
    swap(paths_arr + i, paths_arr + high);
    return i;
}

static void
sort_paths_priority(LIST** paths_arr, int low, int high, Priority priotity)
{
    if (low < high) {
        int part = partition(paths_arr, low, high, priotity);
        sort_paths_priority(paths_arr, low, part - 1, priotity);
        sort_paths_priority(paths_arr, part + 1, high, priotity);
    }
}

PATHS* sort_paths(PATHS* path, Priority priotity)
{
    LIST** paths_arr = calloc(path->count, sizeof(LIST*));
    if (!paths_arr) {
        return NULL;
    }

    int n = 0;
    for (LIST* cur = path->first; cur; cur = cur->next) {
        paths_arr[n] = cur;
        n++;
    }

    sort_paths_priority(paths_arr, 0, n - 1, priotity);

    PATHS* sorted_paths = def_path_construct();
    if (priotity == Longest) {
        for (int i = n - 1; i >= 0; i--) {
            insert_in_path(sorted_paths, paths_arr[i]);
        }
    } else {
        for (int i = 0; i < n; i++) {
            insert_in_path(sorted_paths, paths_arr[i]);
        }
    }

    free(paths_arr);
    return sorted_paths;
}
