#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_ROWS 20

struct data
{
    int task_id;
    int share_id;
    int pc_id;
    int tss;
    struct data *next;
}*head=NULL;

struct data *create_node(int task_id, int share_id, int pc_id, int tss)
{
    struct data *new_node = (struct data *)malloc(sizeof(struct data));
    new_node->task_id = task_id;
    new_node->share_id = share_id;
    new_node->pc_id = pc_id;
    new_node->tss = tss;
    new_node->next = NULL;
    return new_node;
}

void sorted_insert(struct data **head_ref, struct data *new_node)
{
    struct data *current;
    if (*head_ref == NULL || (*head_ref)->share_id >= new_node->share_id)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        current = *head_ref;
        while (current->next != NULL && current->next->share_id < new_node->share_id)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
int min(int arr[], int n)
{
    int min = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

int share_func(int et[20], int sm[20], int dl[20], int dlc[20])
{
    int g=min(dlc,20);
    for (int i = 0; i<20;i++){
        sm[i]=(et[i]*g)/dl[i];
    }
    return sm[20];
}
int main()
{
    int ex_t1[MAX_ROWS], ex_t2[MAX_ROWS], ex_t3[MAX_ROWS], ex_t4[MAX_ROWS];
    int temp1[MAX_ROWS], temp2[MAX_ROWS], temp3[MAX_ROWS], temp4[MAX_ROWS];
    int share1[MAX_ROWS], share2[MAX_ROWS], share3[MAX_ROWS], share4[MAX_ROWS];
    int deadline[MAX_ROWS], rem_deadline[MAX_ROWS];
    int row = 0;
    int test;
    FILE *fp;

    fp = fopen("Set_1_Input_m_2_U_40_n_20.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    while (row < MAX_ROWS && fscanf(fp, "%d\t%d\t%d\t%d\t%d\t%d", &test, &ex_t1[row], &ex_t2[row], &deadline[row], & temp1[row], &temp2[row]) == 6)
    {
        row++;
    }

    fclose(fp);
    row=0;
    while (row < MAX_ROWS)
    {
        ex_t3[row] = ex_t1[row]+ 10;
        ex_t4[row] = ex_t2[row] + 10;
        temp3[row] = temp1[row] + 10;
        temp4[row] = temp2[row] + 10;
        rem_deadline[row]=deadline[row];
        row++;
    }
    
    for (row = 0; row < MAX_ROWS; row++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",row, ex_t1[row], ex_t2[row], ex_t3[row], ex_t4[row], deadline[row], temp1[row], temp2[row], temp3[row], temp4[row]);
        printf("\n");
    }
    
    share_func(ex_t1,share1,deadline,rem_deadline);
    for (row = 0; row < MAX_ROWS; row++)
    {
        struct data *new_node;
        // new_node = create_node(row, share1[row], 1, temp1[row]);
        // sorted_insert(&head, new_node);
        new_node = create_node(row, ex_t1[row], 1, temp1[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row + 20, ex_t2[row], 2, temp2[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row + 40, ex_t3[row], 3, temp3[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row + 60, ex_t4[row], 4, temp4[row]);
        sorted_insert(&head, new_node);
    }

    // struct data *ptr = head;
    // while (ptr != NULL)
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }

    return 0;
}
