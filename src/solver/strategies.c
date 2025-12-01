#include "strategies.h"
StepNode* initStepList() {
   StepNode* newNode = calloc(1, sizeof(StepNode));
   if(newNode == NULL) {
    perror("Failed to allocate memory for new Step");
    exit(EXIT_FAILURE);
  }
  Step emptyStep = {
    .colIndex = -1,
    .rowIndex = -1,
    .value = -1
  };
  newNode->next = NULL;
  newNode->prev = NULL;
  newNode->step = emptyStep;
  return newNode;
}
StepNode* appendStep(StepNode* head, Step newStep) {
  if(head == NULL) {
    head = initStepList();
    head->step = newStep;
    return head;
  }
  StepNode* newNode = calloc(1, sizeof(StepNode));
  if(newNode == NULL) {
    perror("Failed to allocate memory for new Step");
    exit(EXIT_FAILURE);
  }
  StepNode* current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  
  current->next = newNode;
  newNode->step = newStep;
  newNode->prev = current;
  newNode->next = NULL;
  return newNode;
}

void freeStepList(StepNode* head) {
  StepNode* current = head;
  StepNode* next;
  while(current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}