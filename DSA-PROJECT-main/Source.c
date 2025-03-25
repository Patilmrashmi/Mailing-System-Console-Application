#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<Windows.h>

#define MAX_EMAILS 100
#define EMAIL_FILE "emails.txt"
#define MAX_NAME_LENGTH 100
#define MAX_NAMES 100
#define LINE_SIZE 256
#define MAX_EMAIL_LENGTH 1000
char names[MAX_NAMES][MAX_NAME_LENGTH];
char buffer[MAX_NAME_LENGTH];


typedef struct {
    char sender[100];
    char subject[100];
    char message[1000];
} Email;

typedef struct {
    int front;
    int rear;
    int size;
    char* emails[MAX_EMAILS];
} Queue;

char mails[100];

// Initialize the queue
void init(Queue* q) {
    q->front = 0;
    q->rear = MAX_EMAILS - 1;
    q->size = 0;
}

// Check if the queue is empty
int isEmpty(Queue* q) {
    return q->size == 0;
}

// Check if the queue is full
int isFull(Queue* q) {
    return q->size == MAX_EMAILS;
}

// Add an email file to the queue
void enqueue(Queue* q, char* fileName) {
    if (!isFull(q)) {
        q->rear = (q->rear + 1) % MAX_EMAILS;
        q->emails[q->rear] = fileName;
        q->size++;
        printf("%s\n", fileName);
    }
    else {
        printf("Error: queue is full\n");
    }
}

// Remove an email file from the queue
void dequeue(Queue* q, char* fileName) 
{
    printf("%s\n", fileName);
    if (!isEmpty(q)) 
    {
        int r;
        fileName = q->emails[q->front];
        q->front = (q->front + 1) % MAX_EMAILS;
        q->size--;
       /* r = remove(fileName);
        if (r == 0)
            printf("Successfully deleted");
        else
            printf("Error: File not found.\n");*/
    }
    else {
       // printf("Error: Queue is empty jftdh\n");
    }
}

void delete_file(char* file)
{
    int r = remove(file);
    if (r == 0)
        printf("Successfully Deleted.\n");
    else
        printf("Error : File not found.\n");

}

// Write email data to file
void writeEmailToFile(Email email, char* fileName) {
    FILE* file = fopen(fileName, "w");
    fprintf(file, "Sender: %s\n", email.sender);
    fprintf(file, "Subject: %s\n", email.subject);
    fprintf(file, "Message: %s\n", email.message);
    fclose(file);
}


void readEmailFromFile(char* fileName) {
    char emails[MAX_EMAILS][MAX_EMAIL_LENGTH];
    char buffer[MAX_EMAIL_LENGTH];
    int email_count = 0;
    FILE* fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("No Mails is present.\n");
        return 1;
    }

    while (fgets(buffer, MAX_EMAIL_LENGTH, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(emails[email_count], buffer);
        email_count++;
    }
    fclose(fp);
    for (int i = 0; i < email_count; i++) {
        printf("%s\n", emails[i]);
    }
}


void store_queue(char *filename,Queue* q)
{
    FILE* file = fopen("mails.txt", "a");
    fprintf(file, "%s\n",filename);
    fclose(file);
}

void modify_queue(Queue* q,char *TARGET_STRING) {
    char line[LINE_SIZE];
    char temp_file_name[] = "temp.txt";
    FILE* fp_read = fopen("mails.txt", "r");
    FILE* fp_write = fopen(temp_file_name, "w");
    while (fgets(line, LINE_SIZE, fp_read)) 
    {
        if (strstr(line, TARGET_STRING) == NULL)
        {
            fputs(line, fp_write);
        }
    }
    fclose(fp_read);
    fclose(fp_write);
    remove("mails.txt");
    rename(temp_file_name, "mails.txt");
    return 0;
}


void read_name(Queue* q)
{
    int name_count = 0;
    FILE* fp;
    fp = fopen("mails.txt", "r");
    char spinner[] = { '|', '/', '-', '\\' };
    printf("Loading the old Emails ");
    init(&q);
    for (int i = 0; i < 20; i++) {
        printf("\rLoading the old Emails %c", spinner[i % 4]);
        fflush(stdout);
        Sleep(1);
    }
    printf("\r\n");
    if (fp == NULL)
    {
        printf("Could not open old file.\n");
        return 1;
    }
    while (fgets(buffer, MAX_NAME_LENGTH, fp) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(names[name_count], buffer);
        name_count++;
        //enqueue(&q, ("%s\n", names[name_count]));
    }
    for (int i = 0; i < name_count; i++)
    {
        //printf("%s\n", names[i]);
        enqueue(&q, ("%s\n", names[i]));
    }
    fclose(fp);
}

void main()
{
    int ch = 0;
    char *filename[100];
    Email email;
    Queue q;
    init(&q);
    char spinner[] = { '|', '/', '-', '\\' };
    for (int i = 0; i < 20; i++) {
        printf("\rLoading %c", spinner[i % 4]);
        fflush(stdout);
        Sleep(1);
    }
    printf("\r\n");
    read_name(&q);
    printf("\t\t\t\t\t\tWelcome to Mailing Services\n");
    printf("1.Create\n2.Delete\n3.Read\n4.Exit\n5.Check - Empty\n6.Check - Full");
    while (ch != 4)
    {
        printf("\nEnter choice : ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("Enter sender : ");
            gets(email.sender);
            gets(email.sender);
            printf("Enter subject : ");
            gets(email.subject);
            printf("Enter message : ");
            gets(email.message);
            printf("Enter the file name to store the mail : ");
            gets(filename);
            enqueue(&q, &filename);
            writeEmailToFile(email,&filename);
            store_queue(&filename,&q);
            break;
        case 2:
            printf("Enter filename to delete : ");
            scanf("%s", filename);
            delete_file(filename);
            dequeue(&q,&filename);
            modify_queue(&q,&filename);
            break;
        case 3:
            printf("Enter filename to read : ");
            scanf("%s", filename);
            readEmailFromFile(&filename);
            break;
        case 5:
            if (isEmpty(&q))
                printf("Mail Inbox empty.\n");
            else
                printf("Mail Inbox is not empty\n");
            break;
        case 6:
            if (isFull(&q))
                printf("Mail Inbox is full.\n");
            else
                printf("Mail Inbox is not full.\n");
            break;
        default:
            printf("Invalid choice ");
            break;
        }
    }
}