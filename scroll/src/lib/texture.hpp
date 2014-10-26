
#pragma once

//
// テクスチャ管理
// FIXME:OpenGLに依存している
//

#include "defines.hpp"
#include <string>
#include <GLFW/glfw3.h>


class Texture {
	GLuint id_;

  int width_;
  int height_;

	
public:
	Texture(const std::string& filename);
	~Texture();

  // このクラスはコピー禁止
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  // サイズを返す
  int width() const;
  int height() const;

  // OpenGLのコンテキストに拘束する
	void bind() const;

  // 拘束を解除
	void unbind() const;


private:
  // テクスチャの基本的なパラメーター設定を行う
  static void setupTextureParam();
  
	void setupPng(const std::string& filename);
  
};
