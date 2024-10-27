#define STACK_IMPL
#include "../stack.h"

#define GRAPH_IMPL
#include "../graph.h"

#define STRING_IMPL
#include "../string.h"

#define PATH_IMPL
#include "../path.h"

#define CURSOR_IMPL
#include "../cursor.h"

#define MAP_IMPL
#include "../map.h"

#define LIST_IMPL
#include "../list.h"

#define QUEUE_IMPL
#include "../queue.h"

#define PRIORITYQUEUE_IMPL
#include "../priorityQueue.h"

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

int cmpInt(const void *a, const void *b)
{
    int intA = *(int *)a;
    int intB = *(int *)b;
    return intA - intB;
}

void printInt(void *data)
{
    printf("%d ", *(int *)data);
}

void testPriorityQueue()
{
    priorityQueue q = newPriorityQueue(cmpInt);

    if (priorityQueueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be initially empty.\n" RESET);

    int values[] = {5, 3, 10, 1, 4};
    for (int i = 0; i < 5; i++)
        priorityQueuePush(&q, &values[i]);

    int size = priorityQueueGetSize(&q);
    if (size == 5)
        printf(GREEN "Test Passed: Queue size is correct after push operations.\n" RESET);
    else
        printf(RED "Test Failed: Queue size is incorrect. Expected 5, got %d.\n" RESET, size);

    int *peekValue = (int *)priorityQueuePeek(&q);
    if (*peekValue == 10)
        printf(GREEN "Test Passed: Peek returns the correct highest priority element.\n" RESET);
    else
        printf(RED "Test Failed: Peek returned incorrect value. Expected 10, got %d.\n" RESET, *peekValue);

    int expectedOrder[] = {10, 5, 4, 3, 1};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *poppedValue = (int *)priorityQueuePop(&q);
        if (*poppedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *poppedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements popped in correct priority order.\n" RESET);

    if (priorityQueueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is empty after popping all elements.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be empty.\n" RESET);

    priorityQueueFree(&q, NULL);
    printf("Test Completed: Queue memory freed.\n");
}

void testStack()
{
    stack s = newStack();

    if (stackIsEmpty(&s))
        printf(GREEN "Test Passed: Stack is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Stack should be initially empty.\n" RESET);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        stackPush(&s, &values[i]);

    int *topValue = (int *)stackTop(&s);
    if (*topValue == 50)
        printf(GREEN "Test Passed: Top returns the correct last pushed element.\n" RESET);
    else
        printf(RED "Test Failed: Top returned incorrect value. Expected 50, got %d.\n" RESET, *topValue);

    int expectedOrder[] = {50, 40, 30, 20, 10};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *poppedValue = (int *)stackPop(&s);
        if (*poppedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *poppedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements popped in correct LIFO order.\n" RESET);

    if (stackIsEmpty(&s))
        printf(GREEN "Test Passed: Stack is empty after popping all elements.\n" RESET);
    else
        printf(RED "Test Failed: Stack should be empty.\n" RESET);

    stackFree(&s, NULL);
    printf("Test Completed: Stack memory freed.\n");
}

void testQueue()
{
    queue q = newQueue();

    if (queueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be initially empty.\n" RESET);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        enqueue(&q, &values[i]);

    printf("Queue contents after enqueuing: ");
    queuePrint(&q, printInt);
    printf("\n");

    int expectedOrder[] = {10, 20, 30, 40, 50};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *dequeuedValue = (int *)dequeue(&q);
        if (*dequeuedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *dequeuedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements dequeued in correct FIFO order.\n" RESET);

    if (queueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is empty after dequeuing all elements.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be empty.\n" RESET);

    queueFree(&q, NULL);
    printf("Test Completed: Queue memory freed.\n");
}

int main()
{
	testPriorityQueue();
	testStack();
	testQueue();
	return 0;
}
