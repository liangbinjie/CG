#include <GL/glut.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point p0;
    Point p1;
    Point p2;
    Point p3;
} BezierCurve;


// A
BezierCurve A[] = {

    // lado ziq
    {
        {50, 234},
        {63, 215},
        {77, 183},
        {90, 167}
    },

    // lado derecho
    {
        {90, 167},
        {94, 185},
        {99, 211},
        {104, 229}
    },

    // barra
    {
        {65, 214},
        {82, 213},
        {94, 212},
        {101, 212}
    }
};


// n
BezierCurve N[] = {

    // izquierda linea
    {
        {115, 233},
        {116, 222},
        {116, 211},
        {117, 204}
    },

    // Arco
    {
        {117, 218},
        {130, 196},
        {139, 201},
        {144, 227}
    }
};


// d
BezierCurve D[] = {

    {
        {175, 206},
        {150, 210},
        {150, 231},
        {174, 236}
    },

    {
        {174, 236},
        {185, 240},
        {187, 218},
        {177, 216}
    },

    {
        {177, 216},
        {177, 195},
        {177, 178},
        {177, 174}
    },

    {
        {177, 174},
        {178, 195},
        {178, 220},
        {178, 237}
    }
};


// r
BezierCurve R[] = {

    {
        {197, 227},
        {193, 222},
        {194, 210},
        {191, 209}
    },

    {
        {195, 214},
        {201, 198},
        {207, 199},
        {212, 204}
    }
};


//e
BezierCurve E[] = {

    {
        {223, 208},
        {223, 214},
        {223, 210},
        {246, 204}
    },

    {
        {246, 204},
        {246, 190},
        {226, 190},
        {222, 220}
    },

    {
        {222, 220},
        {213, 207},
        {228, 234},
        {248, 219}
    }
};


//   s

BezierCurve S[] = {

    {
        {288,196},
        {275,190},
        {263,190},
        {255,200}
    },

    {
        {255,200},
        {270,197},
        {286,202},
        {284,211}
    },

    {
        {284,211},
        {275,220},
        {263,224},
        {256,219}
    },

    {
        {256,219},
        {264,216},
        {277,215},
        {286,220}
    }
};

// tilde
BezierCurve Accent[] = {

    {
        {243, 186},
        {249, 182},
        {256, 178},
        {262, 174}
    }
};


// L
BezierCurve L[] = {

    {
        {362, 174},
        {364, 190},
        {364, 207},
        {357, 220}
    },

    {
        {357, 220},
        {365, 225},
        {378, 229},
        {391, 231}
    }
};


// i

BezierCurve I[] = {

    // Stem
    {
        {402, 207},
        {402, 216},
        {402, 226},
        {402, 233}
    }
};

BezierCurve IDot[] = {

    {
        {402, 191},
        {402, 190},
        {403, 190},
        {403, 191}
    }
};

 
// a
BezierCurve A2[] = {

    {
        {416, 218},
        {417, 207},
        {425, 201},
        {435, 201}
    },

    {
        {435, 201},
        {445, 201},
        {449, 209},
        {447, 217}
    },

    {
        {447, 217},
        {443, 225},
        {427, 227},
        {419, 220}
    },

    {
        {447, 207},
        {449, 215},
        {450, 225},
        {453, 230}
    }
};


// n

BezierCurve N2[] = {

    // 1. Left stroke: starts at (469,219) and goes down
    {
        {469, 197},
        {468, 224},
        {468, 230},
        {469, 234}
    },

    // 2. Arch: rises from the left and goes to the right
    {
        {469, 210},
        {474, 200},
        {486, 200},
        {491, 220}
    },

    // 3. Right stroke: comes down
    {
        {491, 220},
        {493, 224},
        {493, 229},
        {493, 233}
    }
};


// G
BezierCurve G[] = {

    {
        {532, 191},
        {535, 197},
        {542, 197},
        {550, 197}
    },

    {
        {550, 197},
        {537, 198},
        {520, 202},
        {519, 216}
    },

    {
        {519, 216},
        {518, 228},
        {525, 235},
        {537, 235}
    },

    {
        {537, 235},
        {550, 235},
        {560, 220},
        {565, 207}
    },

    {
        {565, 207},
        {573, 218},
        {570, 235},
        {560, 250}
    },

    {
        {560, 250},
        {545, 270},
        {520, 275},
        {492, 275}
    }
};

int currentCurve = 0;
float currentT = 0.0f;



Point bezierPoint(BezierCurve curve, float t)
{
    Point p;

    float u = 1.0f - t;

    p.x =
        u*u*u * curve.p0.x +
        3*u*u*t * curve.p1.x +
        3*u*t*t * curve.p2.x +
        t*t*t * curve.p3.x;

    p.y =
        u*u*u * curve.p0.y +
        3*u*u*t * curve.p1.y +
        3*u*t*t * curve.p2.y +
        t*t*t * curve.p3.y;

    return p;
}



void drawCurve(BezierCurve curve, float t)
{
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= 50; i++)
    {
        float u = (float)i / 50.0f;

        float localT = t * u;

        Point p = bezierPoint(curve, localT);

        glVertex2f(p.x, p.y);
    }

    glEnd();
}



void drawSignature()
{
    glColor3f(1.0f, 0.0f, 0.0f);

    glLineWidth(2.0f);


    /* -------------------------
       A
       curves 0 - 2
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        if (currentCurve > i)
            drawCurve(A[i], 1.0f);

        else if (currentCurve == i)
            drawCurve(A[i], currentT);
    }


    /* -------------------------
       n
       curves 3 - 4
       ------------------------- */

    for (int i = 0; i < 2; i++)
    {
        int c = i + 3;

        if (currentCurve > c)
            drawCurve(N[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(N[i], currentT);
    }


    /* -------------------------
       d
       curves 5 - 8
       ------------------------- */

    for (int i = 0; i < 4; i++)
    {
        int c = i + 5;

        if (currentCurve > c)
            drawCurve(D[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(D[i], currentT);
    }


    /* -------------------------
       r
       curves 9 - 10
       ------------------------- */

    for (int i = 0; i < 2; i++)
    {
        int c = i + 9;

        if (currentCurve > c)
            drawCurve(R[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(R[i], currentT);
    }


    /* -------------------------
       e
       curves 11 - 13
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        int c = i + 11;

        if (currentCurve > c)
            drawCurve(E[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(E[i], currentT);
    }


    /* -------------------------
       s
       curves 14 - 17
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        int c = i + 14;

        if (currentCurve > c)
            drawCurve(S[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(S[i], currentT);
    }


    /* -------------------------
       Accent
       curve 18
       ------------------------- */

    if (currentCurve > 18)
        drawCurve(Accent[0], 1.0f);

    else if (currentCurve == 18)
        drawCurve(Accent[0], currentT);


    /* -------------------------
       L
       curves 19 - 20
       ------------------------- */

    for (int i = 0; i < 2; i++)
    {
        int c = i + 19;

        if (currentCurve > c)
            drawCurve(L[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(L[i], currentT);
    }


    /* -------------------------
       i
       curves 21 - 22
       ------------------------- */

    if (currentCurve > 21)
        drawCurve(I[0], 1.0f);

    else if (currentCurve == 21)
        drawCurve(I[0], currentT);

    if (currentCurve > 21)
        drawCurve(IDot[0], 1.0f);

    else if (currentCurve == 21)
        drawCurve(IDot[0], currentT);


    /* -------------------------
       a
       curves 23-26
       ------------------------- */

    for (int i = 0; i < 4; i++)
    {
        int c = i + 23;

        if (currentCurve > c)
            drawCurve(A2[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(A2[i], currentT);
    }


    /* -------------------------
       n
       curves 27 - 29
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        int c = i + 27;

        if (currentCurve > c)
            drawCurve(N2[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(N2[i], currentT);
    }


    /* -------------------------
       g
       curves 30-35
       ------------------------- */

    for (int i = 0; i < 6; i++)
    {
        int c = i + 30;

        if (currentCurve > c)
            drawCurve(G[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(G[i], currentT);
    }
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSignature();

    glutSwapBuffers();
}


void update(int value)
{
    if (currentCurve < 36)
    {
        currentT += 0.02f;

        if (currentT >= 1.0f)
        {
            currentT = 0.0f;
            currentCurve++;
        }

        glutPostRedisplay();

        glutTimerFunc(1, update, 0);
    }
}


void init()
{
    glClearColor(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        0,
        600,
        400,
        0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB
    );

    glutInitWindowSize(
        600,
        400
    );

    glutCreateWindow(
        "Firma AndresLiang"
    );

    init();

    glutDisplayFunc(display);

    glutTimerFunc(
        30,
        update,
        0
    );

    glutMainLoop();

    return 0;
}