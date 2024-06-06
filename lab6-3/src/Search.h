#pragma once

#include <stdio.h>

#include "Node.h"

void PrintChildren(Node* root, char* Word, bool* check);
void Find(Node* root, char* prefix, char* Word, int WordLen, bool* check);
void NoneCheck(bool* check);
void SearchForPrefix(Node* root, char* prefix);
