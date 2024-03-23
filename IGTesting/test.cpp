#include "pch.h"
#include "..\JRInteractiveGraphicsS24\Texture.h"
#include "..\JRInteractiveGraphicsS24\IndexBuffer.h"
#include "..\JRInteractiveGraphicsS24\Generate.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <memory>

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

	TEST(AnIndexBuffer, CanAddMultipleIndexData) {
		EXPECT_TRUE(SetUpGraphics());
		IndexBuffer sut;
		EXPECT_TRUE(sut.GetBufferId() != 0);
		sut.AddIndexData(3, 1, 2, 3);
		auto& indexData = sut.GetIndexData();
		EXPECT_TRUE(indexData[0] == 1);
		EXPECT_TRUE(indexData[1] == 2);
		EXPECT_TRUE(indexData[2] == 3);
		glfwTerminate();
	}

	TEST(Generate, CanFillALineCircleIndexBuffer) {
		EXPECT_TRUE(SetUpGraphics());
		std::shared_ptr<IndexBuffer> sut = std::make_shared<IndexBuffer>();
		Generate::LineCircleIndexes(sut, 6, true);
		auto& indexData = sut->GetIndexData();
		EXPECT_TRUE(indexData.size() == 12);
		EXPECT_TRUE(indexData[0] == 0);
		EXPECT_TRUE(indexData[11] == 0);
		glfwTerminate();
	}

}
