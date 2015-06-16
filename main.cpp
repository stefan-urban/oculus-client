
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <OVR_CAPI.h>
#include <GLFW/glfw3.h>

#include "EdvsImage.hpp"
#include "TcpClient.hpp"



// Create io service
boost::asio::io_service io_service;


void edvs_client_app(int argc, char* argv[])
{
    if (argc != 3)
    {
        return;
    }

    // Event handler (will be images)
    EdvsImage images[7];

    // Setup TCP connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    TcpClient c(io_service, endpoint_iterator, images);

    // Start client
    io_service.run();

    c.close();
}


static void oculus_rift_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void oculus_rift_app()
{
    ovrBool init_status = ovr_Initialize(nullptr);

    if (init_status)
    {
        ovrHmd hmd = ovrHmd_Create(0);

        if (hmd)
        {
            // Get more details about the HMD.
            ovrSizei resolution = hmd->Resolution;

            // Init GLFW
            int glfw_init_status = glfwInit();

            if (glfw_init_status == GL_TRUE)
            {
                // Get Oculus Rift window
                int count;
                GLFWmonitor** monitors = glfwGetMonitors(&count);

                // TODO: select right monitor
                const char* name;
                for (int i = 0; i < count; i++)
                {
                    name = glfwGetMonitorName(monitors[i]);
                }

                // Create window
                GLFWwindow* window = glfwCreateWindow(1920, 1080, "test", monitors[2], nullptr);

                if (window)
                {
                    glfwSetKeyCallback(window, oculus_rift_key_callback);

                    glfwMakeContextCurrent(window);
                    glfwSwapInterval(1);

                    ovrHmd_AttachToWindow(hmd, window, nullptr, nullptr);

                    // Stop application if window is closed
                    while (!glfwWindowShouldClose(window))
                    {
                        // Keep running
                        float ratio;
                        int width, height;
                        glfwGetFramebufferSize(window, &width, &height);
                        ratio = width / (float) height;
                        glViewport(0, 0, width, height);
                        glClear(GL_COLOR_BUFFER_BIT);
                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
                        glBegin(GL_TRIANGLES);
                        glColor3f(1.f, 0.f, 0.f);
                        glVertex3f(-0.6f, -0.4f, 0.f);
                        glColor3f(0.f, 1.f, 0.f);
                        glVertex3f(0.6f, -0.4f, 0.f);
                        glColor3f(0.f, 0.f, 1.f);
                        glVertex3f(0.f, 0.6f, 0.f);
                        glEnd();

                        // Do buffer swap
                        glfwSwapBuffers(window);
                        glfwPollEvents();
                    }

                    // Destroy window
                    glfwDestroyWindow(window);
                }
                glfwTerminate();
            }

            // Destroy oculus
            ovrHmd_Destroy(hmd);
        }

        ovr_Shutdown();
    }
}

int main(int argc, char* argv[])
{
    std::cout << "oculus-client v1" << std::endl;


    boost::thread eca(edvs_client_app, argc, argv);
    boost::thread ora(oculus_rift_app);


    eca.join();
    ora.join();

    return 0;
}
