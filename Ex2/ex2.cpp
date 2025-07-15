#include <iostream>
#include "queue.h"
#include <cstring>

using namespace std;

//Constructing the pattern for each node our binary tree has
struct Node {
  int val;
  Node *left, *right;
};

//We declare our man variables
int order[1001], n, count;
Node *R = NULL;

void readingAndConversion () {
  char input[10001];

  cin.getline(input, 10001);

  char sep[] = " ,.[]=";
  char *p = strtok(input, sep);
  while (p != NULL) {
    if (strcmp(p, "0") == 0) {
      order[n] = 0;
      n++;
    }

    if (strcmp(p, "null") == 0) {
      order[n] = -1;
      n++;
    }

    p = strtok(NULL, sep);
  }
}

void transformation () {
  Queue<Node*> q;

  R = new Node;
  R -> val = order[0];
  R -> left = NULL;
  R -> right = NULL;
  q.enqueue(R);

  int iterator = 1;
  while (iterator < n && !q.isEmpty()) {
    Node *temp = q.peek();
    q.dequeue();

    if (iterator < n && order[iterator] == 0) {
      temp -> left = new Node;
      temp -> left->val = order[iterator];
      temp -> left->left = NULL;
      temp -> left->right = NULL;
      q.enqueue(temp->left);
      iterator++;
    } else if (iterator < n) {
      temp ->left = NULL;
      iterator++;
    }

    if (iterator < n && order[iterator] == 0) {
      temp -> right = new Node;
      temp -> right ->val = order[iterator];
      temp -> right ->left = NULL;
      temp -> right ->right = NULL;
      q.enqueue(temp->right);
      iterator++;
    } else if (iterator < n) {
      temp ->right = NULL;
      iterator++;
    }
  }
}

int Dragons (Node * R) {
  if (R == NULL) {
    return 1;
  }

  int left = Dragons(R -> left);
  int right = Dragons(R -> right);

  if (left == 1 && right == 1) {
    return 2;
  } else if (left == 2) {
    count++;
    return 3;
  } else if (right == 2) {
    count++;
    return 3;
  }

  return 1;
}

int main () {
  readingAndConversion();

  transformation();

  int counter = Dragons(R);
  if (counter == 2) {
    count += 1;
  }

  if (count == 1) {
    cout << "1 Dragon";
  } else {
    cout << count << " Dragons";
  }

  return 0;
}


