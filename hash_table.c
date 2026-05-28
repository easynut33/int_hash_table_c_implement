#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1000003
#define MAX_LOAD_FACTOR 0.75

typedef struct Node {
    int key;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    int capacity;
    int size;
} HashTable;

int hash(int key, int capacity);
HashTable *create_hash_table(void);
Node *create_node(int key);
void resize_hash_table(HashTable *table);
void free_hash_table(HashTable *table);
void print_hash_table(HashTable *table);
void insert(HashTable *table, int key);
int search(HashTable *table, int key);

int main(void)
{
    int n;
    int m;
    HashTable *table = create_hash_table();

    printf("=== Hash Table Search Program ===\n");
    printf("This program stores N integers in a hash table.\n");
    printf("After input, it prints the whole hash table status.\n");
    printf("For each search value, it prints whether the key was found.\n\n");

    printf("Enter the number of values (N): ");
    if (scanf("%d", &n) != 1) {
        free_hash_table(table);
        return 0;
    }

    printf("Enter %d integer value(s):\n", n);
    for (int i = 0; i < n; i++) {
        int value;
        scanf("%d", &value);
        insert(table, value);
    }

    printf("\nHash table created. Stored unique values: %d\n", table->size);
    print_hash_table(table);

    printf("Enter the number of search values (M): ");
    if (scanf("%d", &m) != 1) {
        free_hash_table(table);
        return 0;
    }

    printf("Enter %d search value(s):\n", m);
    printf("\nSearch results:\n");
    for (int i = 0; i < m; i++) {
        int value;
        scanf("%d", &value);

        if (search(table, value)) {
            printf("%d: Found!\n", value);
        } else {
            printf("%d: Not Found!\n", value);
        }
    }

    free_hash_table(table);
    return 0;
}

int hash(int key, int capacity)
{
    return (key % capacity + capacity) % capacity;
}

HashTable *create_hash_table(void)
{
    HashTable *table = malloc(sizeof(HashTable));

    if (table == NULL) {
        exit(EXIT_FAILURE);
    }

    table->capacity = INITIAL_CAPACITY;
    table->size = 0;
    table->buckets = calloc(table->capacity, sizeof(Node *));

    if (table->buckets == NULL) {
        free(table);
        exit(EXIT_FAILURE);
    }

    return table;
}

Node *create_node(int key)
{
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }

    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}

void resize_hash_table(HashTable *table)
{
    int old_capacity = table->capacity;
    int new_capacity = old_capacity * 2;
    Node **old_buckets = table->buckets;
    Node **new_buckets = calloc(new_capacity, sizeof(Node *));

    if (new_buckets == NULL) {
        return;
    }

    table->buckets = new_buckets;
    table->capacity = new_capacity;

    for (int i = 0; i < old_capacity; i++) {
        Node *current = old_buckets[i];

        while (current != NULL) {
            Node *next = current->next;
            int new_index = hash(current->key, table->capacity);

            current->next = table->buckets[new_index];
            table->buckets[new_index] = current;
            current = next;
        }
    }

    free(old_buckets);
}

void free_hash_table(HashTable *table)
{
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->capacity; i++) {
        Node *current = table->buckets[i];

        while (current != NULL) {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }

    free(table->buckets);
    free(table);
}

void print_hash_table(HashTable *table)
{
    if (table == NULL) {
        return;
    }

    printf("\n========== Hash Table Status ==========\n");
    printf("Total Size: %d\n", table->size);
    printf("Capacity: %d\n", table->capacity);

    for (int i = 0; i < table->capacity; i++) {
        Node *current = table->buckets[i];

        if (current == NULL) {
            continue;
        }

        printf("Bucket [%d]: ", i);

        while (current != NULL) {
            printf("%d", current->key);
            current = current->next;

            if (current != NULL) {
                printf(" -> ");
            }
        }

        printf("\n");
    }

    printf("=======================================\n\n");
}

void insert(HashTable *table, int key)
{
    int index = hash(key, table->capacity);
    Node *current = table->buckets[index];
    Node *new_node;

    while (current != NULL) {
        if (current->key == key) {
            return;
        }

        current = current->next;
    }

    new_node = create_node(key);
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;

    if ((double)table->size / table->capacity > MAX_LOAD_FACTOR) {
        resize_hash_table(table);
    }
}

int search(HashTable *table, int key)
{
    int index = hash(key, table->capacity);
    Node *current = table->buckets[index];

    while (current != NULL) {
        if (current->key == key) {
            return 1;
        }

        current = current->next;
    }

    return 0;
}
