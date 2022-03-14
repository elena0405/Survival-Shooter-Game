#include <vector>
#include <iostream>
#include <time.h>
#include <cmath>
#include <math.h>
#include <stdlib.h>

#include "transform2D.h"
#include "object2D.h"
#include "lab_m1/Tema1/Tema1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}


void Tema1::Init()
{   
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->SetOrthographic(0, (float)viewSpace.x, 0, (float)viewSpace.y, 0.01f, 400);
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 350;       // logic x
    logicSpace.y = 350;       // logic y
    logicSpace.width = 700;   // logic width
    logicSpace.height = 700;  // logic height

    score = 0;

    translateX = 0;
    translateY = 0;

    speedX = 150;
    speedY = 150;

    speedPX = 300;
    speedPY = 300;

    NObstacles = 5;

    up = false;
    left = false;
    down = false;
    right = false;
    colision = false;

    // Initialize angularStep
    angularStep = 0;
    distanceProjectile = 300;

    // datele necesare crearii jucatorului
    radix = 40;
    // pozitionez jucatorul in centrul ferestrei de afisare
    center1 = glm::vec3(logicSpace.x + logicSpace.width / 2.0f, logicSpace.y + logicSpace.height / 2.0f, 0);
    center2 = glm::vec3(center1.x + 37, center1.y - 20, 0);
    center3 = glm::vec3(center1.x + 40, center1.y + 10, 0);

    // datele necesare crearii inamicului
    corner1 = glm::vec3(400, 400, 0);  // determin coltul din stanga jos pentru patratul 1 (patratul principal)
    corner2 = glm::vec3(460, 460, 0);  // determin coltul din stanga jos pentru patratul 2 (patratul secundar 1)
    corner3 = glm::vec3(460, 390, 0);  // determin coltul din stanga jos pentru patratul 3 (patratul secundar 2)
    squareSide1 = 70;                  // determin lungimea laturii patratului 1
    squareSide2 = 20;                  // determin lungimea laturii patratului 2
    squareSide3 = 20;                  // determin lungimea laturii patratului 3

    // datele necesare crearii dreptunghiurilor pentru harta
    w1 = 30;                          // latimea dreptunghiurilor
    l1 = 1100;                        // lungimea dreptungiurilor 
    corner4 = glm::vec3(0, 0, 0);     // determin coltul din stanga jos pentru dreptunghiul 1 (cel de jos)
    corner5 = glm::vec3(0, 1100, 0);  // determin coltul din stanga jos pentru dreptunghiul 2 (cel de sus)
    corner6 = glm::vec3(0, 0, 0);     // determin coltul din stanga jos pentru dreptunghiul 3 (cel din stanga) 
    corner7 = glm::vec3(1070, 0, 0);  // determin coltul din stanga jos pentru dreptunghiul 4 (cel din dreapta) 

    // datele necesare crearii obstacolelor
    w2 = 30;                           // latimea obstacolelor
    l2 = 100;                          // lungimea obstacolelor
    corner8 = glm::vec3(140, 100, 0);  // coltul din stanga jos al unui obstacol

    // datele necesare crearii healthbar-ului
    l3 = 200;
    w3 = 40;
    corner9 = glm::vec3(logicSpace.x + logicSpace.width - l3 - 3, logicSpace.y + logicSpace.height - w3 - 3, 0);

    // creez inamicul folisind 3 primitive (3 patrate)
    Mesh* square1 = object2D::CreateSquare("square1", corner1, squareSide1, glm::vec3(1, 1, 1), true);
    AddMeshToList(square1); 
    Mesh* square2 = object2D::CreateSquare("square2", corner2, squareSide2, glm::vec3(0, 0.2f, 1), true);
    AddMeshToList(square2);
    Mesh* square3 = object2D::CreateSquare("square3", corner3, squareSide3, glm::vec3(0, 0.2f, 1), true);
    AddMeshToList(square3); 

    // creez jucatorul folosind 3 primitive (3 cercuri)
    Mesh* circle1 = object2D::CreateCircle("circle1", radix, glm::vec3(1, 1, 1), center1, true);
    AddMeshToList(circle1); 
    Mesh* circle2 = object2D::CreateCircle("circle2", radix / 4, glm::vec3(0, 1, 1), center2, true);
    AddMeshToList(circle2); 
    Mesh* circle3 = object2D::CreateCircle("circle3", radix / 4, glm::vec3(0, 1, 1), center3, true);
    AddMeshToList(circle3); 
    
    // creez cei 4 pereti ai hartii
    Mesh* rectangle1 = object2D::CreateRectangle("rectangle1", corner4, l1, w1, glm::vec3(0, 0.9f, 1), true);
    AddMeshToList(rectangle1);
    Mesh* rectangle2 = object2D::CreateRectangle("rectangle2", corner5, l1, w1, glm::vec3(0, 0.9f, 1), true);
    AddMeshToList(rectangle2);
    Mesh* rectangle3 = object2D::CreateRectangle("rectangle3", corner6, w1, l1, glm::vec3(0, 0.9f, 1), true);
    AddMeshToList(rectangle3);
    Mesh* rectangle4 = object2D::CreateRectangle("rectangle4", corner7, w1, l1, glm::vec3(0, 0.9f, 1), true);
    AddMeshToList(rectangle4);

    // creez un obstacol
    Mesh* obstacle = object2D::CreateRectangle("obstacle", corner8, l2, w2, glm::vec3(0, 0, 1), true);
    AddMeshToList(obstacle);

    // creez un proiectil
    squareSide4 = 20;
    Mesh* projectil = object2D::CreateSquare("projectil", center1, squareSide4, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectil);

    // creez un inamic
    Mesh* enemy1 = object2D::CreateSquare("enemy1", corner1, squareSide1, glm::vec3(1, 0, 0.5f), true);
    AddMeshToList(enemy1);
    Mesh* enemy2 = object2D::CreateSquare("enemy2", corner2, squareSide2, glm::vec3(1, 0, 1), true);
    AddMeshToList(enemy2);
    Mesh* enemy3 = object2D::CreateSquare("enemy3", corner3, squareSide3, glm::vec3(1, 0, 1), true);
    AddMeshToList(enemy3);

    // creez cele doua dreptunghiuri pt healthBar
    Mesh* healthBar1 = object2D::CreateRectangle("healthBar1", corner9, l3, w3, glm::vec3(0, 0, 0), true);
    AddMeshToList(healthBar1);
    Mesh* healthBar2 = object2D::CreateRectangle("healthBar2", corner9, l3, w3, glm::vec3(1, 0, 0), true);
    AddMeshToList(healthBar2);

    h1 = HealthBar(corner9, l3, w3, 0, 0, 1, 1, glm::mat3(1));
    h2 = HealthBar(corner9, l3, w3, 0, 0, 1, 1, glm::mat3(1));
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}



void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.55f), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    DrawScene(visMatrix, deltaTimeSeconds);
}


void Tema1::FrameEnd()
{
}

bool Tema1::CheckColisionProjectileObstacles(Projectile projectile) {
    bool colision = false;

    for (int i = 0; i < obstacles.size(); i++) {
        colision = colCircleRectangle(projectile.corner.x + projectile.translatePX + projectile.squareSide / 2.0f,
            projectile.corner.y + projectile.translatePY + projectile.squareSide / 2.0f,
            projectile.squareSide / 2.0f,
            obstacles[i].corner.x + obstacles[i].translateX,
            obstacles[i].corner.y + obstacles[i].translateY,
            obstacles[i].height,
            obstacles[i].width);

        if (colision == true) {
            return true;
        }
    }

    return false;
}

bool Tema1::CheckColisionEnemyProjectile(Enemy enemy) {
    float dx, dy;

    for (int i = 0; i < projectiles.size(); i++) {
        dx = abs((enemy.corner1.x + enemy.translateX + enemy.squareSideBig / 2.0f)
            - (projectiles[i].corner.x + projectiles[i].translatePX + projectiles[i].squareSide / 2.0f));
        dy = abs((enemy.corner1.y + enemy.translateY + enemy.squareSideBig / 2.0f)
            - (projectiles[i].corner.y + projectiles[i].translatePY + projectiles[i].squareSide / 2.0f));

        if (sqrt(pow(dx, 2) + pow(dy, 2)) <= enemy.squareSideBig / 2.0f + projectiles[i].squareSide / 2.0f) {
            projectiles.erase(projectiles.begin() + i);
            return true;
        }
    }

    return false;
}

bool Tema1::CheckColisionEnemyPlayer(Enemy enemy) {
    float dx, dy;

    // calculez distanta pe OX dintre jucator si inamic
    dx = abs((enemy.translateX + enemy.corner1.x + enemy.squareSideBig / 2.0f)
        - (logicSpace.x + logicSpace.width / 2.0f));
    // calculez distanta dintre jucator si inamic pe OY
    dy = abs((enemy.translateY + enemy.corner1.y + enemy.squareSideBig / 2.0f)
        - (logicSpace.y + logicSpace.height / 2.0f));

    if (sqrt(pow(dx, 2) + pow(dy, 2)) > radix + enemy.squareSideBig / 2.0f) {
        return false;
    }

    return true;
}

bool Tema1::CheckColisionProjectileMapWall(Projectile projectile) {
    if ((projectile.corner.x - projectile.squareSide / 2.0f + projectile.translatePX <= corner6.x + w1) ||
        (projectile.corner.x + projectile.squareSide / 2.0f + projectile.translatePX >= corner7.x) ||
        (projectile.corner.y + projectile.squareSide + projectile.translatePY >= corner5.y) ||
        (projectile.corner.y + projectile.translatePY <= corner4.y + w1)) {
        return true;
    }

    return false;
}

bool Tema1::CheckColisionEnemyMapWall(Enemy enemy) {
    if ((enemy.corner1.x + enemy.translateX <= corner6.x + w1) ||
        (enemy.corner1.x + enemy.translateX + enemy.squareSideBig >= corner7.x) ||
        (enemy.corner1.y + enemy.translateY + enemy.squareSideBig >= corner5.y) ||
        (enemy.corner1.y + enemy.translateY <= corner4.y + w1)) {
        return true;
    }

    return false;
}

void Tema1::CreateEnemies(float deltaTimeSeconds) {
    int x = rand() % 1000 - corner1.x;
    int speed = rand() % 550 + 100;
    float angle, dirX, dirY;
    bool colisionPlayer = false;
    bool colisionProjectile = false;
    bool colisionMapWall = false;

    if (((float)(clock() - time2) / CLOCKS_PER_SEC) >= 3) {
        Enemy enemy = Enemy(corner1, corner2, corner3, squareSide1, squareSide2, speed, speed, x, x, glm::mat3(1));
        enemies.push_back(enemy);

        time2 = clock();
    }

    for (int i = 0; i < enemies.size(); i++) {
        angle = atan2((logicSpace.y + logicSpace.height / 2.0f) 
                       - (enemies[i].translateY + enemies[i].corner1.y + enemies[i].squareSideBig / 2.0f),
                      (logicSpace.x + logicSpace.width / 2.0f) 
                       - (enemies[i].translateX + enemies[i].corner1.x + enemies[i].squareSideBig / 2.0f));
        dirX = cos(angle);
        dirY = sin(angle);

        enemies[i].modelMatrix = visMatrix * transform2D::Translate(enemies[i].translateX, enemies[i].translateY);

        enemies[i].modelMatrix *= transform2D::Translate(enemies[i].squareSideBig / 2.0f + enemies[i].corner1.x,
                                                         enemies[i].squareSideBig / 2.0f + enemies[i].corner1.y);
        enemies[i].modelMatrix *= transform2D::Rotate(angle);
        enemies[i].modelMatrix *= transform2D::Translate((-1) * (enemies[i].squareSideBig / 2.0f + enemies[i].corner1.x),
                                                         (-1) * (enemies[i].squareSideBig / 2.0f + enemies[i].corner1.y));

        enemies[i].translateX += deltaTimeSeconds * enemies[i].speedX * dirX;
        enemies[i].translateY += deltaTimeSeconds * enemies[i].speedY * dirY;

        colisionPlayer = CheckColisionEnemyPlayer(enemies[i]);
        colisionProjectile = CheckColisionEnemyProjectile(enemies[i]);
        colisionMapWall = CheckColisionEnemyMapWall(enemies[i]);

        if (colisionMapWall == true) {
            enemies[i].translateX -= deltaTimeSeconds * enemies[i].speedX * dirX;
            enemies[i].translateY -= deltaTimeSeconds * enemies[i].speedY * dirY;
        }

        if (colisionProjectile == true) {
            enemies.erase(enemies.begin() + i);
            score += 1;
            cout << "Score is: " << score << endl;
        } 
        else if (colisionPlayer == true) {
            // daca am coliziune intre jucator si inamic, fac inamicul sa dispara
            enemies.erase(enemies.begin() + i);
            h2.scaleX -= 0.1;
        }
        else {
            RenderMesh2D(meshes["enemy2"], shaders["VertexColor"], enemies[i].modelMatrix);
            RenderMesh2D(meshes["enemy3"], shaders["VertexColor"], enemies[i].modelMatrix);
            RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], enemies[i].modelMatrix);
        }
        
    }
}

void Tema1::CreateProjectiles(float deltaTimeSeconds) {
    bool colisionMapWall = false;
    bool colisionObstacles = false;

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].modelMatrixProjectile = visMatrix * transform2D::Translate(projectiles[i].translatePX, projectiles[i].translatePY);

        projectiles[i].modelMatrixProjectile *= transform2D::Translate(center1.x, center1.y);
        projectiles[i].modelMatrixProjectile *= transform2D::Rotate(projectiles[i].angularStepProjectile);
        projectiles[i].modelMatrixProjectile *= transform2D::Translate((-1) * (center1.x), (-1) * (center1.y));

        projectiles[i].translatePX += deltaTimeSeconds * speedPX * projectiles[i].directionX;
        projectiles[i].translatePY += deltaTimeSeconds * speedPY * projectiles[i].directionY;

        colisionMapWall = CheckColisionProjectileMapWall(projectiles[i]);
        colisionObstacles = CheckColisionProjectileObstacles(projectiles[i]);

        if (projectiles[i].isRunning == 1) {
            // daca proiectilul depaseste distanta maxima admisa, fara sa se intersecteze cu un inamic
            if ((sqrt(pow(projectiles[i].translatePX - translateX, 2) + pow(projectiles[i].translatePY - translateY, 2)) > distanceProjectile)) {
                // elimin proiectilul
                projectiles.erase(projectiles.begin() + i);
            }
            else if (colisionMapWall == true) {
                // daca am coliziune cu vreun perete al hartii, elimin proiectilul
                projectiles.erase(projectiles.begin() + i);
            } 
            else if (colisionObstacles == true) {
                // daca am coliziune cu vreun obstacol al hartii, elimin proiectilul
                projectiles.erase(projectiles.begin() + i);
            }
            else {
                RenderMesh2D(meshes["projectil"], shaders["VertexColor"], projectiles[i].modelMatrixProjectile);
            }
        }
        else {
            projectiles[i].isRunning = 1;
        }
    }
}

void Tema1::CreatePlayer() {
    // Implementez matricea pentru translatie
    modelMatrixPlayer = visMatrix * transform2D::Translate(translateX, translateY);

    // Implementez matricea pentru rotatie
    modelMatrixPlayer *= transform2D::Translate(center1.x, center1.y);
    modelMatrixPlayer *= transform2D::Rotate(angularStep);
    modelMatrixPlayer *= transform2D::Translate((-1) * (center1.x), (-1) * (center1.y));

    if (h2.scaleX > 0) {
        // Apelez RenderMesh2D() pentru toate cele 3 cercuri ale jucatorului
        RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrixPlayer);
        RenderMesh2D(meshes["circle3"], shaders["VertexColor"], modelMatrixPlayer);
        RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrixPlayer);
    }
    else {
        cout << "GAME OVER!" << endl;
    }
}

void Tema1::CreateMap() {
    // Apelez RenderMesh2D() pentru toate cele 4 dreptunghiuri ale peretilor hartii
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["rectangle3"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["rectangle4"], shaders["VertexColor"], visMatrix);
}

void Tema1::CreateObstacles() {
    int x = 100, y = 100;

    for (int i = 0; i < NObstacles; i++) {
        Obstacle obstacle = Obstacle(corner8, w2, l2, x, y, glm::mat3(1));
        obstacles.push_back(obstacle);
        obstacles[i].modelMatrix = visMatrix * transform2D::Translate(obstacles[i].translateX, obstacles[i].translateY);

        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], obstacles[i].modelMatrix);

        x += 150;
        y += 200;
    }
}

void Tema1::CreateHealthBar(float deltaTimeSeconds) {

    h1.modelMatrix = visMatrix * transform2D::Translate(h1.width + h1.height, 0);
    h1.modelMatrix *= transform2D::Translate(h1.translateX, h1.translateY);

    h2.modelMatrix = visMatrix * transform2D::Translate(h2.width + h2.height, 0);
    h2.modelMatrix *= transform2D::Translate(h2.translateX, h2.translateY);
    h2.modelMatrix *= transform2D::Translate(h2.corner.x + h2.width, h2.corner.y);
    h2.modelMatrix *= transform2D::Scale(h2.scaleX, 1);
    h2.modelMatrix *= transform2D::Translate(-(h2.corner.x + h2.width), -(h2.corner.y));

    if (h2.scaleX > 0) {
        RenderMesh2D(meshes["healthBar2"], shaders["VertexColor"], h2.modelMatrix);
    }
    RenderMesh2D(meshes["healthBar1"], shaders["VertexColor"], h1.modelMatrix);
}

float Tema1::clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

bool Tema1::colCircleRectangle(float posCircleX, float posCircleY, float radix, float posRectangleX, float posRectangleY, float sizeX, float sizeY) {
    glm::vec2 center(posCircleX, posCircleY);
    glm::vec2 aabb_half_extents(sizeX / 2.0f, sizeY / 2.0f);
    glm::vec2 aabb_center(posRectangleX + aabb_half_extents.x, posRectangleY + aabb_half_extents.y);
    glm::vec2 difference = center - aabb_center;

    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    return glm::length(difference) < radix;
}

void Tema1::CheckColisionObstaclePlayer() {
    bool colision = false;
    float circlePosX = logicSpace.x + logicSpace.width / 2.0f;
    float circlePosY = logicSpace.y + logicSpace.height / 2.0f;
    float obstaclePosX, obstaclePosY;

    for (int i = 0; i < obstacles.size(); i++) {
        obstaclePosX = obstacles[i].corner.x + obstacles[i].translateX;
        obstaclePosY = obstacles[i].corner.y + obstacles[i].translateY;

        colision = colCircleRectangle(circlePosX, circlePosY, radix, obstaclePosX, obstaclePosY, obstacles[i].height, obstacles[i].width);
        if (colision == true) {
            if (obstaclePosX >= circlePosX) {
                // daca am coliziune la dreapta
                right = true;
            }
            if (obstaclePosX <= circlePosX) {
                // daca am coliziune la stanga
                left = true;
            }
            if (obstaclePosY >= circlePosY) {
                // daca am coliziune sus
                up = true;
            }
            if (obstaclePosY <= circlePosY) {
                // daca am coliziune jos
                down = true;
            }
        }
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    left = false;
    right = false;
    up = false;
    down = false;

    CreatePlayer();

    CreateHealthBar(deltaTimeSeconds);

    CreateMap();

    CreateProjectiles(deltaTimeSeconds);

    CreateEnemies(deltaTimeSeconds);

    CreateObstacles();

    CheckColisionObstaclePlayer();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{   
    if (window->KeyHold(GLFW_KEY_A)) {
        // verific daca am coliziune cu peretele din stanga al hartii sau cu obstacolele
        if ((translateX + center1.x - radix > corner6.x + w1) && (left == false)) {
            // daca nu, ma deplasez in continuare
            translateX = translateX - speedX * deltaTime;
            logicSpace.x -= speedX * deltaTime;
            h1.translateX -= speedX * deltaTime;
            h2.translateX -= speedX * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // verific daca am coliziune cu peretele din dreapta al hartii sau cu obstacolele
        if ((center1.x + radix + translateX < corner7.x) && (right == false)) {
            // daca nu, ma deplasez in continuare
            translateX = translateX + speedX * deltaTime;
            logicSpace.x += speedX * deltaTime;
            h1.translateX += speedX * deltaTime;
            h2.translateX += speedX * deltaTime;
        }
    }
    
    if (window->KeyHold(GLFW_KEY_W)) {
        // verific daca am coliziune cu peretele de sus al hartii sau cu obstacolele
        if ((translateY + center1.y + radix < corner5.y) && (up == false)) {
            // daca nu, ma deplasez in continuare
            translateY = translateY + speedY * deltaTime;
            logicSpace.y += speedY * deltaTime;
            h1.translateY += speedY * deltaTime;
            h2.translateY += speedY * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // verific daca am coliziune cu peretele de jos al hartii sau cu obstacolele
        if ((center1.y - radix + translateY > corner4.y + w1) && (down == false)) {
            // daca nu, ma deplasez in continuare
            translateY = translateY - speedY * deltaTime;
            logicSpace.y -= speedY * deltaTime;
            h1.translateY -= speedY * deltaTime;
            h2.translateY -= speedY * deltaTime;
        }
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{   
    glm::ivec2 resolution = window->GetResolution();
    glm::vec3 logicSpacePos = glm::vec3(center1.x + translateX, center1.y + translateY, 1);
    glm::vec3 viewSpacePos = visMatrix * logicSpacePos;
    double dx = (double)(mouseX - viewSpacePos.x);
    double dy = (double)(resolution.y - mouseY - viewSpacePos.y);
 
    angularStep = atan2(dy, dx);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && ((float)(clock() - time1) / CLOCKS_PER_SEC) >= 0.5) {
        // daca apas pe click stanga la mouse, creez un proiectil nou
        Projectile projectile = Projectile(0, cos(angularStep), sin(angularStep), translateX, translateY, distanceProjectile, angularStep, center1, glm::mat3(1), squareSide4);
        // adaug proiectilul in vectorul de proiectile
        projectiles.push_back(projectile);
        time1 = clock();
    }
 }


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
