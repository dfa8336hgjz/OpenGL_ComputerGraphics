#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>

#include "windowG7.h"
#include "shaders.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "picker.h"
#include "light.h"
#include "primitives/Sun.h"
#include "primitives/skyBox.h"

#include <SFML\Audio.hpp>
#define WIDTH 1920
#define HEIGHT 1080

bool musicStart = false;
bool hoohComing = false;

int main()
{
    sf::SoundBuffer sb1, sb2;
    sb1.loadFromFile("music/oracion.ogg");
    sf::Sound sound1, sound2;
    sound1.setBuffer(sb1);

    std::vector<std::string> notePath = {
        "music/c.ogg",
        "music/d.ogg",
        "music/e.ogg",
        "music/f.ogg",
        "music/g.ogg",
        "music/a.ogg",
        "music/b.ogg"
    };

    std::vector<float> surface_vertices = {
         -1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
         1.0f,  0.0f, -1.0f,  10.0f,  0.0f,
         1.0f,  0.0f,  1.0f,  10.0f, 10.0f,
        -1.0f,  0.0f,  1.0f,  0.0f,  10.0f,
    };

    std::vector<unsigned int> surface_indices = {
        0, 1, 2,
        2, 3, 0
    };

    if (!glfwInit())
        return -1;

    Window7 window(WIDTH, HEIGHT, "Group 7", NULL, NULL);

    if (!window.windowCreated()) {
        std::cout << "error";
        return -1;
    }
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    window.setCallback();
    glEnable(GL_DEPTH_TEST);

    Picker7 pick_list(WIDTH, HEIGHT);

    Shader7 surface_sd("shaders/onlyTexture.shader");
    Shader7 textureModel_sd("shaders/model.shader");
    Shader7 colorModel_sd("shaders/onlyColor.shader");

    Mesh7 surface;
    surface.createMesh(GL_STATIC_DRAW, surface_vertices, 20, surface_indices, 6);
    Texture7 surface_tx("images/grass.jpg", GL_RGB);

    Model7 building("models/toanha.fbx");
    Model7 recorder("models/mayphatnhac.fbx");
    Model7 bell("models/chuong1.fbx");
    Model7 door("models/door.fbx");
    Model7 disk("models/disk.fbx");
    Model7 piano("models/piano.fbx");
    Model7 snorlax1("models/snorlax1.dae", 1);
    Model7 snorlax2("models/snorlax2.dae", 1);
    Model7 spoink("models/spoink.dae");
    Model7 dugtrio("models/dugtrio.dae");
    Model7 hooh("models/hooh.dae");
    Model7 tree("models/tree.fbx");
    Model7 bulb1("models/light.fbx");
    Model7 mountain("models/mountain.fbx");
    Model7 chair("models/chair.fbx");
    Sun sun("models/sun.fbx");

    Light7 mainLight(&textureModel_sd);
    SkyBox skybox;

    float increment = 2.0f;
    float openAngle = 0.0f;
    float rotateDiskAngle = 0.0f;
    float bulbOnColor = 1.0f;
    float hoohposZ = 2500.0f;
    float dugtrioposY = -7.0f;

    pick_list.addObject(&disk);
    pick_list.addObject(&door);
    pick_list.addObject(&bulb1);
    pick_list.addObject(&piano);

    while (!window.canClose())
    {
        //render
        window.keyHandle();
        pick_list.renderOffScene();
        pick_list.getIdAtCursor();
        pick_list.handleMouseClick(window.keys);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = window.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(window.camZoom), (float)window.getWidth() / window.getHeight(), 0.1f, 2000.0f);
        sun.getViewProjection(view, projection);

        surface_sd.bind();
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(300.0f, 300.0f, 300.0f));
            surface_sd.SetUniformMat4f("model", model);
            surface_sd.SetUniformMat4f("projection", projection);
            surface_sd.SetUniformMat4f("view", view);
            surface_sd.SetUniform1f("brightness", skybox.skyColor);
            surface_tx.bind(0);
            surface.render();
            surface_tx.unbind();

            if (pick_list.bulb1On) bulbOnColor = 1.0f;
            else bulbOnColor = 0.4f;
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 25.0f, 50.0f));
            model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            surface_sd.SetUniformMat4f("model", model);
            surface_sd.SetUniformMat4f("projection", projection);
            surface_sd.SetUniformMat4f("view", view);
            surface_sd.SetUniform1f("brightness", bulbOnColor);
            bulb1.getMVP(model, projection, view);
            bulb1.render();

        surface_sd.unbind();
        

        textureModel_sd.bind();
            mainLight.setCam(&textureModel_sd, window.getViewPos());
            mainLight.useSunLight(&textureModel_sd, sun.getPos());

            mainLight.setMaterial(&textureModel_sd, 128.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 73.0f, 80.0f));
            model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            bell.render();

            mainLight.setMaterial(&textureModel_sd, 2.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 210.0f));
            model = glm::scale(model, glm::vec3(300.0f, 100.0f, 100.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            mountain.render();

     

            if (pick_list.bulb1On) mainLight.useBulb1(&textureModel_sd);
            else mainLight.noBulb1(&textureModel_sd);

            mainLight.setMaterial(&textureModel_sd, 32.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 6.0f, 85.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            recorder.render();

            mainLight.setMaterial(&textureModel_sd, 64.0f);
            for (int i = 0; i < 5; ++i){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(14.0f, 0.1f, 35.0f + i * 6.0f));
                model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                textureModel_sd.SetUniformMat4f("model", model);
                textureModel_sd.SetUniformMat4f("projection", projection);
                textureModel_sd.SetUniformMat4f("view", view);
                chair.render();
            }

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f, 0.1f, 58.0f));
            model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            chair.render();

            mainLight.setMaterial(&textureModel_sd, 128.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.65f, 5.85f, 83.0f));
            model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
            if (pick_list.sunRise && sound1.getStatus() != sf::SoundSource::Stopped) {
                model = glm::rotate(model, glm::radians(rotateDiskAngle), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateDiskAngle += increment;
            }
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            disk.getMVP(model, projection, view);
            disk.render();

            mainLight.setMaterial(&textureModel_sd, 2.0f);
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -500.0f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            building.render();


            mainLight.setMaterial(&textureModel_sd, 128.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f, 1.0f, 67.0f));
            model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            piano.getMVP(model, projection, view);
            piano.render();
           
            mainLight.setMaterial(&textureModel_sd, 32.0f);
            mainLight.noBulb1(&textureModel_sd);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.0f, 7.0f, 27.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            model = glm::rotate(model, glm::radians(openAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            openAngle += increment;

            if (pick_list.doorOpen) {
                increment = 2.0f;
                if (openAngle > 130.0f) openAngle = 130.0f;
            }
            else {
                increment = -2.0f;
                if (openAngle < 0.0f) openAngle = 0.0f;
            }
            textureModel_sd.SetUniformMat4f("model", model);
            textureModel_sd.SetUniformMat4f("projection", projection);
            textureModel_sd.SetUniformMat4f("view", view);
            door.getMVP(model, projection, view);
            door.render();


            mainLight.setMaterial(&textureModel_sd, 2.0f);
            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 3; ++j) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(60.0f + j * 30.0f, 0.0f, (-i) * 40.0f + 80.0f));
                    model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    textureModel_sd.SetUniformMat4f("model", model);
                    textureModel_sd.SetUniformMat4f("projection", projection);
                    textureModel_sd.SetUniformMat4f("view", view);
                    tree.render();

                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(-60.0f - j * 30.0f, 0.0f, (-i) * 40.0f + 80.0f));
                    model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    textureModel_sd.SetUniformMat4f("model", model);
                    tree.render();
                }
            }

            mainLight.setMaterial(&textureModel_sd, 4.0f);
            if (!pick_list.sunRise) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-30.0f, 9.0f, -30.0f));
                model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                textureModel_sd.SetUniformMat4f("model", model);
                textureModel_sd.SetUniformMat4f("projection", projection);
                textureModel_sd.SetUniformMat4f("view", view);
                snorlax1.render();

                
            }
            else {

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -30.0f));
                model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                textureModel_sd.SetUniformMat4f("model", model);
                textureModel_sd.SetUniformMat4f("projection", projection);
                textureModel_sd.SetUniformMat4f("view", view);
                snorlax2.render();


                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 2; j++) {
                        model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3(40.0f + 10.0f * i, glm::sin(rotateDiskAngle * 0.06f + i * 20.0f) + 0.5f, -50.0f - 10.0f * j));
                        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
                        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        textureModel_sd.SetUniformMat4f("model", model);
                        textureModel_sd.SetUniformMat4f("projection", projection);
                        textureModel_sd.SetUniformMat4f("view", view);
                        spoink.render();
                    }
                }

                if (hoohposZ < 240.0f && !hoohComing) {
                    sb2.loadFromFile("music/hooh.ogg");
                    sound2.setBuffer(sb2);
                    sound2.setVolume(50);
                    sound2.play();
                    hoohComing = true;
                }
               
                if(hoohposZ > -200000.0f){ 
                    hoohposZ -= increment * 0.2f;
                    mainLight.setMaterial(&textureModel_sd, 2.0f);
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(0.0f, 160.0f, hoohposZ));
                    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    textureModel_sd.SetUniformMat4f("model", model);
                    textureModel_sd.SetUniformMat4f("projection", projection);
                    textureModel_sd.SetUniformMat4f("view", view);
                    hooh.render();
                }

                mainLight.setMaterial(&textureModel_sd, 32.0f);
                if (sun.riseY > -400.0f) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(-10.0f, dugtrioposY, -80.0f));
                    model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    textureModel_sd.SetUniformMat4f("model", model);
                    textureModel_sd.SetUniformMat4f("projection", projection);
                    textureModel_sd.SetUniformMat4f("view", view);
                    dugtrio.render();

                    if (dugtrioposY < 0.0f) {
                        dugtrioposY += increment * 0.05f;
                    }
                }

                sun.rise();
                sun.render();
                skybox.sunRise();

                if(!musicStart){
                    sound1.play();
                    musicStart = true;
                }
            }

            if (pick_list.piano) {
                srand(time(NULL));
                int randNum = rand() % 7;
                
                sb2.loadFromFile(notePath.at(randNum));
                sound2.setBuffer(sb2);
                sound2.play();
                pick_list.piano = false;
            }
        textureModel_sd.unbind();
        skybox.render(view, projection);

        window.swapBuffer();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}