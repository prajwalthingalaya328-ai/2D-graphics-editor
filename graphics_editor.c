#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS 30
#define COLS 80
#define MAX_OBJECTS 100

typedef struct
{
    int type;
    int p[8];
} Object;

char canvas[ROWS][COLS];
Object objects[MAX_OBJECTS];
int objectCount = 0;

void clearCanvas()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas()
{
    printf("\n");

    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void plot(int x, int y)
{
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = '*';
}

void drawLineCanvas(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        plot(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangleCanvas(int x, int y, int width, int height)
{
    drawLineCanvas(x, y, x + width, y);
    drawLineCanvas(x, y, x, y + height);
    drawLineCanvas(x + width, y, x + width, y + height);
    drawLineCanvas(x, y + height, x + width, y + height);
}

void drawTriangleCanvas(int x1, int y1,
                        int x2, int y2,
                        int x3, int y3)
{
    drawLineCanvas(x1, y1, x2, y2);
    drawLineCanvas(x2, y2, x3, y3);
    drawLineCanvas(x3, y3, x1, y1);
}

void drawCircleCanvas(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(y >= x)
    {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);

        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        x++;

        if(d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void redrawAll()
{
    clearCanvas();

    for(int i = 0; i < objectCount; i++)
    {
        Object obj = objects[i];

        switch(obj.type)
        {
            case 1:
                drawLineCanvas(
                    obj.p[0], obj.p[1],
                    obj.p[2], obj.p[3]
                );
                break;

            case 2:
                drawRectangleCanvas(
                    obj.p[0], obj.p[1],
                    obj.p[2], obj.p[3]
                );
                break;

            case 3:
                drawTriangleCanvas(
                    obj.p[0], obj.p[1],
                    obj.p[2], obj.p[3],
                    obj.p[4], obj.p[5]
                );
                break;

            case 4:
                drawCircleCanvas(
                    obj.p[0], obj.p[1],
                    obj.p[2]
                );
                break;
        }
    }
}

void addLine()
{
    Object obj;
    obj.type = 1;

    printf("Enter x1 y1 x2 y2: ");
    scanf("%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3]);

    objects[objectCount++] = obj;
    redrawAll();
}

void addRectangle()
{
    Object obj;
    obj.type = 2;

    printf("Enter x y width height: ");
    scanf("%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3]);

    objects[objectCount++] = obj;
    redrawAll();
}

void addTriangle()
{
    Object obj;
    obj.type = 3;

    printf("Enter x1 y1 x2 y2 x3 y3: ");
    scanf("%d%d%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3],
          &obj.p[4], &obj.p[5]);

    objects[objectCount++] = obj;
    redrawAll();
}

void addCircle()
{
    Object obj;
    obj.type = 4;

    printf("Enter centerX centerY radius: ");
    scanf("%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2]);

    objects[objectCount++] = obj;
    redrawAll();
}

void listObjects()
{
    printf("\nObjects:\n");

    for(int i = 0; i < objectCount; i++)
    {
        printf("%d. ", i);

        switch(objects[i].type)
        {
            case 1:
                printf("Line\n");
                break;

            case 2:
                printf("Rectangle\n");
                break;

            case 3:
                printf("Triangle\n");
                break;

            case 4:
                printf("Circle\n");
                break;
        }
    }
}

void deleteObject()
{
    int index;

    if(objectCount == 0)
    {
        printf("No objects available.\n");
        return;
    }

    listObjects();

    printf("Enter object number to delete: ");
    scanf("%d", &index);

    if(index < 0 || index >= objectCount)
    {
        printf("Invalid index.\n");
        return;
    }

    for(int i = index; i < objectCount - 1; i++)
    {
        objects[i] = objects[i + 1];
    }

    objectCount--;

    redrawAll();

    printf("Object deleted.\n");
}

void modifyObject()
{
    int index;

    if(objectCount == 0)
    {
        printf("No objects available.\n");
        return;
    }

    listObjects();

    printf("Enter object number to modify: ");
    scanf("%d", &index);

    if(index < 0 || index >= objectCount)
    {
        printf("Invalid index.\n");
        return;
    }

    Object *obj = &objects[index];

    switch(obj->type)
    {
        case 1:
            printf("Enter new x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &obj->p[0], &obj->p[1],
                  &obj->p[2], &obj->p[3]);
            break;

        case 2:
            printf("Enter new x y width height: ");
            scanf("%d%d%d%d",
                  &obj->p[0], &obj->p[1],
                  &obj->p[2], &obj->p[3]);
            break;

        case 3:
            printf("Enter new x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &obj->p[0], &obj->p[1],
                  &obj->p[2], &obj->p[3],
                  &obj->p[4], &obj->p[5]);
            break;

        case 4:
            printf("Enter new centerX centerY radius: ");
            scanf("%d%d%d",
                  &obj->p[0], &obj->p[1],
                  &obj->p[2]);
            break;
    }

    redrawAll();

    printf("Object modified.\n");
}

int main()
{
    int choice;

    clearCanvas();

    while(1)
    {
        printf("\n");
        printf("=====================================\n");
        printf("         2D GRAPHICS EDITOR\n");
        printf("=====================================\n");
        printf("1. Add Line\n");
        printf("2. Add Rectangle\n");
        printf("3. Add Triangle\n");
        printf("4. Add Circle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Display Picture\n");
        printf("8. Clear Picture\n");
        printf("9. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addLine();
                break;

            case 2:
                addRectangle();
                break;

            case 3:
                addTriangle();
                break;

            case 4:
                addCircle();
                break;

            case 5:
                deleteObject();
                break;

            case 6:
                modifyObject();
                break;

            case 7:
                displayCanvas();
                break;

            case 8:
                objectCount = 0;
                clearCanvas();
                printf("Canvas cleared.\n");
                break;

            case 9:
                printf("Exiting Graphics Editor...\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}