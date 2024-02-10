#include "pch.h"
#include "..\JRInteractiveGraphicsS24\Texture.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

namespace igtesting
{
	TEST(ATexture, HasNullTextureDataWhenCreated) {
		Texture sut;
		EXPECT_TRUE(sut.GetTextureData() == nullptr);
	}

	TEST(ATexture, CanSetTheTextureData) {
		Texture sut;
		unsigned char data[] = { 255, 255, 255, 255	};
		sut.SetTextureData(4, data);
		EXPECT_TRUE(sut.GetTextureData() != nullptr);
		EXPECT_TRUE(sut.IsLoadedFromFile() == false);
	}

	bool SetUpGraphics() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(100, 100, "Testing", NULL, NULL);
		if (window == nullptr) {
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return false;
		}
		return true;
	}

	TEST(ATexture, ShouldInitializeItsTextureId) {
		EXPECT_TRUE(SetUpGraphics());
		Texture sut;
		EXPECT_TRUE(sut.GetTextureId() != 0);
		glfwTerminate();
	}


}
