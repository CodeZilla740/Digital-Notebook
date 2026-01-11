#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct notebook {
    int page_no;
    char content[500];
    struct notebook *llink;
    struct notebook *rlink;
} NB;

NB *current = NULL;

void bookmarkPage(int **bookMark, int *size, int *top)
{
    if (current == NULL)
    {
        printf("No current page to bookmark\n");
        return;
    }
    if (*bookMark == NULL)
    {
        *bookMark = (int *)malloc((*size) * sizeof(int));
        if (*bookMark == NULL)
        {
            printf("No memory allocation\n");
            exit(1);
        }
    }
    if (*top == (*size) - 1)
    {
        *size = (*size) * 2;
        int *temp = (int *)realloc(*bookMark, (*size) * sizeof(int));
        if (temp == NULL)
        {
            printf("Memory reallocation failed\n");
            return;
        }
        *bookMark = temp;
    }
    (*top)++;
    (*bookMark)[*top] = current->page_no;
    printf("Bookmarked page is: %d\n", current->page_no);
    printf("Bookmarked page content: %s\n", current->content);
    printf("The bookmark stack is updated\n");
}

NB *get_page()
{
    NB *temp = (NB *)malloc(sizeof(NB));
    if (temp == NULL)
    {
        printf("No memory to create new page\n");
        exit(0);
    }
    printf("Enter Page No.: ");
    scanf("%d", &temp->page_no);
    getchar();

    printf("Enter the content of the page: ");
    fgets(temp->content, 500, stdin);
    temp->content[strcspn(temp->content, "\n")] = '\0';

    temp->llink = NULL;
    temp->rlink = NULL;

    return temp;
}

NB *addPage(NB *first)
{
    NB *new = get_page();
    printf("Page added successfully.\n");
    if (first == NULL)
    {
        first = new;
        current = new;
        return first;
    }
    NB *temp = first;
    while (temp->rlink != NULL)
    {
        temp = temp->rlink;
    }
    temp->rlink = new;
    new->llink = temp;
    return first;
}

NB *displayCurrent()
{
    if (current == NULL)
    {
        printf("No pages to display.\n");
        return NULL;
    }
    printf("Current page: %d\n", current->page_no);
    printf("Content: %s\n", current->content);
    return current;
}

NB *nextPage()
{
    if (current == NULL)
    {
        printf("No pages added.\n");
        return NULL;
    }
    if (current->rlink == NULL)
    {
        printf("This is the last page.\n");
        return current;
    }
    current = current->rlink;
    printf("The next page is:\n");
    printf("Page No: %d\n", current->page_no);
    printf("Content: %s\n", current->content);
    return current;
}

NB *prevPage()
{
    if (current == NULL)
    {
        printf("No pages added.\n");
        return NULL;
    }
    if (current->llink == NULL)
    {
        printf("This is the first page.\n");
        return current;
    }
    current = current->llink;
    printf("The previous page is:\n");
    printf("Page No: %d\n", current->page_no);
    printf("Content: %s\n", current->content);
    return current;
}

/* Set current to the last bookmarked page (look up by page number in the list) */
void gotoBookMark(int *bookMark, int top, NB *first)
{
    if (top < 0 || bookMark == NULL)
    {
        printf("There's no bookmark\n");
        return;
    }
    int pg = bookMark[top];
    NB *t = first;
    while (t != NULL)
    {
        if (t->page_no == pg)
        {
            current = t;
            printf("Moved to bookmarked page number %d\n", pg);
            printf("Content: %s\n", current->content);
            return;
        }
        t = t->rlink;
    }
    printf("Bookmarked page %d not found in notebook\n", pg);
}

/* Delete current node; return updated 'first' pointer. Sets 'current' appropriately. */
NB *deleteCurrentPage(NB *first, NB *last)
{
    if (current == NULL)
    {
        printf("No page to delete, Notebook is empty or no current selected.\n");
        return first;
    }
    NB *temp = current;

    /* single node */
    if (first == last && first != NULL)
    {
        printf("Deleted Page No.: %d\n", temp->page_no);
        printf("Deleted Page's content: %s\n", temp->content);
        free(temp);
        current = NULL;
        return NULL;
    }
    else if (current == first)
    {
        first = first->rlink;
        if (first) 
            first->llink = NULL;
        current = first;
    }
    else if (current == last)
    {
        last = last->llink;
        if (last) 
            last->rlink = NULL;
        current = last;
    }
    else
    {
        temp->llink->rlink = temp->rlink;
        temp->rlink->llink = temp->llink;
        current = temp->rlink;
    }

    printf("Deleted Page No.: %d\n", temp->page_no);
    printf("Deleted Page's content: %s\n", temp->content);
    free(temp);
    return first;
}

NB *showNotebook(NB *first)
{
    NB *temp = first;
    if (first == NULL)
    {
        printf("Notebook is empty.\n");
        return NULL;
    }
    while (temp != NULL)
    {
        printf("Page No.: %d\n", temp->page_no);
        printf("Content: %s\n", temp->content);
        temp = temp->rlink;
    }
    return first;
}

int main()
{
    int size = 1;
    int top = -1;
    int choice;
    NB *first = NULL;
    NB *last = NULL;
    int *bookMark = NULL;

    for (;;)
    {
        printf("\n====================================");
        printf("\n      DIGITAL NOTEBOOK MENU");
        printf("\n====================================");
        printf("\n1. Add new page");
        printf("\n2. Show current page");
        printf("\n3. Next page");
        printf("\n4. Previous page");
        printf("\n5. Bookmark this page");
        printf("\n6. Go to last bookmarked page");
        printf("\n7. Delete current page");
        printf("\n8. Show notebook summary");
        printf("\n9. Exit");
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            exit(1);
        }
        getchar();

        switch (choice)
        {
        case 1:
            first = addPage(first);
            /* recompute last */
            if (first == NULL)
                last = NULL;
            else
            {
                NB *t = first;
                while (t->rlink != NULL)
                    t = t->rlink;
                last = t;
            }
            break;
        case 2:
            current = displayCurrent();
            break;
        case 3:
            nextPage();
            break;
        case 4:
            prevPage();
            break;
        case 5:
            bookmarkPage(&bookMark, &size, &top);
            break;
        case 6:
            gotoBookMark(bookMark, top, first);
            break;
        case 7:
            first = deleteCurrentPage(first, last);
            /* recompute last after deletion */
            if (first == NULL)
                last = NULL;
            else
            {
                NB *t = first;
                while (t->rlink != NULL)
                    t = t->rlink;
                last = t;
            }
            break;
        case 8:
            first = showNotebook(first);
            break;
        case 9:
            printf("Notebook closed.\n");
            /* free remaining nodes (optional cleanup) */
            while (first != NULL)
            {
                NB *tmp = first;
                first = first->rlink;
                free(tmp);
            }
            free(bookMark);
            exit(0);
        default:
            printf("\nInvalid choice.\n");
        }
    }
    return 0;
}
