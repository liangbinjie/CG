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


/* =========================================================
   A
   ========================================================= */

BezierCurve A[] = {

    // Left stroke
    {
        {50, 234},
        {63, 215},
        {77, 183},
        {90, 167}
    },

    // Right stroke
    {
        {90, 167},
        {94, 185},
        {99, 211},
        {104, 229}
    },

    // Bar
    {
        {65, 214},
        {82, 213},
        {94, 212},
        {101, 212}
    }
};


/* =========================================================
   n
   ========================================================= */

BezierCurve N[] = {

    // Left stroke
    {
        {115, 233},
        {116, 222},
        {116, 211},
        {117, 204}
    },

    // Arch
    {
        {117, 218},
        {130, 196},
        {139, 201},
        {144, 227}
    }
};


/* =========================================================
   d
   ========================================================= */

BezierCurve D[] = {

    // Bowl
    {
        {175, 206},
        {150, 210},
        {150, 231},
        {174, 236}
    },

    // Bowl continuation
    {
        {174, 236},
        {185, 240},
        {187, 218},
        {177, 216}
    },

    // Stem going up
    {
        {177, 216},
        {177, 195},
        {177, 178},
        {177, 174}
    },

    // Stem going down
    {
        {177, 174},
        {178, 195},
        {178, 220},
        {178, 237}
    }
};


/* =========================================================
   r
   ========================================================= */

BezierCurve R[] = {

    // Left stroke
    {
        {197, 227},
        {193, 222},
        {194, 210},
        {191, 209}
    },

    // Curve
    {
        {195, 214},
        {201, 198},
        {207, 199},
        {212, 204}
    }
};


/* =========================================================
   e
   ========================================================= */


BezierCurve E[] = {

    // Top stroke
    {
        {224, 220},
        {218, 205},
        {238, 198},
        {246, 207}
    },

    // Bowl and exit stroke
    {
        {224, 220},
        {220, 245},
        {250, 235},
        {270, 220}
    }
};


/* =========================================================
   s
   ========================================================= */

BezierCurve S[] = {

    // Upper curve
    {
        {250, 213},
        {257, 204},
        {270, 202},
        {280, 207}
    },

    // Turn toward the center
    {
        {280, 207},
        {287, 212},
        {283, 217},
        {273, 219}
    },

    // Lower curve
    {
        {273, 219},
        {262, 221},
        {256, 224},
        {259, 228}
    },

    // Final exit
    {
        {259, 228},
        {266, 232},
        {279, 228},
        {286, 220}
    }
};

/* =========================================================
   Accent
   ========================================================= */

BezierCurve Accent[] = {

    {
        {243, 186},
        {249, 182},
        {256, 178},
        {262, 174}
    }
};


/* =========================================================
   L
   ========================================================= */

BezierCurve L[] = {

    // Vertical stroke
    {
        {362, 174},
        {364, 190},
        {364, 207},
        {357, 220}
    },

    // Bottom stroke
    {
        {357, 220},
        {365, 225},
        {378, 229},
        {391, 231}
    }
};


/* =========================================================
   i
   ========================================================= */

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


/* =========================================================
   a
   ========================================================= */

BezierCurve A2[] = {

    // Left side and top of bowl
    {
        {416, 218},
        {417, 207},
        {425, 201},
        {435, 201}
    },

    // Right side of bowl
    {
        {435, 201},
        {445, 201},
        {449, 209},
        {447, 217}
    },

    // Bottom / closing part of bowl
    {
        {447, 217},
        {443, 225},
        {427, 227},
        {419, 220}
    },

    // Right stroke
    {
        {447, 207},
        {449, 215},
        {450, 225},
        {453, 230}
    }
};


/* =========================================================
   n
   ========================================================= */

BezierCurve N2[] = {

    // Left stroke
    {
        {466, 224},
        {467, 214},
        {468, 205},
        {469, 201}
    },

    // Arch
    {
        {469, 201},
        {477, 194},
        {489, 198},
        {495, 213}
    },

    // Ending
    {
        {495, 213},
        {498, 220},
        {500, 224},
        {500, 228}
    }
};


/* =========================================================
   g
   ========================================================= */

BezierCurve G[] = {

    // Top of bowl
    {
        {505, 217},
        {508, 207},
        {517, 201},
        {527, 202}
    },

    // Right side of bowl
    {
        {527, 202},
        {538, 203},
        {543, 212},
        {541, 220}
    },

    // Bottom of bowl
    {
        {541, 220},
        {538, 229},
        {527, 233},
        {518, 229}
    },

    // Descending stroke
    {
        {518, 229},
        {527, 234},
        {535, 238},
        {534, 248}
    },

    // Loop/tail
    {
        {534, 248},
        {533, 258},
        {520, 264},
        {512, 257}
    },

    // Final exit
    {
        {512, 257},
        {507, 253},
        {514, 249},
        {522, 249}
    }
};


/* =========================================================
   Animation variables
   ========================================================= */

int currentCurve = 0;
float currentT = 0.0f;


/* =========================================================
   Calculate Bézier point
   ========================================================= */

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


/* =========================================================
   Draw one Bézier curve
   ========================================================= */

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


/* =========================================================
   Draw signature
   ========================================================= */

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
       curves 11 - 12
       ------------------------- */

    for (int i = 0; i < 2; i++)
    {
        int c = i + 11;

        if (currentCurve > c)
            drawCurve(E[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(E[i], currentT);
    }


    /* -------------------------
       s
       curves 13 - 16
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        int c = i + 13;

        if (currentCurve > c)
            drawCurve(S[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(S[i], currentT);
    }


    /* -------------------------
       Accent
       curve 17
       ------------------------- */

    if (currentCurve > 17)
        drawCurve(Accent[0], 1.0f);

    else if (currentCurve == 17)
        drawCurve(Accent[0], currentT);


    /* -------------------------
       L
       curves 18 - 19
       ------------------------- */

    for (int i = 0; i < 2; i++)
    {
        int c = i + 18;

        if (currentCurve > c)
            drawCurve(L[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(L[i], currentT);
    }


    /* -------------------------
       i
       curves 20 - 21
       ------------------------- */

    if (currentCurve > 20)
        drawCurve(I[0], 1.0f);

    else if (currentCurve == 21)
        drawCurve(I[0], currentT);

    if (currentCurve > 20)
        drawCurve(IDot[0], 1.0f);

    else if (currentCurve == 20)
        drawCurve(IDot[0], currentT);


    /* -------------------------
       a
       curves 22-25
       ------------------------- */

    for (int i = 0; i < 4; i++)
    {
        int c = i + 22;

        if (currentCurve > c)
            drawCurve(A2[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(A2[i], currentT);
    }


    /* -------------------------
       n
       curves 26 - 28
       ------------------------- */

    for (int i = 0; i < 3; i++)
    {
        int c = i + 26;

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
        int c = i + 29;

        if (currentCurve > c)
            drawCurve(G[i], 1.0f);

        else if (currentCurve == c)
            drawCurve(G[i], currentT);
    }
}


/* =========================================================
   Display
   ========================================================= */

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSignature();

    glutSwapBuffers();
}


/* =========================================================
   Animation
   ========================================================= */

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

        glutTimerFunc(5, update, 0);
    }
}


/* =========================================================
   Initialization
   ========================================================= */

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


/* =========================================================
   Main
   ========================================================= */

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
        "Animated Signature"
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