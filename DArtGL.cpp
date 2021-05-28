#include "DArtGL.h"

#include "ObjReader.h"

const GLuint maxHeight = 500;
//
// extern "C" {
// _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// }

DArtGL::DArtGL(QWidget* parent) : QOpenGLWidget(parent), shaderProgram(this)
{
	setFocusPolicy(Qt::StrongFocus);
	HighlightPos = maxHeight;
	startTimer(10);
}

DArtGL::~DArtGL()
{
	makeCurrent();
	glDeleteProgram(programID);
}

void DArtGL::initializeGL()
{
	initializeOpenGLFunctions();
	core = QOpenGLContext::currentContext()
		->versionFunctions<QOpenGLFunctions_3_3_Core>();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POINT_SMOOTH); //∆Ù”√

	glHint(GL_POINT_SMOOTH, GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH); //∆Ù”√

	glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	
	LightVAO = InitModel("./sun.obj", LightFnum);
	VAO = InitModel("./city2.0_3.obj", WorldFnum);
	SwordVAO = InitModel("./wolf.obj", SwordFnum);
	// VAO = InitModel("./hill.obj");

	// Shader
	InitShader(shaderProgram, "./main.vert", "./main.frag");
	programID = shaderProgram.programId();
	InitShader(LightShaderProgram, "./default.vert", "./default.frag");
	LightProgramID = LightShaderProgram.programId();
	InitShader(SwordShaderProgram, "./sword.vert", "./sword.frag");
	SwordProgramID = SwordShaderProgram.programId();

	qDebug() << model << " " << view << " " << projection;
	// Get a handle for our "MVP" uniform
	Model = glGetUniformLocation(programID, "Model");
	View = glGetUniformLocation(programID, "View");
	Projection = glGetUniformLocation(programID, "Projection");
	Light = glGetUniformLocation(programID, "Light");
	CamPos = glGetUniformLocation(programID, "viewPos");
	HighlightPosId = glGetUniformLocation(programID, "Highlight");

	Light = glGetUniformLocation(LightProgramID, "Light");
	Model = glGetUniformLocation(LightProgramID, "Model");
	View = glGetUniformLocation(LightProgramID, "View");
	Projection = glGetUniformLocation(LightProgramID, "Projection");

	Light = glGetUniformLocation(SwordProgramID, "Light");
	swordModelID = glGetUniformLocation(SwordProgramID, "Model");
	View = glGetUniformLocation(SwordProgramID, "View");
	Projection = glGetUniformLocation(SwordProgramID, "Projection");
	// Projection matrix : 45°„ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection.perspective(45.0, width() * 1.0 / height(), 0.1, 100.0);
	// Model matrix : an identity matrix (model will be at the origin)
	model.setToIdentity();
	swordModel.scale(20);
	swordModel.translate(0, 35, 0);
}

void DArtGL::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void DArtGL::paintGL()
{
	view = camera.GetViewMatrix();
	view.scale(scaleRate);
	view.scale(0.002);
	view.rotate(90, 0, 1, 0);
	qDebug() << camera.Yaw << " " << camera.Pitch;
	view.translate(camera.Yaw, 0, camera.Pitch);
	view.rotate(ryRate, 0, 1, 0);
	view.rotate(rxRate, 1, 0, 0);
	view.rotate(rzRate, 0, 0, 1);
	view.translate(-camera.Yaw, 0, -camera.Pitch);

	// Clear the screen
	glClearColor(183 / 255.0, 223 / 255.0, 231 / 255.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_LIGHTING);
	core->glEnable(GL_LIGHT0);
	// Use our shader
	glUseProgram(programID);
	glUniform3fv(Light, 1, lightPos);
	float camPosition[3] = {camera.Position.x(), camera.Position.y(), camera.Position.z()};
	glUniform3fv(CamPos, 1, camPosition);
	glUniformMatrix4fv(Model, 1, GL_FALSE, model.data());
	glUniformMatrix4fv(View, 1, GL_FALSE, view.data());
	glUniformMatrix4fv(Projection, 1, GL_FALSE, projection.data());
	glUniform1i(HighlightPosId, HighlightPos);
	// render the cube
	core->glBindVertexArray(VAO);
	if (LineMode)
		core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, WorldFnum);
	// glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(LightProgramID);
	glUniform3fv(Light, 1, lightPos);
	glUniformMatrix4fv(Model, 1, GL_FALSE, model.data());
	glUniformMatrix4fv(View, 1, GL_FALSE, view.data());
	glUniformMatrix4fv(Projection, 1, GL_FALSE, projection.data());
	core->glBindVertexArray(LightVAO);
	glDrawArrays(GL_TRIANGLES, 0, LightFnum);


	glUseProgram(SwordProgramID);
	glUniform3fv(Light, 1, lightPos);
	glUniformMatrix4fv(swordModelID, 1, GL_FALSE, swordModel.data());
	glUniformMatrix4fv(View, 1, GL_FALSE, view.data());
	glUniformMatrix4fv(Projection, 1, GL_FALSE, projection.data());
	core->glBindVertexArray(SwordVAO);
	glDrawArrays(GL_TRIANGLES, 0, SwordFnum);
}

void DArtGL::keyPressEvent(QKeyEvent* event)
{
	qDebug() << event->key();
	float sensity = 0.1f;
	if (event->key() == Qt::Key_A)
	{
		camera.ProcessKeyboard(LEFT, sensity);
	}
	if (event->key() == Qt::Key_D)
	{
		camera.ProcessKeyboard(RIGHT, sensity);
	}
	if (event->key() == Qt::Key_W)
	{
		camera.ProcessKeyboard(FORWARD, sensity);
	}
	if (event->key() == Qt::Key_S)
	{
		camera.ProcessKeyboard(BACKWARD, sensity);
	}
	if (event->key() == Qt::Key_Q)
	{
		camera.ProcessKeyboard(UP, sensity);
	}
	if (event->key() == Qt::Key_Z)
	{
		camera.ProcessKeyboard(DOWN, sensity);
	}
	if (event->key() == Qt::Key_L)
	{
		LineMode = !LineMode;
	}
}

void DArtGL::mousePressEvent(QMouseEvent* e)
{
	isMoving = true;
	lastPos = e->pos();
}

void DArtGL::mouseMoveEvent(QMouseEvent* event)
{
	if (isMoving)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			QPoint dpos = (event->pos() - lastPos);
			float dx = dpos.x() * 1.0 / width();
			float dy = dpos.y() * 1.0 / height();
			ryRate += dx * 100;
			rzRate += dy * 100;
		}
		else if (event->buttons() & Qt::MidButton)
		{
			QPoint dpos = (event->pos() - lastPos);
			float dx = -dpos.x() * 1.0 / width();
			float dy = dpos.y() * 1.0 / height();
			camera.ProcessMouseMovement(dx * 600 / (sqrt(sqrt(scaleRate))),
			                            dy * 400 / (sqrt(sqrt(scaleRate))));
		}
		update();
		lastPos = event->pos();
	}
}

void DArtGL::mouseReleaseEvent(QMouseEvent* event)
{
	isMoving = false;
}

void DArtGL::timerEvent(QTimerEvent*)
{
	timeCounter++;
	lightPos[1] = (sin(timeCounter * 0.01) * 0.1 + 1) * 650;
	lightPos[0] = (cos(timeCounter * 0.01) * 0.5) * 2000;
	lightPos[2] = (sin(timeCounter * 0.01) * 0.5) * 2000;

	qDebug() << "*" << lightPos[0] << " " << lightPos[1] << " " << lightPos[2];

	swordModel.rotate(0.5, 0, 1, 0);

	swordModel.translate(0, sin(timeCounter * 0.01) / 50.0, 0);

	HighlightPos -= 100.0 / HighlightPos + 0.1;
	if (HighlightPos < 80)
	{
		HighlightPos = maxHeight;
	}
	qDebug() << "H" << HighlightPos;
	update();
}

void DArtGL::wheelEvent(QWheelEvent* event)
{
	if (event->delta() > 0)
	{
		scaleRate *= 1.1;
	}
	else
	{
		scaleRate *= 0.9;
	}
	update();
}

GLuint DArtGL::InitModel(QString obj, GLuint& face_cnt)
{
	ObjReader reader(obj);
	modelData = reader.GetVertices();

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	core->glGenVertexArrays(1, &VAO);
	core->glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double) * modelData.size(), &modelData[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	core->glBindVertexArray(0);
	face_cnt = modelData.size() / 6;
	return VAO;
}

bool DArtGL::InitShader(QOpenGLShaderProgram& shaderProgram, QString vert, QString frag)
{
	bool success;
	// load and compile vertex shader
	success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
	                                                vert);
	qDebug() << success;
	// load and compile fragment shader
	success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                                frag);
	qDebug() << success;
	shaderProgram.link();
	return true;
}
