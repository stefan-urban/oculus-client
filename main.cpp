
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <OVR_CAPI.h>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "EdvsImage.hpp"
#include "TcpClient.hpp"


#define DEBUG 1

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


static void oculus_rift_key_callback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
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
            // Check if we are in direct mode
            bool directHmdMode = (0 == (ovrHmdCap_ExtendDesktop & hmd->HmdCaps));

            // Get more details about the HMD.
            ovrSizei resolution = hmd->Resolution;

            // Init GLFW
            int glfw_init_status = glfwInit();

            if (glfw_init_status == GL_TRUE)
            {
                // Get Oculus Rift window
                int count;
                GLFWmonitor** monitors = glfwGetMonitors(&count);

                // Select right monitor
                std::cout << "Select output monitor:" << std::endl;

                for (int i = 0; i < count; i++)
                {
                    std::cout << "[" << i << "] " << glfwGetMonitorName(monitors[i]) << std::endl;
                }

                // Get user input which monitor to use
                int monitor_id = -1;

#if DEBUG
                monitor_id = 2;
#endif

                while (monitor_id == -1)
                {
                    std::cin >> monitor_id;

                    if (monitor_id < 0 || monitor_id >= count)
                    {
                        std::cout << "Out of range!" << std::endl;
                        monitor_id = -1;
                    }
                }

                // Create window
                GLFWwindow* window = glfwCreateWindow(1920, 1080, "test", monitors[monitor_id], nullptr);

                if (window)
                {
                    glfwSetKeyCallback(window, oculus_rift_key_callback);

                    glfwMakeContextCurrent(window);
                    glfwSwapInterval(1);

                    //ovrHmd_AttachToWindow(hmd, window, nullptr, nullptr);
                    ovrHmd_ConfigureTracking(hmd, 128, 0);


                    GLuint vbo = 0;
                    glGenBuffers(1, &vbo);

                    static bool sw = false;

                    // Stop application if window is closed
                    while (!glfwWindowShouldClose(window))
                    {
                        // Keep running
                        int width, height;
                        glfwGetFramebufferSize(window, &width, &height);

                        glViewport(0, 0, width, height);

                        glClear(GL_COLOR_BUFFER_BIT);
                        glClearColor(0.0f, 0.4f, 0.74f, 0.0f);



                        glBindBuffer(GL_ARRAY_BUFFER, vbo); //Bind GL_ARRAY_BUFFER to our handle
                        glEnableVertexAttribArray(0); //?
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Information about the array, 3 points for each vertex, using the float type, don't normalize, no stepping, and an offset of 0. I don't know what the first parameter does however, and how does this function know which array to deal with (does it always assume we're talking about GL_ARRAY_BUFFER?



                        if (sw)
                            glDrawArrays(GL_POINTS, 0, 1); //Draw the vertices, once again how does this know which vertices to draw? (Does it always use the ones in GL_ARRAY_BUFFER)
                        sw = !sw;

                        glDisableVertexAttribArray(0); //?
                        glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind


                        //ovrTrackingState tracking_state = ovrHmd_GetTrackingState(hmd, 0.);

                        // Show frame
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
