#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printArray2(long * arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(stdout, "%ld\n", arr[i]);
    }
    fprintf(stdout, "\n");
}

long min(long a, long b)
{
    if (a < b) return a;
    else return b;
}

long max(long a, long b)
{
    if (a > b) return a;
    else return b;
}

// int partition(long * arr, long mean, long long * meanl, long long * meanr, int lb, int ub)
// {
//     // if (arr[lb] == arr[ub] && ub - lb == 1)
//     //     return lb;
//
//     int down = lb;
//     int up = ub;
//
//     while (down < up)
//     {
//         //fprintf(stdout, "partition\n");
//         while (arr[down] <= mean && down < ub)
//             down++;
//             //*meanl += arr[down++];
//         while (arr[up] > mean && up > lb)
//             up--;
//             //*meanr += arr[up--];
//
//         if (down < up)
//         {
//             long temp = arr[down];
//             arr[down] = arr[up];
//             arr[up] = temp;
//         }
//         else
//         {
//             *meanl = *meanl / (up - lb + 1);
//             *meanr = *meanr / (ub - up + 1);
//             //fprintf(stdout, "%lld, %lld, %d\n", *meanl, *meanr, up);
//             return up;
//         }
//     }
//     return up;
// }
//
// void Quick_Sort_Helper(long * arr, long mean, int lb, int ub)
// {
//     if (lb >= ub)
//     {
//         return;
//     }
//
//     long first = arr[lb];
//     long median = arr[lb+(ub-lb)/2];
//     long end = arr[ub];
//     long pivot;
//
//     if (first >= min(median, end) && first <= max(median, end))
//         pivot = first;
//     else if (median >= min(first, end) && median <= max(first, end))
//         pivot = median;
//     else
//         pivot = end;
//
//     //fprintf(stdout, "%ld, %d, %d\n", mean, lb, ub);
//     //printArray2(arr, 10);
//     long long meanl = 0;
//     long long meanr = 0;
//     int part_ind = partition(arr, pivot, &meanl, &meanr, lb, ub);
//
//     Quick_Sort_Helper(arr, (long) meanl, lb, part_ind);
//     Quick_Sort_Helper(arr, (long) meanr, part_ind+1, ub);
// }
//
// void Quick_Sort(long *Array, int Size)
// {
//     long first = Array[0];
//     long median = Array[Size/2];
//     long end = Array[Size - 1];
//
//     if (first >= min(median, end) && first <= max(median, end))
//         Quick_Sort_Helper(Array, first, 0, Size - 1);
//     else if (median >= min(first, end) && median <= max(first, end))
//         Quick_Sort_Helper(Array, median, 0, Size - 1);
//     else
//         Quick_Sort_Helper(Array, end, 0, Size - 1);
// }

int partition(long * arr, long pivot, int lb, int ub)
{
    int up = ub;
    int down = lb;

    while (down < up)
    {
        while(arr[down] < pivot && down < ub)
            down++;
        while(arr[up] > pivot && up > lb)
            up--;

        if (down < up)
        {
            long temp = arr[up];
            arr[up] = arr[down];
            arr[down] = temp;
        }
        else
        {
            return up;
        }
    }
    return up;
}

void Quick_Sort_Helper(long * arr, int lb, int ub)
{
    if (lb >= ub)
        return;

    long first = arr[lb];
    long mid = arr[lb + (ub - lb) / 2];
    long end = arr[ub];

    long pivot;
    if (first >= min(mid, end) && first <= max(mid, end))
        pivot = first;
    else if (mid >= min(first, end) && mid <= max(first, end))
        pivot = mid;
    else
        pivot = end;

    int part = partition(arr, pivot, lb, ub);
    Quick_Sort_Helper(arr, lb, part);
    Quick_Sort_Helper(arr, part+1, ub);
}

void Quick_Sort(long *Array, int Size)
{
    Quick_Sort_Helper(Array, 0, Size - 1);
}


void merge(long * arr, long * temp, int lb, int mid, int ub)
{
    int i = lb;
    int j = mid + 1;
    int m;
    for (m = lb; m <= ub; m++)
    {
        if (i > mid) arr[m] = temp[j++];
        else if (j > ub) arr[m] = temp[i++];
        else if (temp[j] < temp[i]) arr[m] = temp[j++];
        else arr[m] = temp[i++];
    }
}

void Merge_Sort(long *Array, int Size)
{
    int group = 1;
    long * temp = malloc(sizeof(long) * Size);
    memcpy(temp, Array, Size * sizeof(long));
    //fprintf(stdout, "temp:\n");
    //printArray2(temp, Size);

    long * sorted = Array;
    long * merged = temp;
    long * listswitch;

    while (group < Size)
    {
        int i = 0;
        while (i < Size - group)
        {
            merge(merged, sorted, i, i + group - 1, min(i + 2*group - 1, Size - 1));
            i += group * 2;
        }
        for ( ; i < Size; i++)
        {
            merged[i] = sorted[i];
        }

        listswitch = sorted;
        sorted = merged;
        merged = listswitch;
        //fprintf(stdout, "group: %d\n", group);
        //printArray2(sorted, Size);
        group = group * 2;
    }

    if (sorted == temp)
    {
        memcpy(Array, sorted, Size * sizeof(long));
    }

    return;
}
