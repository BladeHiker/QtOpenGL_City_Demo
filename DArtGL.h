#pragma once
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtGui>
#include <QtOpenGL/QtOpenGL>
#include <vector>
#include "Camera.h"

class DArtGL :
	public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	explicit DArtGL(QWidget* parent = 0);
	~DArtGL() override;
protected:
	void initializeGL(void) override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void timerEvent(QTimerEvent*) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	QOpenGLFunctions_3_3_Core* core;
	QOpenGLShaderProgram shaderProgram;
	QOpenGLShaderProgram LightShaderProgram;
	QOpenGLShaderProgram SwordShaderProgram;
	GLuint programID;
	GLuint LightProgramID;
	GLuint SwordProgramID;
	GLuint VAO;
	GLuint LightVAO;
	GLuint SwordVAO;
	QMatrix4x4 projection;
	QMatrix4x4 view;
	QMatrix4x4 model;
	QMatrix4x4 swordModel;
	GLuint Model;
	GLuint View;
	GLuint Projection;
	GLuint HighlightPosId;
	GLuint HighlightPos;

	GLuint swordModelID;
	GLuint swordViewID;
	GLuint swordProjectionID;
	GLuint CamPos;

	GLuint WorldFnum;
	GLuint SwordFnum;
	GLuint LightFnum;

	float lightPos[3] = {10, 10, 10};
	GLuint Light;

	unsigned int timeCounter = 0;

	bool LineMode = false;
	
	QPoint lastPos;
	bool isMoving = false;

	std::vector<double> modelData;

	Camera camera;

	GLfloat scaleRate = 1;
	GLfloat rxRate = 0;
	GLfloat ryRate = 0;
	GLfloat rzRate = 0;

	GLfloat SwordRotate = 0;

	GLuint InitModel(QString obj, GLuint& face_cnt);
	bool InitShader(QOpenGLShaderProgram& shaderProgram, QString vert, QString frag);
};
