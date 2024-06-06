#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

void FirstOutput(long *distance_spt, int num_of_vertices);
void SecondOutput(int *adjacencyMatrix, long *distance_spt, int *parent, int num_of_vertices, int start, int finish);
