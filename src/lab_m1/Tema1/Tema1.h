#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class Projectile {
    public:
        int isRunning;
        float squareSide;
        float directionX;
        float directionY;
        float translatePX;
        float translatePY;
        float distanceProjectile;
        float angularStepProjectile;
        glm::vec3 corner;
        glm::mat3 modelMatrixProjectile;

        Projectile() {}

        Projectile(int isRunning, float directionX, float directionY, float translatePX, float translatePY, 
            float distanceProjectile, float angularStepProjectile, glm::vec3 corner, glm::mat3 modelMatrixProjectile, float squareSide) {
            this->isRunning = isRunning;
            this->directionX = directionX;
            this->directionY = directionY;
            this->translatePX = translatePX;
            this->translatePY = translatePY;
            this->distanceProjectile = distanceProjectile;
            this->angularStepProjectile = angularStepProjectile;
            this->corner = corner;
            this->squareSide = squareSide;
            this->modelMatrixProjectile = modelMatrixProjectile;
        }

        ~Projectile() {}
    };
    
    class Enemy {
    public:
        glm::vec3 corner1, corner2, corner3;
        float squareSideBig, squareSideSmall;
        float translateX, translateY;
        float speedX, speedY;
        glm::mat3 modelMatrix;

        Enemy() {}

        Enemy(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, float squareSideBig, float squareSideSmall, float speedX,
            float speedY, float translateX, float translateY, glm::mat3 modelMatrix) {
            this->speedX = speedX;
            this->speedY = speedY;
            this->corner1 = corner1;
            this->corner2 = corner2;
            this->corner3 = corner3;
            this->translateX = translateX;
            this->translateY = translateY;
            this->modelMatrix = modelMatrix;
            this->squareSideBig = squareSideBig;
            this->squareSideSmall = squareSideSmall;
        }

        ~Enemy() {}
    };

    class Obstacle {
    public:
        glm::vec3 corner;
        float width, height;
        float translateX, translateY;
        glm::mat3 modelMatrix;

        Obstacle() {}

        Obstacle(glm::vec3 corner, float width, float height, float translateX, float translateY, glm::mat3 modelMatrix) {
            this->width = width;
            this->height = height;
            this->corner = corner;
            this->translateX = translateX;
            this->translateY = translateY;
            this->modelMatrix = modelMatrix;
        }

        ~Obstacle() {}
    };

    class HealthBar {
    public:
        glm::vec3 corner;
        float height, width;
        float translateX, translateY;
        float scaleX, sclaeY;
        glm::mat3 modelMatrix;

        HealthBar() {}

        HealthBar(glm::vec3 corner, float width, float height, float tx, float ty, float sx, float sy, glm::mat3 modelMatrix) {
            this->scaleX = sx;
            this->sclaeY = sy;
            this->width = width;
            this->corner = corner;
            this->height = height;
            this->translateX = tx;
            this->translateY = ty;
            this->modelMatrix = modelMatrix;
        }

        ~HealthBar() {}
    };
    
    class Tema1 : public gfxc::SimpleScene
    {
     public:
         struct ViewportSpace
         {
             ViewportSpace() : x(0), y(0), width(1), height(1) {}
             ViewportSpace(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };

         struct LogicSpace
         {
             LogicSpace() : x(0), y(0), width(1), height(1) {}
             LogicSpace(float x, float y, float width, float height)
                 : x(x), y(y), width(width), height(height) {}
             float x;
             float y;
             float width;
             float height;
         };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        bool colCircleRectangle(float posCircleX, float posCircleY, float radix, float posRectangleX, float posRectangleY, float sizeX, float sizeY);
        float clamp(float value, float min, float max);

        void CreateMap();
        void CreatePlayer();
        void CreateObstacles();
        void CheckColisionObstaclePlayer();
        bool CheckColisionEnemyPlayer(Enemy enemy);
        bool CheckColisionProjectileMapWall(Projectile projectile);
        bool CheckColisionProjectileObstacles(Projectile projectile);
        bool CheckColisionEnemyProjectile(Enemy enemy);
        bool CheckColisionEnemyMapWall(Enemy enemy);
        void CreateEnemies(float deltaTimeSeconds);
        void CreateHealthBar(float deltaTimeSeconds);
        void CreateProjectiles(float deltaTimeSeconds);

     protected:
        clock_t time1, time2;
        int NObstacles;
        int score;
        HealthBar h1, h2;
        int isCharged, isProjectileRunning;
        float translateX, translateY;
        float angularStep;
        float distanceProjectile;
        float squareSide1, squareSide2, squareSide3;
        float squareSide4;
        float speedX, speedY, speedPX, speedPY;
        float w1, l1, l2, w2, l3, w3, radix;
        bool up, down, left, right, colision;
        glm::vec3 center1, center2, center3;
        std::vector<Projectile> projectiles;
        std::vector<Enemy> enemies;
        std::vector<Obstacle> obstacles;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrixPlayer, modelMatrixProjectile, visMatrix;
        glm::vec3 corner1, corner2, corner3, corner4, corner5, corner6, corner7, corner8, corner9;
        
    };
}   // namespace m1
