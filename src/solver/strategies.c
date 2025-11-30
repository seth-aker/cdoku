#include "strategies.h";

StepNode* pushStep(StepNode* head, Step newStep) {
  StepNode* newNode = malloc(sizeof(StepNode));
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
  newNode->next == NULL;
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