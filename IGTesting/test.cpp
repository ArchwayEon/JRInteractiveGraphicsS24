#include "pch.h"
#include "..\JRInteractiveGraphicsS24\Texture.h"
#include "..\JRInteractiveGraphicsS24\VertexBuffer.h"
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

	TEST(AVertexBuffer, CanQueryItsVertexData) {
		EXPECT_TRUE(SetUpGraphics());
		VertexBuffer sut(8);
		sut.AddVertexData(8, 1.1f, 2.2f, 3.3f, 0.1f, 0.2f, 0.3f, 0.0f, 1.0f);
		sut.AddVertexData(8, 1.4f, 2.5f, 3.6f, 0.4f, 0.5f, 0.6f, 4.1f, 5.1f);
		float actual = sut.GetVertexDataValue(1, 5);
		float expected = 0.6f;
		EXPECT_EQ(actual, expected);
	}

	TEST(AVertexBuffer, CanSetSpecificVertexData) {
		EXPECT_TRUE(SetUpGraphics());
		VertexBuffer sut(8);
		sut.AddVertexData(8, 1.1f, 2.2f, 3.3f, 0.1f, 0.2f, 0.3f, 0.0f, 1.0f);
		sut.AddVertexData(8, 1.4f, 2.5f, 3.6f, 0.4f, 0.5f, 0.6f, 4.1f, 5.1f);
		sut.SetVertexDataValue(1, 5, 0.7f);
		float actual = sut.GetVertexDataValue(1, 5);
		float expected = 0.7f;
		EXPECT_EQ(actual, expected);
	}


}
